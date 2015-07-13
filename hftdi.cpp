#include "hftdi.h"
#include "stdlib.h"
#include <memory.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "hTools.h"

void    thread_stream_poll(void*    p)
{
    LOGE("thread_stream_poll init\n");

    hftdi*  ftdi = (hftdi*)p;
    husb* usb = ftdi->m_usb;
    pthread_mutex_t     pt;
    pthread_mutex_init(&pt,NULL);
    int r = 0;
    int gr = 0;
    while(true){
        pthread_cond_wait(&ftdi->m_cond,&pt);
        LOGE("pthread_cond_wait ok\n");

        do{
            gr = poll(&usb->m_pfd,1,5000);
            if(gr > 0){
                r = usb->reap_for_handle();
            }else if(gr == -1){
                break;
            }else if(gr !=0 || gr !=-1){
                continue;
            }
        }while(r != 2);
        usleep(10 * 1000);
        LOGE("readly wakeup main thread\n");

        pthread_cond_signal(&ftdi->m_cond_all_finish);
    }
}



void hftdi::init(bool isquick)
{
    m_ftdi = (ftdi_context*)malloc(sizeof(ftdi_context));
    m_usb  = new husb();
    m_usb->init();

    m_ftdi->usb_ctx = NULL;
    m_ftdi->usb_dev = NULL;
    m_ftdi->usb_read_timeout = 5000;
    m_ftdi->usb_write_timeout = 5000;

    m_ftdi->type = TYPE_BM;    /* chip type */
    m_ftdi->baudrate = -1;
    m_ftdi->bitbang_enabled = 0;  /* 0: normal mode 1: any of the bitbang modes enabled */

    m_ftdi->readbuffer = NULL;
    m_ftdi->readbuffer_offset = 0;
    m_ftdi->readbuffer_remaining = 0;
    m_ftdi->writebuffer_chunksize = 4096;
    m_ftdi->max_packet_size = 0;
    m_ftdi->error_str = NULL;
    m_ftdi->module_detach_mode = AUTO_DETACH_SIO_MODULE;
    m_ftdi->bitbang_mode = 1; /* when bitbang is enabled this holds the number of the mode  */
    m_ftdi->eeprom = 0;
    pthread_cond_init(&m_cond,NULL);
    pthread_cond_init(&m_cond_all_finish,NULL);

    if(!m_usb->is_running()){
        LOGE("current usb_is not running so we call ftdi_set_interface()\n\n");

        ftdi_set_interface(INTERFACE_ANY);
    }

}
void hftdi::reinit()
{
    ::free(m_ftdi);
    delete  m_usb;
    m_ftdi = 0;
    m_usb  = 0;
    init();
}



hftdi::hftdi()
{
    init();
}


int hftdi::ftdi_usb_purge_rx_buffer()
{
   if(m_usb->control_transfer(FTDI_DEVICE_OUT_REQTYPE,SIO_RESET_REQUEST,
                            SIO_RESET_PURGE_RX,m_ftdi->index,NULL,0,m_ftdi->usb_write_timeout) < 0)
       return   -1;

    m_ftdi->readbuffer_offset = 0;
    m_ftdi->readbuffer_remaining = 0;
   return   0;
}

int hftdi::ftdi_usb_purge_tx_buffer()
{
    if(m_usb->control_transfer(FTDI_DEVICE_OUT_REQTYPE,SIO_RESET_REQUEST,SIO_RESET_PURGE_TX,m_ftdi->index,NULL,0,m_ftdi->usb_write_timeout) < 0)
        return  -1;

    return  0;
}


int hftdi::ftdi_set_latency_timer(unsigned char latency)
{
    if(m_usb->control_transfer(FTDI_DEVICE_OUT_REQTYPE,SIO_SET_LATENCY_TIMER_REQUEST,latency,m_ftdi->index,NULL,0,m_ftdi->usb_write_timeout) < 0)
        return  -2;
    return  0;
}

int hftdi::ftdi_set_bitmode(unsigned char btmask, unsigned char mode)
{
    unsigned short usb_val;
    usb_val = btmask;
    usb_val |= (mode << 8);

    if(m_usb->control_transfer(FTDI_DEVICE_OUT_REQTYPE,SIO_SET_BITMODE_REQUEST,usb_val,m_ftdi->index,NULL,0,m_ftdi->usb_write_timeout) < 0)
        return  -1;

    m_ftdi->bitbang_mode = mode;
    m_ftdi->bitbang_enabled = (mode == BITMODE_RESET) ? 0 : 1;
    return  0;
}


int hftdi::ftdi_read_data(unsigned char *buf, int len)
{
    int actual_length = 0;
    int ret = m_usb->bulk_transfer(m_ftdi->out_ep,buf,len,&actual_length,m_ftdi->usb_read_timeout);
    if(ret < 0)
        return  -1;
    return  ret;
}

int hftdi::ftdi_poll_modem_status(unsigned short *status)
{
    char    usb_val[2];
    if(m_usb->control_transfer(FTDI_DEVICE_IN_REQTYPE,SIO_POLL_MODEM_STATUS_REQUEST,0,m_ftdi->index,(unsigned char*)usb_val,2,m_ftdi->usb_read_timeout) != 2)
        return  -1;
    *status = (usb_val[1] << 8) | (usb_val[0] & 0xFF);
    return  0;
}

int hftdi::ftdi_write_data(unsigned char *buf, int size)
{
    int actual_len = 0;
    if(m_usb->bulk_transfer(m_ftdi->in_ep,buf,size,&actual_len,m_ftdi->usb_write_timeout) < 0)
        return  -1;

    return  actual_len;
}


int hftdi::ftdi_usb_open()
{
//    struct

    if(m_usb->claim_interface(m_ftdi->interface) < 0)
        return  -1;
    if(ftdi_usb_reset() != 0)
        return  -6;
    m_ftdi->type = TYPE_232H;

    if(ftdi_set_baudrate(9600) != 0)
        return  -7;

    return  0;
}

int hftdi::ftdi_set_baudrate(int baudrate)
{
    unsigned short value, index;
    int actual_baudrate;


    if (m_ftdi->bitbang_enabled)
    {
        baudrate = baudrate*4;
    }

    actual_baudrate = ftdi_convert_baudrate(baudrate,&value, &index);
    if (actual_baudrate <= 0)
        return  -1;

    // Check within tolerance (about 5%)
    if ((actual_baudrate * 2 < baudrate /* Catch overflows */ )
            || ((actual_baudrate < baudrate)
                ? (actual_baudrate * 21 < baudrate * 20)
                : (baudrate * 21 < actual_baudrate * 20)))
        return  -1;

    if (m_usb->control_transfer(FTDI_DEVICE_OUT_REQTYPE,
                                SIO_SET_BAUDRATE_REQUEST, value,
                                index, NULL, 0, m_ftdi->usb_write_timeout) < 0)
        return  -2;

    m_ftdi->baudrate = baudrate;
    return 0;
}

int hftdi::ftdi_usb_reset()
{
    if(m_usb->control_transfer(FTDI_DEVICE_OUT_REQTYPE,SIO_RESET_REQUEST,SIO_RESET_SIO,m_ftdi->index,NULL,0,m_ftdi->usb_write_timeout) < 0)
        return  -1;
    m_ftdi->readbuffer_offset = 0;
    m_ftdi->readbuffer_remaining = 0;
    return  0;
}


int hftdi::ftdi_convert_baudrate(int baudrate, unsigned short *value, unsigned short *index)
{
    int best_baud;
    unsigned long encoded_divisor;

    if (baudrate <= 0)
    {
        // Return error
        return -1;
    }

#define H_CLK 120000000
#define C_CLK  48000000
    if ((m_ftdi->type == TYPE_2232H) || (m_ftdi->type == TYPE_4232H) || (m_ftdi->type == TYPE_232H))
    {
        if(baudrate*10 > H_CLK /0x3fff)
        {
            /* On H Devices, use 12 000 000 Baudrate when possible
               We have a 14 bit divisor, a 1 bit divisor switch (10 or 16)
               three fractional bits and a 120 MHz clock
               Assume AN_120 "Sub-integer divisors between 0 and 2 are not allowed" holds for
               DIV/10 CLK too, so /1, /1.5 and /2 can be handled the same*/
            best_baud = ftdi_to_clkbits(baudrate, H_CLK, 10, &encoded_divisor);
            encoded_divisor |= 0x20000; /* switch on CLK/10*/
        }
        else
            best_baud = ftdi_to_clkbits(baudrate, C_CLK, 16, &encoded_divisor);
    }
    else if ((m_ftdi->type == TYPE_BM) || (m_ftdi->type == TYPE_2232C) || (m_ftdi->type == TYPE_R ))
    {
        best_baud = ftdi_to_clkbits(baudrate, C_CLK, 16, &encoded_divisor);
    }
    else
    {
        best_baud = ftdi_to_clkbits_AM(baudrate, &encoded_divisor);
    }
    // Split into "value" and "index" values
    *value = (unsigned short)(encoded_divisor & 0xFFFF);
    if (m_ftdi->type == TYPE_2232H || m_ftdi->type == TYPE_4232H || m_ftdi->type == TYPE_232H)
    {
        *index = (unsigned short)(encoded_divisor >> 8);
        *index &= 0xFF00;
        *index |= m_ftdi->index;
    }
    else
        *index = (unsigned short)(encoded_divisor >> 16);

    // Return the nearest baud rate
    return best_baud;
}


int hftdi::ftdi_to_clkbits(int baudrate, unsigned int clk, int clk_div, unsigned long *encoded_divisor)
{
    static const char frac_code[8] = {0, 3, 2, 4, 1, 5, 6, 7};
    int best_baud = 0;
    int divisor, best_divisor;
    if (baudrate >=  clk/clk_div)
    {
        *encoded_divisor = 0;
        best_baud = clk/clk_div;
    }
    else if (baudrate >=  clk/(clk_div + clk_div/2))
    {
        *encoded_divisor = 1;
        best_baud = clk/(clk_div + clk_div/2);
    }
    else if (baudrate >=  clk/(2*clk_div))
    {
        *encoded_divisor = 2;
        best_baud = clk/(2*clk_div);
    }
    else
    {
        /* We divide by 16 to have 3 fractional bits and one bit for rounding */
        divisor = clk*16/clk_div / baudrate;
        if (divisor & 1) /* Decide if to round up or down*/
            best_divisor = divisor /2 +1;
        else
            best_divisor = divisor/2;
        if(best_divisor > 0x20000)
            best_divisor = 0x1ffff;
        best_baud = clk*16/clk_div/best_divisor;
        if (best_baud & 1) /* Decide if to round up or down*/
            best_baud = best_baud /2 +1;
        else
            best_baud = best_baud /2;
        *encoded_divisor = (best_divisor >> 3) | (frac_code[best_divisor & 0x7] << 14);
    }
    return best_baud;
}

bool hftdi::hwrite(uchar a1, uchar a2, uchar a3, uchar a4, uchar a5)
{
    usleep(10 * 1000);
    unsigned    char buf[512];
    memset(buf,0,512);
    unsigned    char p[] = {a1,a2,a3,a4,a5};
    unsigned    short   st = 0;

    ftdi_read_data(buf,6);
    if(buf[0] != 0xFF){
        ftdi_usb_purge_rx_buffer();
        ftdi_usb_purge_tx_buffer();
        LOGE("clear buffer\n");

    }

    ftdi_poll_modem_status(&st);

    unsigned char z1,z2;
    z1 = st>>8;
    z2 = st;

    if(z1 & 0xF0){
        if(z1 & 0x10){
            //Clear to send CTS
            //inactive
        }
        else{
            //active
        }
        if(z1 & 0x20){
            //Data set ready DTS
            //active
        }
        else{

        }
        if(z1 & 0x40){
            //Ring indicator RI
        }
        else{

        }
        if(z1 & 0x80){
            //Receive line signal detect
        }
        else{

        }

        if(z2 & 0x0){
            //Data ready DR
        }
        if(z2 & 0x2){
            //Overrun error
        }
        if(z2 & 0x4){
            //parity error
        }
        if(z2 & 0x8){
            //Framing error
        }
        if(z2 & 0x10){
            //break interrupt
        }
        if(z2 & 0x20){
            //Transmitter holding register
        }
        if(z2 & 0x40){
            //Transmitter empty
        }
        if(z2 & 0x80){
            //Error in RCVR FIFO
        }

    }

    if( 5 != ftdi_write_data(p,5)){
        LOGE("write error write\n");

        return  false;
    }

    int ret = ftdi_read_data(buf,6);
    LOGE("read buf = %d\n",ret);


    if(ret == 6){
        if(buf[0] == 0xFF)
            return  true;
        else{
            LOGE("write error return value\n");

            return  true;
        }
    }
    return  true;
}


int hftdi::ftdi_setflowctrl(int flowctrl)
{
    if(m_usb->control_transfer(FTDI_DEVICE_OUT_REQTYPE,SIO_SET_FLOW_CTRL_REQUEST,0,(flowctrl | m_ftdi->index),NULL,0,m_ftdi->usb_write_timeout) < 0)
        return  -1;
    return  0;
}


int hftdi::ftdi_to_clkbits_AM(int baudrate, unsigned long *encoded_divisor)
{
    static const char frac_code[8] = {0, 3, 2, 4, 1, 5, 6, 7};
    static const char am_adjust_up[8] = {0, 0, 0, 1, 0, 3, 2, 1};
    static const char am_adjust_dn[8] = {0, 0, 0, 1, 0, 1, 2, 3};
    int divisor, best_divisor, best_baud, best_baud_diff;
    int i;
    divisor = 24000000 / baudrate;

    // Round down to supported fraction (AM only)
    divisor -= am_adjust_dn[divisor & 7];

    // Try this divisor and the one above it (because division rounds down)
    best_divisor = 0;
    best_baud = 0;
    best_baud_diff = 0;
    for (i = 0; i < 2; i++)
    {
        int try_divisor = divisor + i;
        int baud_estimate;
        int baud_diff;

        // Round up to supported divisor value
        if (try_divisor <= 8)
        {
            // Round up to minimum supported divisor
            try_divisor = 8;
        }
        else if (divisor < 16)
        {
            // AM doesn't support divisors 9 through 15 inclusive
            try_divisor = 16;
        }
        else
        {
            // Round up to supported fraction (AM only)
            try_divisor += am_adjust_up[try_divisor & 7];
            if (try_divisor > 0x1FFF8)
            {
                // Round down to maximum supported divisor value (for AM)
                try_divisor = 0x1FFF8;
            }
        }
        // Get estimated baud rate (to nearest integer)
        baud_estimate = (24000000 + (try_divisor / 2)) / try_divisor;
        // Get absolute difference from requested baud rate
        if (baud_estimate < baudrate)
        {
            baud_diff = baudrate - baud_estimate;
        }
        else
        {
            baud_diff = baud_estimate - baudrate;
        }
        if (i == 0 || baud_diff < best_baud_diff)
        {
            // Closest to requested baud rate so far
            best_divisor = try_divisor;
            best_baud = baud_estimate;
            best_baud_diff = baud_diff;
            if (baud_diff == 0)
            {
                // Spot on! No point trying
                break;
            }
        }
    }
    // Encode the best divisor value
    *encoded_divisor = (best_divisor >> 3) | (frac_code[best_divisor & 7] << 14);
    // Deal with special cases for encoded value
    if (*encoded_divisor == 1)
    {
        *encoded_divisor = 0;    // 3000000 baud
    }
    else if (*encoded_divisor == 0x4001)
    {
        *encoded_divisor = 1;    // 2000000 baud (BM only)
    }
    return best_baud;
}



int hftdi::ftdi_set_interface(ftdi_interface interface)
{
    if (m_ftdi == NULL)
        return  -2;

    switch (interface)
    {
        case INTERFACE_ANY:
        case INTERFACE_A:
            m_ftdi->interface = 0;
            m_ftdi->index     = INTERFACE_A;
            m_ftdi->in_ep     = 0x02;
            m_ftdi->out_ep    = 0x81;
            break;
        case INTERFACE_B:
            m_ftdi->interface = 1;
            m_ftdi->index     = INTERFACE_B;
            m_ftdi->in_ep     = 0x04;
            m_ftdi->out_ep    = 0x83;
            break;
        case INTERFACE_C:
            m_ftdi->interface = 2;
            m_ftdi->index     = INTERFACE_C;
            m_ftdi->in_ep     = 0x06;
            m_ftdi->out_ep    = 0x85;
            break;
        case INTERFACE_D:
            m_ftdi->interface = 3;
            m_ftdi->index     = INTERFACE_D;
            m_ftdi->in_ep     = 0x08;
            m_ftdi->out_ep    = 0x87;
            break;
        default:
            return  -1;
    }
    return 0;
}

void    hftdi::ftdi_stream_restart(libusb_transfer_cb_fn cb, int packcount)
{
    m_usb->set_urb_buf();
    m_usb->restart_stream();
    m_usb->wait_all_transfer_finish(cb);
}

void    hftdi::ftdi_stream_start(libusb_transfer_cb_fn callback, int pack_count)
{
    static  bool    first = true;
    if(first){
        first = false;
        m_usb->set_urb_buf();
    }
    m_usb->stream_start();
    m_usb->wait_all_transfer_finish(callback);
}


int    hftdi::ftdi_usb_close()
{
    m_usb->close();
    reinit();
    return  0;
}


void     hftdi::ftdi_usb_close_internal()
{
    m_usb->close();
}

void     hftdi::ftdi_usb_open_internal()
{
    m_usb->init();
//    m_usb->de
}
