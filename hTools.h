#ifndef HTOOLS_H
#define HTOOLS_H
#include <unistd.h>
#include <sys/time.h>

#ifndef LOG_TAG

#define LOG_TAG "HOSLESS_LIB"
#define LOGE(...)  printf(__VA_ARGS__);fflush(stdout);
#endif



typedef unsigned long long i64;
typedef unsigned char       uchar;

static  inline  void    do_gettime(i64* sec,i64* frac)
{
    struct  timespec    ts;
    struct  timeval     tv;
    gettimeofday(&tv,0);
    ts.tv_sec = tv.tv_sec;
    ts.tv_nsec= tv.tv_usec * 1000;
    *sec = ts.tv_sec;
    *frac= ts.tv_nsec;
}
static inline i64   fractionAdjustment()
{
    return  1000 * 1000ull;
}


#define H_UINT64_C(c)   static_cast<unsigned long long>(c ## ULL)

static  i64 elapsedAndRestart(i64 sec,i64 frac,
                              i64* nowsec,i64* nowfrac)
{
    do_gettime(nowsec,nowfrac);
    sec = *nowsec - sec;
    frac= *nowfrac- frac;
    return  sec *  H_UINT64_C(1000) + frac / fractionAdjustment();
}

class hElapsedTimer
{
public:

    void    start(){
        do_gettime(&m_t1,&m_t2);
    }

    void    restart(){
         elapsedAndRestart(m_t1,m_t2,&m_t1,&m_t2);
    }

    i64     elapsed() const{
        i64 sec,frac;
        return  elapsedAndRestart(m_t1,m_t2,&sec,&frac);
    }
private:
    i64 m_t1;
    i64 m_t2;
};

class   hElapsedTimer2
{
public:
    void    start(){
        gettimeofday(&_tm1,NULL);
    }

    int    elapsed() {
        gettimeofday(&_tm2,NULL);
        return  ((_tm2.tv_sec - _tm1.tv_sec) * 1000000 + (_tm2.tv_usec - _tm1.tv_usec)) / 1000;
    }

    void    restart(){
        start();
        _tm2.tv_sec = _tm2.tv_usec = 0;
    }
    timeval _tm1;
    timeval _tm2;
};




#endif // HTOOLS_H
