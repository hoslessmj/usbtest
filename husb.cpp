#include "husb.h"
#include <sys/ioctl.h>
#include <string.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "hTools.h"

void    thread_poll(void* p)
{
    husb*   usb = (husb*)p;
    for(;;){
        if(::poll(&usb->m_pfd,1,1000)){
            pthread_cond_signal(&usb->m_sync);
        }
        if(usb->exit == true){
            break;
        }
    }
     LOGE("thread_poll exit\n");

}

struct hosless_set_buf{
    unsigned    int     index;
    struct usbfs_urb*    uurb;
};


int husb::set_urb_buf()
{
    for(int x=0;x < 128;++x){
        m_urbs[x] =(usbfs_urb*)malloc(sizeof(usbfs_urb));
        m_urbs[x]->buffer = (unsigned char*)malloc(0x10000);
        m_urbs[x]->buffer_length = 0x10000;
        if(!m_urbs[x] || !m_urbs[x]->buffer){
            LOGE("husb::husb() malloc error\n");
            return -1;
        }
    }

    int r = 0;
    struct hosless_set_buf buf;
    for(int x=0;x < 128;++x){
        buf.index = x;
        buf.uurb = m_urbs[x];
        r = ::ioctl(m_fd,IOCTL_USBFS_HOSLESS_SET_BUF,&buf);
        if(r !=0){
            return  r;
        }
    }
    return  0;
}


int husb::stream_start()
{
    int p = 0;
    int r = ::ioctl(m_fd,IOCTL_USBFS_HOSLESS_STREAM_START,&p);
    return  r;
}



husb::husb()
{
    pthread_cond_init(&m_sync,NULL);
}

bool husb::init()
{
    m_fd = ::open("/dev/LTUSB04",O_RDWR);
    if(m_fd){
        m_pfd.fd = m_fd;
        m_pfd.events = POLLIN ;
        m_pfd.revents= 0;
    }
    exit = false;
    pthread_create(&m_thread,NULL,(void*(*)(void*))thread_poll,this);
    return  m_fd != 0 ? true : false;
}
void husb::join()
{
//    pthread_join(m_thread,NULL);
    exit = true;
    pthread_join(m_thread,NULL);
}

int husb::submit_transfer(libusb_transfer *tr)
{
    static  int cur = 0;
    usbfs_urb*  urbs = m_urbs[cur++];
    if(cur>=128)
        cur = 0;
//    int is_out = (transfr->endpoint & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_OUT;

    urbs->usercontext = tr;
    urbs->type = USBFS_URB_TYPE_BULK;
    urbs->stream_id = 0;
    urbs->endpoint  = tr->endpoint;
    urbs->buffer    = tr->buffer;
    urbs->buffer_length = tr->length;

    int r = ::ioctl(m_fd,IOCTL_USBFS_SUBMITURB,urbs);
    if(r < 0)
        return  LIBUSB_ERROR_IO;
    return  r;
}

void husb::close()
{
    if(m_fd == -1)
        return;
    exit = true;
    pthread_join(m_thread,NULL);
    ::close(m_fd);
}

int husb::claim_interface(int number)
{
    int r = ::ioctl(m_fd,IOCTL_USBFS_CLAIMINTF,&number);
    if(r){
        return  -1;
    }
    return  r;
}


void husb::read_cb(libusb_transfer *transfer)
{
    char    pbuf[12];
    if(transfer->actual_length == 512){
            memcpy(pbuf,transfer->buffer,12);
    }
}

int husb::bulk_transfer_async(unsigned char endpoint, unsigned char *data, int len, int *transferred, unsigned int timeout, libusb_transfer_cb_fn cb)
{
    static  libusb_transfer sp;
    static  usbfs_urb        urb;
//    memset(&urb,0,sizeof(usbfs_urb));
//    libusb_transfer*    transfr = alloc_transfer(0);
    libusb_transfer*    transfr = &sp;
    int completed = 0;
    int r;

    fill_bulk_transfer(transfr,endpoint,data,len,0,&completed,timeout);
    transfr->callback = cb;
    usbfs_urb*  urbs = &urb;
    int is_out = (transfr->endpoint & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_OUT;
    transfr->flags = 0;
    urbs->usercontext = transfr;
    urbs->type = USBFS_URB_TYPE_BULK;
    urbs->stream_id = 0;
    urbs->endpoint  = endpoint;
    urbs->buffer    = data;
    urbs->buffer_length = len;

    if(!is_out){
        urbs->flags = USBFS_URB_SHORT_NOT_OK;
    }
    if(is_out && transfr->flags & LIBUSB_TRANSFER_ADD_ZERO_PACKET){
        urbs->flags |= USBFS_URB_ZERO_PACKET;
    }

    r = ::ioctl(m_fd,IOCTL_USBFS_SUBMITURB,urbs);
    if(r < 0)
        return  LIBUSB_ERROR_IO;

    return  r;
}

int husb::bulk_transfer(unsigned char endpoint, unsigned char *data, int len, int *transferred, unsigned int timeout)
{
    libusb_transfer*    transfr = alloc_transfer(0);
    int completed = 0;
    int r;

    fill_bulk_transfer(transfr,endpoint,data,len,0,&completed,timeout);
    transfr->callback = husb::read_cb;
    usbfs_urb*  urbs;
    int is_out = (transfr->endpoint & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_OUT;
    transfr->flags = 0;
    urbs = (usbfs_urb*)calloc(1,sizeof(usbfs_urb));
    urbs->usercontext = transfr;
    urbs->type = USBFS_URB_TYPE_BULK;
    urbs->stream_id = 0;
    urbs->endpoint  = endpoint;
    urbs->buffer    = data;
    urbs->buffer_length = len;

    if(!is_out){
        urbs->flags = USBFS_URB_SHORT_NOT_OK;
    }
    if(is_out && transfr->flags & LIBUSB_TRANSFER_ADD_ZERO_PACKET){
        urbs->flags |= USBFS_URB_ZERO_PACKET;
    }


//    if(is_out){
//        urbs->flags |= USBFS_URB_ZERO_PACKET;
//    }

    r = ::ioctl(m_fd,IOCTL_USBFS_SUBMITURB,urbs);
    if(r < 0)
        return  LIBUSB_ERROR_IO;

    handle_events();

    *transferred = transfr->actual_length;
    if(transfr->status == LIBUSB_TRANSFER_COMPLETED)
        return  transfr->actual_length;
    r = transfr->status;

    free_transfer(transfr);
    return  transfr->actual_length;
}





void husb::transfer_callback(libusb_transfer *transfer)
{

}



int husb::control_transfer(uint8_t bmRequestType, uint8_t br, u_int16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wlen, unsigned int timeout)
{
    struct  usbfs_urb*  urb = (usbfs_urb*)calloc(1,sizeof(struct usbfs_urb));
    int completed = 0;

    libusb_transfer*    transfer = alloc_transfer(0);

    unsigned    char*   buffer = (unsigned char*)malloc(LIBUSB_CONTROL_SETUP_SIZE + wlen);

    libusb_control_setup*   setup = (libusb_control_setup*)(void*)buffer;
    setup->bmRequestType          = bmRequestType;
    setup->bRequest               = br;
    setup->wValue                 = libusb_cpu_to_le16(wValue);
    setup->wIndex                 = libusb_cpu_to_le16(wIndex);
    setup->wLength                = libusb_cpu_to_le16(wlen);

    if((bmRequestType & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_OUT)
        memcpy(buffer + LIBUSB_CONTROL_SETUP_SIZE,data,wlen);

    transfer->endpoint = 0;
    transfer->type     = LIBUSB_TRANSFER_TYPE_CONTROL;
    transfer->timeout  = timeout;
    transfer->buffer   = buffer;
    if(setup){
        transfer->length = (int)(LIBUSB_CONTROL_SETUP_SIZE + libusb_le16_to_cpu(setup->wLength));
    }
    transfer->user_data = &completed;
    transfer->timeout   = timeout;
    transfer->flags     = LIBUSB_TRANSFER_FREE_BUFFER;
    transfer->callback  = husb::transfer_callback;

    urb->usercontext    = transfer;
    urb->type           = USBFS_URB_TYPE_CONTROL;
    urb->endpoint       = transfer->endpoint;
    urb->buffer         = transfer->buffer;
    urb->buffer_length  = transfer->length;

    int r = ::ioctl(m_fd,IOCTL_USBFS_SUBMITURB,urb);
    if(r != 0)
        return  r;

    handle_events();

    switch(transfer->status){
    case LIBUSB_TRANSFER_COMPLETED:
        r = transfer->actual_length;
        break;
    default:
        return  -1;
    }
    free_transfer(transfer);
    return  r;
}





libusb_transfer*    husb::alloc_transfer(int iso)
{
    libusb_transfer*    p = (libusb_transfer*)malloc(sizeof(libusb_transfer));
    return  p;
}



//int husb::submit_transfer(libusb_transfer *transfer)
//{
////    switch(transfer->type){
////    case LIBUSB_TRANSFER_TYPE_CONTROL:
////        return  submit_cotrol_transfer(transfer);
////    case LIBUSB_TRANSFER_TYPE_BULK:
////    case LIBUSB_TRANSFER_TYPE_BULK_STREAM:
////        return  submit_bulk_transfer(transfr);
////    case LIBUSB_TRANSFER_TYPE_INTERRUPT:
////        return  submit_bulk_transfer(transfer);
////    case LIBUSB_TRANSFER_TYPE_ISOCHRONOUS:
////        return  LIBUSB_ERROR_INVALID_PARAM;
////    }
//    return  -1;
//}









void    husb::handle_events()
{
    pthread_mutex_t     mu;
    pthread_mutex_init(&mu,NULL);
    for(;;){
        pthread_cond_wait(&m_sync,&mu);
        int r = reap_for_handle();
        if(r == 0)
            break;
    }

//    int r = 0;
//    int cg = 0;
//    do{
//        cg = ::poll(&m_pfd,1,0);
//        if(cg >0){
//            r = reap_for_handle();
//            if(r == 1)
//                continue;
//        }else if(cg == 0){
//            r = reap_for_handle();
//            if(r == 1)
//                break;
//            continue;
//        }else if(cg <   0){
//            break;
//        }
//    }while(r == 0);

}

extern  int                 g_all_size;

int     husb::reap_for_handle_async(libusb_transfer_cb_fn cb)
{
    libusb_transfer    transfer;
    usbfs_urb*          urb;

    int r = ::ioctl(m_fd,IOCTL_USBFS_HOSLESS_GET_URB,&urb);

    if(urb->status == 0 || urb->actual_length>=512){
        transfer.status = LIBUSB_TRANSFER_COMPLETED;
        transfer.actual_length = urb->actual_length;
        transfer.buffer            = (unsigned char*)urb->buffer;
        cb(&transfer);
        return  0;
    }
    return  -1;

}

int     husb::reap_for_handle()
{
    static  int recount = 0;
    libusb_transfer*    transfer;
    usbfs_urb*  urb;

    int r = ::ioctl(m_fd,IOCTL_USBFS_REAPURBNDELAY,&urb);

    if(r == -1){
        return  1;
    }
    if(r < 0){
        return  2;
    }
    transfer = (libusb_transfer*)urb->usercontext;
    if(urb->status == 0){
        transfer->status = LIBUSB_TRANSFER_COMPLETED;
        transfer->actual_length = urb->actual_length;
        transfer->buffer = (unsigned char*)urb->buffer;
        if(transfer->callback)
            transfer->callback(transfer);
    }
    return  0;
}

int      husb::get_urb_state()
{
    return  -1;
}

int     husb::get_queue_urb(libusb_transfer_cb_fn cb)
{
    libusb_transfer    transfer;
    usbfs_urb*  urb;

    int r = ::ioctl(m_fd,IOCTL_USBFS_HOSLESS_GET_URB,&urb);
    if(r == -4096){
        return  -4096;
    }
    if(r < 0){
//        LOGE("get urb return code = %d\n",r);
        return  -1;
    }
    if(urb->status == 0){
        transfer.status = LIBUSB_TRANSFER_COMPLETED;
        transfer.actual_length = urb->actual_length;
        transfer.buffer = (unsigned char*)urb->buffer;
        cb(&transfer);
    }
    return  0;


}

void    husb::wait_all_transfer_finish(libusb_transfer_cb_fn cb)
{
    hElapsedTimer2   tm;
    tm.start();
    int     elapsed = 0;
    int     rtz = 0;
    while(true){
        rtz = get_queue_urb(cb);
        if(rtz == -4096){
            usleep(50);
            continue;
        }

        if(rtz == -1){
//            LOGE("get_queue_urb = -1\n");
            continue;
        }
        elapsed = tm.elapsed();
        if(elapsed <0 || elapsed >=1000){
            LOGE("%d byte / s time = %d\n",g_all_size,elapsed);

            g_all_size = 0;
            tm.restart();
        }
    }
}


void    husb::free_transfer(libusb_transfer *tf)
{
    free(tf);
}
int    husb::release_interface(int iface)
{
    int r = ::ioctl(m_fd,IOCTL_USBFS_RELEASEINTF,&iface);
    if(r){
        return  LIBUSB_ERROR_OTHER;
    }
    return  0;
}

void    husb::restart_stream()
{
   int sp;
   LOGE("restart_stream return value = %d\n",::ioctl(m_fd,IOCTL_USBFS_HOSLESS_STREAM_RESTART,&sp));

}

bool    husb::is_running()
{
    int running = 0;
    int r = ::ioctl(m_fd,IOCTL_USBFS_HOSLESS_GET_RUNNING,&running);
    if(r !=0){
        LOGE("get_running error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n\n\n");
        return  false;
    }
    return  running == 1 ? true : false;
}
