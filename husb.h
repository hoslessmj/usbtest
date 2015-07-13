#ifndef HUSB_H
#define HUSB_H

#include <linux/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <linux/stat.h>
#include <sys/poll.h>
#include <pthread.h>


#define USBFS_MAXDRIVERNAME 255



#define USBFS_URB_SHORT_NOT_OK		0x01
#define USBFS_URB_ISO_ASAP			0x02
#define USBFS_URB_BULK_CONTINUATION	0x04
#define USBFS_URB_QUEUE_BULK		0x10
#define USBFS_URB_ZERO_PACKET		0x40

#define LIBUSB_ENDPOINT_DIR_MASK		0x80
#define LIBUSB_CONTROL_SETUP_SIZE (sizeof(struct libusb_control_setup))

#define MAX_ISO_BUFFER_LENGTH		32768
#define MAX_BULK_BUFFER_LENGTH		16384
#define MAX_CTRL_BUFFER_LENGTH		4096

#define USBFS_CAP_ZERO_PACKET		0x01
#define USBFS_CAP_BULK_CONTINUATION	0x02
#define USBFS_CAP_NO_PACKET_SIZE_LIM	0x04
#define USBFS_CAP_BULK_SCATTER_GATHER	0x08

#define USBFS_DISCONNECT_CLAIM_IF_DRIVER	0x01
#define USBFS_DISCONNECT_CLAIM_EXCEPT_DRIVER	0x02

#define IOCTL_USBFS_CONTROL	_IOWR('U', 0, struct usbfs_ctrltransfer)
#define IOCTL_USBFS_BULK		_IOWR('U', 2, struct usbfs_bulktransfer)
#define IOCTL_USBFS_RESETEP	_IOR('U', 3, unsigned int)
#define IOCTL_USBFS_SETINTF	_IOR('U', 4, struct usbfs_setinterface)
#define IOCTL_USBFS_SETCONFIG	_IOR('U', 5, unsigned int)
#define IOCTL_USBFS_GETDRIVER	_IOW('U', 8, struct usbfs_getdriver)
#define IOCTL_USBFS_SUBMITURB	_IOR('U', 10, struct usbfs_urb)
#define IOCTL_USBFS_DISCARDURB	_IO('U', 11)
#define IOCTL_USBFS_REAPURB	_IOW('U', 12, void *)
#define IOCTL_USBFS_REAPURBNDELAY	_IOW('U', 13, void *)
#define IOCTL_USBFS_CLAIMINTF	_IOR('U', 15, unsigned int)
#define IOCTL_USBFS_RELEASEINTF	_IOR('U', 16, unsigned int)
#define IOCTL_USBFS_CONNECTINFO	_IOW('U', 17, struct usbfs_connectinfo)
#define IOCTL_USBFS_IOCTL         _IOWR('U', 18, struct usbfs_ioctl)
#define IOCTL_USBFS_HUB_PORTINFO	_IOR('U', 19, struct usbfs_hub_portinfo)
#define IOCTL_USBFS_RESET		_IO('U', 20)
#define IOCTL_USBFS_CLEAR_HALT	_IOR('U', 21, unsigned int)
#define IOCTL_USBFS_DISCONNECT	_IO('U', 22)
#define IOCTL_USBFS_CONNECT	_IO('U', 23)
#define IOCTL_USBFS_CLAIM_PORT	_IOR('U', 24, unsigned int)
#define IOCTL_USBFS_RELEASE_PORT	_IOR('U', 25, unsigned int)
#define IOCTL_USBFS_GET_CAPABILITIES	_IOR('U', 26, __u32)
#define IOCTL_USBFS_DISCONNECT_CLAIM	_IOR('U', 27, struct usbfs_disconnect_claim)
#define IOCTL_USBFS_ALLOC_STREAMS	_IOR('U', 28, struct usbfs_streams)
#define IOCTL_USBFS_FREE_STREAMS	_IOR('U', 29, struct usbfs_streams)

#define IOCTL_USBFS_HOSLESS_GET_URBSIZE            _IOR('U',100, int)

#define IOCTL_USBFS_HOSLESS_SET_BUF _IOR('U',103,struct hosless_set_buf)

#define IOCTL_USBFS_HOSLESS_STREAM_START    _IOR('U',101,unsigned int)

#define IOCTL_USBFS_HOSLESS_GET_URB             _IOR('U',104,void*)

#define IOCTL_USBFS_HOSLESS_GET_RUNNING         _IOR('U',105,int)

#define IOCTL_USBFS_HOSLESS_STREAM_RESTART      _IOR('U',106,int)



#define libusb_le16_to_cpu libusb_cpu_to_le16

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;




typedef void (*libusb_transfer_cb_fn)(struct libusb_transfer *transfer);



enum usbfs_urb_type {
    USBFS_URB_TYPE_ISO = 0,
    USBFS_URB_TYPE_INTERRUPT = 1,
    USBFS_URB_TYPE_CONTROL = 2,
    USBFS_URB_TYPE_BULK = 3,
};

enum libusb_endpoint_direction {
    /** In: device-to-host */
    LIBUSB_ENDPOINT_IN = 0x80,

    /** Out: host-to-device */
    LIBUSB_ENDPOINT_OUT = 0x00
};

enum libusb_transfer_flags {
    /** Report short frames as errors */
    LIBUSB_TRANSFER_SHORT_NOT_OK = 1<<0,

    /** Automatically free() transfer buffer during libusb_free_transfer() */
    LIBUSB_TRANSFER_FREE_BUFFER = 1<<1,

    /** Automatically call libusb_free_transfer() after callback returns.
     * If this flag is set, it is illegal to call libusb_free_transfer()
     * from your transfer callback, as this will result in a double-free
     * when this flag is acted upon. */
    LIBUSB_TRANSFER_FREE_TRANSFER = 1<<2,

    /** Terminate transfers that are a multiple of the endpoint's
     * wMaxPacketSize with an extra zero length packet. This is useful
     * when a device protocol mandates that each logical request is
     * terminated by an incomplete packet (i.e. the logical requests are
     * not separated by other means).
     *
     * This flag only affects host-to-device transfers to bulk and interrupt
     * endpoints. In other situations, it is ignored.
     *
     * This flag only affects transfers with a length that is a multiple of
     * the endpoint's wMaxPacketSize. On transfers of other lengths, this
     * flag has no effect. Therefore, if you are working with a device that
     * needs a ZLP whenever the end of the logical request falls on a packet
     * boundary, then it is sensible to set this flag on <em>every</em>
     * transfer (you do not have to worry about only setting it on transfers
     * that end on the boundary).
     *
     * This flag is currently only supported on Linux.
     * On other systems, libusb_submit_transfer() will return
     * LIBUSB_ERROR_NOT_SUPPORTED for every transfer where this flag is set.
     *
     * Available since libusb-1.0.9.
     */
    LIBUSB_TRANSFER_ADD_ZERO_PACKET = 1 << 3,
};

enum libusb_request_type {
    /** Standard */
    LIBUSB_REQUEST_TYPE_STANDARD = (0x00 << 5),

    /** Class */
    LIBUSB_REQUEST_TYPE_CLASS = (0x01 << 5),

    /** Vendor */
    LIBUSB_REQUEST_TYPE_VENDOR = (0x02 << 5),

    /** Reserved */
    LIBUSB_REQUEST_TYPE_RESERVED = (0x03 << 5)
};

enum libusb_request_recipient {
    /** Device */
    LIBUSB_RECIPIENT_DEVICE = 0x00,

    /** Interface */
    LIBUSB_RECIPIENT_INTERFACE = 0x01,

    /** Endpoint */
    LIBUSB_RECIPIENT_ENDPOINT = 0x02,

    /** Other */
    LIBUSB_RECIPIENT_OTHER = 0x03,
};

enum libusb_error {
    /** Success (no error) */
    LIBUSB_SUCCESS = 0,

    /** Input/output error */
    LIBUSB_ERROR_IO = -1,

    /** Invalid parameter */
    LIBUSB_ERROR_INVALID_PARAM = -2,

    /** Access denied (insufficient permissions) */
    LIBUSB_ERROR_ACCESS = -3,

    /** No such device (it may have been disconnected) */
    LIBUSB_ERROR_NO_DEVICE = -4,

    /** Entity not found */
    LIBUSB_ERROR_NOT_FOUND = -5,

    /** Resource busy */
    LIBUSB_ERROR_BUSY = -6,

    /** Operation timed out */
    LIBUSB_ERROR_TIMEOUT = -7,

    /** Overflow */
    LIBUSB_ERROR_OVERFLOW = -8,

    /** Pipe error */
    LIBUSB_ERROR_PIPE = -9,

    /** System call interrupted (perhaps due to signal) */
    LIBUSB_ERROR_INTERRUPTED = -10,

    /** Insufficient memory */
    LIBUSB_ERROR_NO_MEM = -11,

    /** Operation not supported or unimplemented on this platform */
    LIBUSB_ERROR_NOT_SUPPORTED = -12,

    /* NB: Remember to update LIBUSB_ERROR_COUNT below as well as the
       message strings in strerror.c when adding new error codes here. */

    /** Other error */
    LIBUSB_ERROR_OTHER = -99,
};




enum libusb_transfer_type {
    /** Control endpoint */
    LIBUSB_TRANSFER_TYPE_CONTROL = 0,

    /** Isochronous endpoint */
    LIBUSB_TRANSFER_TYPE_ISOCHRONOUS = 1,

    /** Bulk endpoint */
    LIBUSB_TRANSFER_TYPE_BULK = 2,

    /** Interrupt endpoint */
    LIBUSB_TRANSFER_TYPE_INTERRUPT = 3,

    /** Stream endpoint */
    LIBUSB_TRANSFER_TYPE_BULK_STREAM = 4,
};



enum libusb_transfer_status {
    /** Transfer completed without error. Note that this does not indicate
     * that the entire amount of requested data was transferred. */
    LIBUSB_TRANSFER_COMPLETED,

    /** Transfer failed */
    LIBUSB_TRANSFER_ERROR,

    /** Transfer timed out */
    LIBUSB_TRANSFER_TIMED_OUT,

    /** Transfer was cancelled */
    LIBUSB_TRANSFER_CANCELLED,

    /** For bulk/interrupt endpoints: halt condition detected (endpoint
     * stalled). For control endpoints: control request not supported. */
    LIBUSB_TRANSFER_STALL,

    /** Device was disconnected */
    LIBUSB_TRANSFER_NO_DEVICE,

    /** Device sent more data than requested */
    LIBUSB_TRANSFER_OVERFLOW,

    /* NB! Remember to update libusb_error_name()
       when adding new status codes here. */
};





struct usbfs_ctrltransfer {
    /* keep in sync with usbdevice_fs.h:usbdevfs_ctrltransfer */
    uint8_t  bmRequestType;
    uint8_t  bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;

    uint32_t timeout;	/* in milliseconds */

    /* pointer to data */
    void *data;
};

struct usbfs_bulktransfer {
    /* keep in sync with usbdevice_fs.h:usbdevfs_bulktransfer */
    unsigned int ep;
    unsigned int len;
    unsigned int timeout;	/* in milliseconds */

    /* pointer to data */
    void *data;
};

struct usbfs_setinterface {
    /* keep in sync with usbdevice_fs.h:usbdevfs_setinterface */
    unsigned int interface;
    unsigned int altsetting;
};


struct usbfs_getdriver {
    unsigned int interface;
    char driver[USBFS_MAXDRIVERNAME + 1];
};

struct usbfs_iso_packet_desc {
    unsigned int length;
    unsigned int actual_length;
    unsigned int status;
};


struct usbfs_urb {
    unsigned char type;
    unsigned char endpoint;
    int status;
    unsigned int flags;
    void *buffer;
    int buffer_length;
    int actual_length;
    int start_frame;
    union {
        int number_of_packets;	/* Only used for isoc urbs */
        unsigned int stream_id;	/* Only used with bulk streams */
    };
    int error_count;
    unsigned int signr;
    void *usercontext;
    struct usbfs_iso_packet_desc iso_frame_desc[0];
};

struct usbfs_connectinfo {
    unsigned int devnum;
    unsigned char slow;
};

struct usbfs_ioctl {
    int ifno;	/* interface 0..N ; negative numbers reserved */
    int ioctl_code;	/* MUST encode size + direction of data so the
             * macros in <asm/ioctl.h> give correct values */
    void *data;	/* param buffer (in, or out) */
};

struct usbfs_hub_portinfo {
    unsigned char numports;
    unsigned char port[127];	/* port to device num mapping */
};



struct usbfs_disconnect_claim {
    unsigned int interface;
    unsigned int flags;
    char driver[USBFS_MAXDRIVERNAME + 1];
};

struct usbfs_streams {
    unsigned int num_streams; /* Not used by USBDEVFS_FREE_STREAMS */
    unsigned int num_eps;
    unsigned char eps[0];
};





struct libusb_transfer {
    uint8_t flags;
    unsigned char endpoint;
    unsigned char type;
    unsigned int timeout;
    enum libusb_transfer_status status;
    int length;
    int actual_length;
    libusb_transfer_cb_fn callback;
    void *user_data;
    unsigned char *buffer;
    int num_iso_packets;
};


struct libusb_control_setup {
    uint8_t  bmRequestType;
    uint8_t  bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
};

















static inline uint16_t libusb_cpu_to_le16(const uint16_t x)
{
    union {
        uint8_t  b8[2];
        uint16_t b16;
    } _tmp;
    _tmp.b8[1] = (uint8_t) (x >> 8);
    _tmp.b8[0] = (uint8_t) (x & 0xff);
    return _tmp.b16;
}



















class   husb
{
public:
    husb();

    void restart_stream();
    bool is_running();

    bool init();
    void close();
    int   get_urb_state();

    void join();

    int submit_transfer(libusb_transfer* tr);
    int stream_start();

    void wait_all_transfer_finish(libusb_transfer_cb_fn cb);


    int set_urb_buf();

    int get_queue_urb(libusb_transfer_cb_fn cb);



    int bulk_transfer_async(unsigned char endpoint, unsigned char *data, int len, int *transferred, unsigned int timeout,libusb_transfer_cb_fn cb);
    int bulk_transfer(unsigned char endpoint,unsigned char* data,int len,int *transferred,unsigned int timeout);
    int control_transfer(uint8_t bmRequestType,uint8_t br,u_int16_t wValue,uint16_t wIndex,unsigned char* data,uint16_t wlen,unsigned int timeout);

    int claim_interface(int number);


     void                fill_bulk_transfer(libusb_transfer* transfer,unsigned char endpoint,unsigned char* buf,int len,libusb_transfer_cb_fn cb,void* userdata,unsigned int timeout);

    int release_interface(int   iface);

     static void    read_cb(libusb_transfer*    transfer);

    void                handle_events();


    static  void    transfer_callback(libusb_transfer*  transfer);


private:
    libusb_transfer*    alloc_transfer(int iso);
    void                free_transfer(libusb_transfer*  tf);


    void                fill_control_setup(unsigned char* buffer,uint8_t bmr,uint8_t br,uint16_t wvalue,uint16_t windex,uint16_t wlen);
    void                fill_control_transfer(libusb_transfer*  transfer,unsigned char* buffer,libusb_transfer_cb_fn cb,void* user_data,unsigned int timeout);





    int                 m_fd;

    usbfs_urb*    m_urbs[128];
public:
    pollfd             m_pfd;

    int                 reap_for_handle();
    int                 reap_for_handle_async(libusb_transfer_cb_fn cb);

    pthread_cond_t  m_sync;
    pthread_t           m_thread;
    bool                    exit;
};


inline  void husb::fill_control_setup(unsigned char *buffer, uint8_t bmr, uint8_t br, uint16_t wvalue, uint16_t windex, uint16_t wlen)
{
    libusb_control_setup*   setup = (libusb_control_setup*)(void*)buffer;
    setup->bmRequestType = bmr;
    setup->bRequest      = br;
    setup->wValue        = libusb_cpu_to_le16(wvalue);
    setup->wIndex        = libusb_cpu_to_le16(windex);
    setup->wLength       = libusb_cpu_to_le16(wlen);
}

inline  void husb::fill_control_transfer(libusb_transfer *transfer, unsigned char *buffer, libusb_transfer_cb_fn cb, void *user_data, unsigned int timeout)
{
    struct  libusb_control_setup*   setup = (libusb_control_setup*)(void*)buffer;
    transfer->endpoint = 0;
    transfer->type     = LIBUSB_TRANSFER_TYPE_CONTROL;
    transfer->timeout  = timeout;
    transfer->buffer   = buffer;
    if(setup)
        transfer->length = (int)(LIBUSB_CONTROL_SETUP_SIZE + libusb_le16_to_cpu(setup->wLength));

    transfer->user_data = user_data;
    transfer->callback  = cb;

}

inline  void husb::fill_bulk_transfer(libusb_transfer *transfer, unsigned char endpoint, unsigned char *buf, int len, libusb_transfer_cb_fn cb, void *userdata, unsigned int timeout)
{
    transfer->endpoint = endpoint;
    transfer->type     = LIBUSB_TRANSFER_TYPE_BULK;
    transfer->timeout  = timeout;
    transfer->buffer   = buf;
    transfer->length   = len;
    transfer->user_data= userdata;
    transfer->callback = cb;
}









#endif // HUSB_H
