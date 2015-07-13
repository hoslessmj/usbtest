#ifndef HCAMER_H
#define HCAMER_H
#include <math.h>
#include <QImage>


#include <fstream>
using   namespace   std;





#define get_y(_x,_y) m_y[_y * 352 + _x]
#define get_u(_x,_y) m_u[(_y>>1) * 176  +  (_x>>1)]
#define get_v(_x,_y) m_v[(_y>>1) * 176  +  (_x>>1)]


//class hFishEyeFix;
//class hBrTable;
class hCamerParemerProvide;

const int g_table_v1[256]={ -180, -179, -177, -176, -174, -173, -172, -170, -169, -167, -166, -165, -163, -162, -160, -159, -158, -156, -155, -153, -152, -151, -149, -148, -146, -145, -144, -142, -141, -139, -138, -137, -135, -134, -132, -131, -130, -128, -127, -125, -124, -123, -121, -120, -118, -117, -115, -114, -113, -111, -110, -108, -107, -106, -104, -103, -101, -100, -99, -97, -96, -94, -93, -92, -90, -89, -87, -86, -85, -83, -82, -80, -79, -78, -76, -75, -73, -72, -71, -69, -68, -66, -65, -64, -62, -61, -59, -58, -57, -55, -54, -52, -51, -50, -48, -47, -45, -44, -43, -41, -40, -38, -37, -36, -34, -33, -31, -30, -29, -27, -26, -24, -23, -22, -20, -19, -17, -16, -15, -13, -12, -10, -9, -8, -6, -5, -3, -2, 0, 1, 2, 4, 5, 7, 8, 9, 11, 12, 14, 15, 16, 18, 19, 21, 22, 23, 25, 26, 28, 29, 30, 32, 33, 35, 36, 37, 39, 40, 42, 43, 44, 46, 47, 49, 50, 51, 53, 54, 56, 57, 58, 60, 61, 63, 64, 65, 67, 68, 70, 71, 72, 74, 75, 77, 78, 79, 81, 82, 84, 85, 86, 88, 89, 91, 92, 93, 95, 96, 98, 99, 100, 102, 103, 105, 106, 107, 109, 110, 112, 113, 114, 116, 117, 119, 120, 122, 123, 124, 126, 127, 129, 130, 131, 133, 134, 136, 137, 138, 140, 141, 143, 144, 145, 147, 148, 150, 151, 152, 154, 155, 157, 158, 159, 161, 162, 164, 165, 166, 168, 169, 171, 172, 173, 175, 176, 178 };
const int g_table_v2[256]={ -92, -91, -91, -90, -89, -88, -88, -87, -86, -86, -85, -84, -83, -83, -82, -81, -81, -80, -79, -78, -78, -77, -76, -76, -75, -74, -73, -73, -72, -71, -71, -70, -69, -68, -68, -67, -66, -66, -65, -64, -63, -63, -62, -61, -61, -60, -59, -58, -58, -57, -56, -56, -55, -54, -53, -53, -52, -51, -51, -50, -49, -48, -48, -47, -46, -46, -45, -44, -43, -43, -42, -41, -41, -40, -39, -38, -38, -37, -36, -36, -35, -34, -33, -33, -32, -31, -31, -30, -29, -28, -28, -27, -26, -26, -25, -24, -23, -23, -22, -21, -21, -20, -19, -18, -18, -17, -16, -16, -15, -14, -13, -13, -12, -11, -11, -10, -9, -8, -8, -7, -6, -6, -5, -4, -3, -3, -2, -1, 0, 0, 1, 2, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9, 10, 10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 17, 18, 19, 20, 20, 21, 22, 22, 23, 24, 25, 25, 26, 27, 27, 28, 29, 30, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 37, 38, 39, 40, 40, 41, 42, 42, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50, 50, 51, 52, 52, 53, 54, 55, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 62, 63, 64, 65, 65, 66, 67, 67, 68, 69, 70, 70, 71, 72, 72, 73, 74, 75, 75, 76, 77, 77, 78, 79, 80, 80, 81, 82, 82, 83, 84, 85, 85, 86, 87, 87, 88, 89, 90, 90 };
const int g_table_u1[256]={ -44, -44, -44, -43, -43, -43, -42, -42, -42, -41, -41, -41, -40, -40, -40, -39, -39, -39, -38, -38, -38, -37, -37, -37, -36, -36, -36, -35, -35, -35, -34, -34, -33, -33, -33, -32, -32, -32, -31, -31, -31, -30, -30, -30, -29, -29, -29, -28, -28, -28, -27, -27, -27, -26, -26, -26, -25, -25, -25, -24, -24, -24, -23, -23, -22, -22, -22, -21, -21, -21, -20, -20, -20, -19, -19, -19, -18, -18, -18, -17, -17, -17, -16, -16, -16, -15, -15, -15, -14, -14, -14, -13, -13, -13, -12, -12, -11, -11, -11, -10, -10, -10, -9, -9, -9, -8, -8, -8, -7, -7, -7, -6, -6, -6, -5, -5, -5, -4, -4, -4, -3, -3, -3, -2, -2, -2, -1, -1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 31, 32, 32, 33, 33, 33, 34, 34, 34, 35, 35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 41, 42, 42, 42, 43, 43 };
const int g_table_u2[256]={ -227, -226, -224, -222, -220, -219, -217, -215, -213, -212, -210, -208, -206, -204, -203, -201, -199, -197, -196, -194, -192, -190, -188, -187, -185, -183, -181, -180, -178, -176, -174, -173, -171, -169, -167, -165, -164, -162, -160, -158, -157, -155, -153, -151, -149, -148, -146, -144, -142, -141, -139, -137, -135, -134, -132, -130, -128, -126, -125, -123, -121, -119, -118, -116, -114, -112, -110, -109, -107, -105, -103, -102, -100, -98, -96, -94, -93, -91, -89, -87, -86, -84, -82, -80, -79, -77, -75, -73, -71, -70, -68, -66, -64, -63, -61, -59, -57, -55, -54, -52, -50, -48, -47, -45, -43, -41, -40, -38, -36, -34, -32, -31, -29, -27, -25, -24, -22, -20, -18, -16, -15, -13, -11, -9, -8, -6, -4, -2, 0, 1, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 21, 23, 24, 26, 28, 30, 31, 33, 35, 37, 39, 40, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 60, 62, 63, 65, 67, 69, 70, 72, 74, 76, 78, 79, 81, 83, 85, 86, 88, 90, 92, 93, 95, 97, 99, 101, 102, 104, 106, 108, 109, 111, 113, 115, 117, 118, 120, 122, 124, 125, 127, 129, 131, 133, 134, 136, 138, 140, 141, 143, 145, 147, 148, 150, 152, 154, 156, 157, 159, 161, 163, 164, 166, 168, 170, 172, 173, 175, 177, 179, 180, 182, 184, 186, 187, 189, 191, 193, 195, 196, 198, 200, 202, 203, 205, 207, 209, 211, 212, 214, 216, 218, 219, 221, 223, 225 };



struct  point{
    int x;
    int y;
    point(){x=0;y=0;}
    point(int u,int v){
        x = u;
        y = v;
    }
};








class hCamer
{
public:
    enum CH{
        FRONT = 0,
        BACK  = 1,
        LEFT  = 2,
        RIGHT = 3,
        ALL   = 10,
    };
public:
    hCamer(CH   ch){
        m_ch = ch;
        m_isclip = false;
        m_fg_ep.x = m_fg_ep.y = 0;
        m_fg_sp = m_fg_sp1 = m_fg_ep1 = m_fg_ep;
    }

    virtual void    test_task(){
        un_init();
    }

    virtual void    toRgb(unsigned char*    dst){
        int width = 352;
        int height = 288;
        int R,G,B,Y,U,V,A;
        int x,y;
        unsigned char*  rgb24  = dst;
        for (y=0;y<height;y++)
        {
         for (x=0;x<width;x++)
         {
          Y = get_y(x,y);
          U = get_u(x,y);
          V = get_v(x,y);
          R = 1.164 * (Y-16) + 1.596*(U - 128);
          G = 1.164 * (Y-16) - 0.813*(U - 128) - 0.392 * (V - 128);
          B = 1.164 * (Y-16) + 2.017*(V - 128);
          A = 0;
          //防止越界
          if (R>255)R=255;
          if (R<0)R=0;
          if (G>255)G=255;
          if (G<0)G=0;
          if (B>255)B=255;
          if (B<0)B=0;
          *(rgb24 + (y * width * 4) + x * 4+3) = 1;
          *(rgb24 + (y * width * 4) + x * 4) = B;
          *(rgb24 + (y * width * 4) + x * 4+1) = G;
          *(rgb24 + (y * width * 4) + x * 4+2) = R;
         }
        }
    }


    virtual void    toRgb(){
//        hElapsedTimer   tm;
//        tm.start();
        int width = 352;
        int height = 288;

        int R,G,B,Y,U,V,A;
        int x,y;
        unsigned char*  rgb24  = m_rgb;
        for (y=0;y<height;y++)
        {
         for (x=0;x<width;x++)
         {
          Y = get_y(x,y);
          U = get_u(x,y);
          V = get_v(x,y);
          R = 1.164 * (Y-16) + 1.596*(U - 128);
          G = 1.164 * (Y-16) - 0.813*(U - 128) - 0.392 * (V - 128);
          B = 1.164 * (Y-16) + 2.017*(V - 128);
          A = 0;
          //防止越界
          if (R>255)R=255;
          if (R<0)R=0;
          if (G>255)G=255;
          if (G<0)G=0;
          if (B>255)B=255;
          if (B<0)B=0;
          *(rgb24 + (y * width * 4) + x * 4+3) = 1;
          *(rgb24 + (y * width * 4) + x * 4) = R;
          *(rgb24 + (y * width * 4) + x * 4+1) = G;
          *(rgb24 + (y * width * 4) + x * 4+2) = B;
         }
        }
    }


    virtual void    create_helper() = 0;
    virtual void    un_init() = 0;
    virtual void    finish_image() = 0;

    virtual void    load(const char*    name);
    virtual void    save();

public:
    virtual void    init();

public:
    unsigned char       *m_y,*m_u,*m_v;
    CH                  m_ch;

    hCamerParemerProvide*   m_helper;
    unsigned char*          m_rgb;
    unsigned char*          m_rgb_save;
    unsigned char*          m_rgb_rotate;
    QImage                  m_img;
    QImage                  m_dst_img;
    QImage                  m_init_img;
    bool                    m_isclip;
    point                   m_fg_sp1,m_fg_ep1,m_fg_sp,m_fg_ep;

    point                   m_clip_left,m_clip_top,m_clip_right,m_clip_bottom;

    int                     m_base_x;


//    pinjie_line*                  pj_first;
//    pinjie_line*                  pj_second;
    point                   m_pp;

};


class hAllCamer  : public hCamer
{
public:
    hAllCamer():hCamer(hCamer::ALL){this->init();}

    virtual void    create_helper(){
   //     m_helper = new hCamerParemerProvide(m_ch);
    }
    virtual void    un_init(){}
    virtual void    finish_image(){}

    virtual void    init();
};

class hLeftCamer : public hCamer
{
public:
    hLeftCamer():hCamer(hCamer::LEFT){}
    virtual void    create_helper();
    virtual void    un_init();
    virtual void    finish_image();

    virtual void    init();
};
class hRightCamer :public hCamer
{
public:
    hRightCamer():hCamer(hCamer::RIGHT){}
    virtual void    create_helper();
    virtual void    un_init();
    virtual void    finish_image();

    virtual void    init();
};

class hBackCamer : public hCamer
{
public:
    hBackCamer():hCamer(hCamer::BACK){}
    virtual void    create_helper();
    virtual void    un_init();
    virtual void    finish_image();

    virtual void    init();
};

class hFrontCamer : public hCamer
{
public:
    hFrontCamer():hCamer(hCamer::FRONT){}
    virtual void    create_helper();
    virtual void    un_init();
    virtual void    finish_image();

    virtual void    init();
};

class hCamerParemerProvide
{
public:
    hCamerParemerProvide(hCamer* cm,float rx = 0,float ry = 0,float rz = 0,float xo = 0,float yo = 0,
                                          float  a = 0,float b  = 0,float y  = 0,float h  = 0,float m  = 0,float n = 0,float bxo=176,float byo=288){
        m_ch = cm->m_ch;
        m_cm = cm;
        m_rx = rx;  m_ry = ry;  m_z = rz;   m_xo = xo;  m_yo = yo;
        m_a  = a;   m_b  = b ;  m_y = y ;   m_h  = h ;  m_m  = m;   m_n = n;
        m_box = bxo; m_boy = byo;
        m_width = 352;m_height = 288;

        switch(m_ch){
        case hCamer::LEFT:
        case hCamer::RIGHT:
            m_height = 352;
            m_width  = 288;
            break;
        }
    }

    virtual void    reset(float rx = 0,float ry = 0,float rz = 0,float xo = 0,float yo = 0,
                          float  a = 0,float b  = 0,float y  = 0,float h  = 0,float m  = 0,float n = 0,float bxo = 176,float byo = 288)
    {
        m_rx = rx;  m_ry = ry;  m_z = rz;   m_xo = xo;  m_yo = yo;
        m_a  = a;   m_b  = b ;  m_y = y ;   m_h  = h ;  m_m  = m;   m_n = n;
        m_box = bxo; m_boy = byo;
        m_width = 352;m_height = 288;
        init();
    }


    virtual void    init(){
        uninit_gen();
        britable_gen();
    }

    virtual void    uninit();

    void    uninit_gen(){
        int z = m_z;
        int r = m_rx;
        int ry= m_ry;
        int xo= m_xo;
        int yo= m_yo;
        int height = 288;
        int width  = 352;
        switch(m_ch){
        case hCamer::LEFT:
        case hCamer::RIGHT:
            height = 352;
            width  = 288;
            break;
        }


        for(int v=0;v < height;++v){
            for(int u=0;u<width;++u){
                int x = xo + r *  (u-xo) / sqrt(z*z + ((u-xo)*(u-xo)) + ((v-yo) * (v-yo)));
                int y = yo + ry * (v-yo) / sqrt(z*z + ((u-xo)*(u-xo)) + ((v-yo) * (v-yo)));
                if(x<0 || y<0 || x>=width || y>=height){
                    m_uninit_tables[v][u].x = m_uninit_tables[v][u].y = 0;
                    continue;
                }
                m_uninit_tables[v][u].x = x;
                m_uninit_tables[v][u].y = y;
            }
        }
    }


    void    britable_gen(){
        int H = 288;
        int W = 352;
        float   b = m_b * 3.14 / 180;
        float   a = m_a * 3.14 / 180;
        float   y = m_y * 3.14 / 180;
        float   h = m_h ;
        float   m = m_m;
        float   n = m_n;
        float   N = W;
        float   M = H;
        float   mb = m / M;
        float   nb = n / N;

        for(int i =0; i < H / 2;++i){
            for(int j =0; j <W;++j){
                float   px = j * nb - (n / 2);
                float   py = (m / 2) - i * mb + (h / tan(y));

                float   wx = (px * W) / (2 * tan(b) * sqrt(h * h + py * py));
                float   wy = (py * H - H * h * tan(y)) / (2 * py * tan(y) * tan(a) + 2 * h * tan(a));

                int     pi = m_boy  - wy;
                int     pj = m_box  + wx;



                if(pi <0 || pj <0 || pi>=H || pj >=W){
                    m_bri_tables[i][j].x = m_bri_tables[i][j].y = 0;
                    continue;
                }else{
                    m_bri_tables[i][j].x = pj;
                    m_bri_tables[i][j].y = pi;
                    continue;
                }
            }
        }
    }


    point               m_uninit_tables[352][352];
    point               m_bri_tables[352][352];
    point               m_rotate_tables[352][352];

    point               m_tables[500][500];


    hCamer::CH          m_ch;
    hCamer*             m_cm;
    //uninit par
    float   m_rx,m_ry,m_z,m_xo,m_yo;

    //britable par
    float   m_a,m_b,m_y,m_h,m_m,m_n,m_box,m_boy;


    int                  m_width;
    int                  m_height;
};



class hLeftParemerProvide : public hCamerParemerProvide
{
public:
    hLeftParemerProvide(hCamer* cm,float rx = 0,float ry = 0,float rz = 0,float xo = 0,float yo = 0,
                                          float  a = 0,float b  = 0,float y  = 0,float h  = 0,float m  = 0,float n = 0):
        hCamerParemerProvide(cm,rx,ry,rz,xo,yo,a,b,y,h,m,n){}

    virtual void uninit_for_image();
    virtual void uninit();
    virtual void init();
};
class hRightParemerProvide : public hCamerParemerProvide
{
public:
    hRightParemerProvide(hCamer* cm,float rx = 0,float ry = 0,float rz = 0,float xo = 0,float yo = 0,
                                          float  a = 0,float b  = 0,float y  = 0,float h  = 0,float m  = 0,float n = 0):
        hCamerParemerProvide(cm,rx,ry,rz,xo,yo,a,b,y,h,m,n){}

    virtual void uninit_for_image();
    virtual void uninit();
    virtual void init();
};

class hFrontParemerProvide : public hCamerParemerProvide
{
public:
    hFrontParemerProvide(hCamer* cm,float rx = 0,float ry = 0,float rz = 0,float xo = 0,float yo = 0,
                                          float  a = 0,float b  = 0,float y  = 0,float h  = 0,float m  = 0,float n = 0):
        hCamerParemerProvide(cm,rx,ry,rz,xo,yo,a,b,y,h,m,n){}
    virtual void uninit_for_image();
    virtual void uninit();
    virtual void init();
};

class hBackParemerProvide   : public hCamerParemerProvide
{
public:
    hBackParemerProvide(hCamer* cm,float rx = 0,float ry = 0,float rz = 0,float xo = 0,float yo = 0,
                        float  a = 0,float b  = 0,float y  = 0,float h  = 0,float m  = 0,float n = 0):
        hCamerParemerProvide(cm,rx,ry,rz,xo,yo,a,b,y,h,m,n){}

    virtual void uninit_for_image();
    virtual void uninit();
    virtual void init();
};

class hStitchingLine
{
public:

    struct  stitchingline_data{
        point   src;
        point   dst;
    };

    hStitchingLine(hCamer*  s,hCamer* d,int stitchingLineCount = 10);



    void        draw();



    hCamer*     src,*dst;
    int         line_count;
    int         line_size;
    int         all_size;
    stitchingline_data*     data;
};
class   hCamerarchive
{
public:
    static void    save(const char*    filename,hCamer*    p);
    static void    load(const char*    filename,hCamer*    p);


};


#endif // HCAMER_H
