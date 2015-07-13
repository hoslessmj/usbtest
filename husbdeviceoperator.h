#ifndef HUSBDEVICEOPERATOR_H
#define HUSBDEVICEOPERATOR_H

#include "hftdi.h"
#include "hcamer.h"
#include <pthread.h>
#include "hTools.h"

class   hCamer;
class   hAllCamer;


class hUsbDeviceOperator
{
public:
    hUsbDeviceOperator();


    void    start();


    void     normal_start();
    void     quick_start();


    bool     init();
    hCamer* ch_data(unsigned int ch){
        return m_camers[ch];
    }
    hCamer* all_camer(){
        return  (hCamer*)m_ac;
    }

    typedef enum error_code{
        no_device = 0x1000,
        ok        = 0,
    }ERROR_CODE;

    error_code    _start();
    error_code    _restart();

    void   stop_capture();
    void   close_device();
private:
    bool	hwrite(unsigned char a1,unsigned char a2,unsigned char a3,unsigned char a4,unsigned char a5);
    bool    hwrite(unsigned char a1,unsigned char a2,uchar a3){
        return hwrite(0x55,0xaa,a1,a2,a3);
    }
    void    _start_dsp();
    void	_set_video(int p);
    void   restart_capture();
    void   start_capture();
    void   start_dsp_chanel(int ch);
    bool   open_device();
    void   start_dsp();
public:
    ftdi_context*    m_contex;
    hCamer*          m_camers[4];
    hAllCamer*       m_ac;
    pthread_cond_t   m_cond;
    bool             m_read_data_thread_isrun;
    bool             m_work_data_thread_isrun;
    bool             m_device_disconnect;

    hftdi*           m_ftdi;

};

#endif // HUSBDEVICEOPERATOR_H
