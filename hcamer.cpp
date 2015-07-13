#include "hcamer.h"
#include <stdio.h>


#define LOGE  printf

void hCamer::load(const char *name)
{
    m_img = QImage(name);
}

void hCamer::save()
{
    static  int _count = 0;
    char    filename[256];
    sprintf(filename,"/sdcard/ch_%d-%d.img",(int)m_ch,_count++);
    LOGE("filename = %s\n",filename);
    FILE*   fl = fopen(filename,"w+");
    if(fl == NULL){
        LOGE("fopen error\n");
        return;
    }
    toRgb(m_rgb_save);
    fwrite(m_rgb_save,1,352 * 288 *4,fl);
    fclose(fl);
}
void hBackCamer::init()
{
    hCamer::init();
//   m_dst_img = QImage(352,288,QImage::Format_RGB32);
    m_img = QImage(m_rgb,352,288,QImage::Format_RGB32);
    m_clip_left.x = m_clip_top.y = 0;
    m_clip_right.x = 352;
    m_clip_bottom.y= 105;
    m_dst_img = QImage(m_clip_right.x - m_clip_left.x,m_clip_bottom.y - m_clip_top.y,QImage::Format_RGB32);
}

void hFrontCamer::init()
{
    hCamer::init();
    m_img = QImage(m_rgb,352,288,QImage::Format_RGB32);
    m_clip_left.x = m_clip_top.y = 0;
    m_clip_right.x = 350;m_clip_bottom.y = 84;
    m_dst_img = QImage(m_clip_right.x - m_clip_left.x,m_clip_bottom.y - m_clip_top.y,QImage::Format_RGB32);
}

void hRightCamer::init()
{
    m_y = new unsigned char[352 * 288];
    m_u = new unsigned char[176 * 144];
    m_v = new unsigned char[176 * 144];
    m_rgb = new unsigned char[352 * 288 * 4];
    m_rgb_rotate = new unsigned char[288 * 352 * 4];
//    m_dst_img = QImage(288,352,QImage::Format_RGB32);
    m_img = QImage(m_rgb,352,288,QImage::Format_RGB32);
    m_init_img = QImage(352,288,QImage::Format_RGB32);
    m_clip_left.x =113; m_clip_top.y = 0;
    m_clip_right.x=230;
    m_clip_bottom.y=352;
    m_dst_img = QImage(m_clip_right.x - m_clip_left.x,m_clip_bottom.y - m_clip_top.y,QImage::Format_RGB32);
}

void hLeftCamer::init()
{
    m_y = new unsigned char[352 * 288];
    m_u = new unsigned char[176 * 144];
    m_v = new unsigned char[176 * 144];
    m_rgb = new unsigned char[352 * 288 * 4];
    m_rgb_rotate = new unsigned char[288 * 352 * 4];
    m_img = QImage(m_rgb,352,288,QImage::Format_RGB32);
 //   m_dst_img = QImage(288,352,QImage::Format_RGB32);
    m_init_img= QImage(352,288,QImage::Format_RGB32);
//    m_fg_sp.x = 58;m_fg_sp.y = 0;
//    m_fg_ep.x =175;m_fg_ep.y = 53;
    m_clip_left.x =48; m_clip_top.y = 0;
    m_clip_right.x=170;m_clip_bottom.y=352;
    m_dst_img = QImage(m_clip_right.x - m_clip_left.x,m_clip_bottom.y - m_clip_top.y,QImage::Format_RGB32);
}

void hAllCamer::init()
{
    m_y = new unsigned char[704 * 576];
    m_u = new unsigned char[352 * 288];
    m_v = new unsigned char[352 * 288];
    m_rgb = new unsigned char[704 * 576 * 4];
 //   m_base_img = QImage(m_rgb,704,576,QImage::Format_RGB32);
    m_helper = 0;
}

void hCamer::init()
{
    m_y = new unsigned char[352 * 288];
    m_u = new unsigned char[176 * 144];
    m_v = new unsigned char[176 * 144];
    m_rgb = new unsigned char[352 * 288 * 4]; //rgb 32 0x00FFFFFF
    m_rgb_save = new unsigned char[352 * 288 * 4];
    m_rgb_rotate = 0;
//    m_base_img = QImage(m_rgb,352,288,QImage::Format_RGB32);
    m_helper = 0;
    m_init_img = QImage(352,288,QImage::Format_RGB32);
}


void hRightCamer::create_helper()
{
//    m_helper = new hRightParemerProvide(this,160,166,60,121,188);
//    m_helper = new hRightParemerProvide(this,176,156,70,176,144,26,45,45,1,8,4);
//    m_helper = new hRightParemerProvide(this,158,166,75,175,127);
    m_helper = new hRightParemerProvide(this,163,166,70,175,125);
    m_helper->init();

}
void hRightCamer::un_init()
{
    m_helper->uninit();
}
void hRightCamer::finish_image()
{
}


void hBackCamer::create_helper()
{
    //m_helper = new hBackParemerProvide(this,176,144,80,176,165,41,51,45,0.35,12,5);
    //m_helper = new hBackParemerProvide(this,151,156,60,176,144,39,45,45,0.5,12,4);
    //m_helper = new hBackParemerProvide(this,166,125,100,176,144,37,45,45,0.6,7,5);
    //m_helper = new hBackParemerProvide(this,156,176,176,170,144,37,45,45,0.5,7,5);
    m_helper = new hBackParemerProvide(this,156,156,70,176,144,40,49,45,0.5,10,4);
    m_helper->init();
}
void hBackCamer::un_init()
{
    m_helper->uninit();
}
void hBackCamer::finish_image()
{

}

void hLeftCamer::create_helper()
{
//    m_helper = new hLeftParemerProvide(this,160,162,65,166,186);
//    m_helper = new hLeftParemerProvide(this,159,156,75,190,125);
//    m_helper = new hLeftParemerProvide(this,160,156,60,176,125,25,45,45,0.9,6,6);
    m_helper = new hLeftParemerProvide(this,163,161,70,175,125);
    m_helper->init();

}
void hLeftCamer::un_init()
{
    m_helper->uninit();
}
void hLeftCamer::finish_image()
{

}

void hFrontCamer::create_helper()
{
//    m_helper = new hFrontParemerProvide(this,161,116,100,176,144,31,45,45,0.5,10,6);
//    m_helper = new hFrontParemerProvide(this,156,136,100,171,144,32,45,45,0.5,10,5);
    m_helper = new hFrontParemerProvide(this,156,136,100,171,144,32,45,45,0.5,10,5);
    m_helper->init();
}
void hFrontCamer::un_init()
{
    m_helper->uninit();
}
void hFrontCamer::finish_image()
{

}

#define sget_y(_x,_y) y[_y * 352 + _x]
#define sget_u(_x,_y) u[(_y>>1) * 176  +  (_x>>1)]
#define sget_v(_x,_y) v[(_y>>1) * 176  +  (_x>>1)]

#define dget_y(_x,_y) m_uy[_y * 352 + _x]
#define dget_u(_x,_y) m_uu[(_y>>1) * 176 + (_x>>1)]
#define dget_v(_x,_y) m_uv[(_y>>1) * 176 + (_x>>1)]



void hRightParemerProvide::uninit()
{
    uninit_for_image();
    return;
    unsigned    char    *y,*u,*v;
    y = m_cm->m_y;
    u = m_cm->m_u;
    v = m_cm->m_v;
//    hElapsedTimer2  tm;
//    tm.start();
    int R,G,B,Y,U,V,A;
    unsigned    char*   rgb24 = m_cm->m_rgb;
    int nx,ny;
    for(int sy = 0;sy < 352;++sy){
        for(int sx = 0;sx < 288;++sx){
            nx = m_tables[sy][sx].x;
            ny = m_tables[sy][sx].y;
            Y = sget_y(nx,ny);
            U = sget_u(nx,ny);
            V = sget_v(nx,ny);
//            R = 1.164 * (Y-16) + 1.596*(U - 128);
//            G = 1.164 * (Y-16) - 0.813*(U - 128) - 0.392 * (V - 128);
//            B = 1.164 * (Y-16) + 2.017*(V - 128);

            R = Y + g_table_v1[U];
            G = Y - g_table_u1[U] + g_table_v2[V];
            B = Y + g_table_u2[V];
            A = 0;
            if (R>255)R=255;
            if (R<0)R=0;
            if (G>255)G=255;
            if (G<0)G=0;
            if (B>255)B=255;
            if (B<0)B=0;
            *(rgb24 + (sy * 288 * 4) + sx * 4+3) = 1;
            *(rgb24 + (sy * 288 * 4) + sx * 4)   = B;
            *(rgb24 + (sy * 288 * 4) + sx * 4+1) = G;
            *(rgb24 + (sy * 288 * 4) + sx * 4+2) = R;
        }
    }
}


void hFrontParemerProvide::uninit()
{
    uninit_for_image();
    return;
    unsigned    char    *y,*u,*v;
    y = m_cm->m_y;
    u = m_cm->m_u;
    v = m_cm->m_v;
//    hElapsedTimer2  tm;
//    tm.start();
    int R,G,B,Y,U,V,A;
    unsigned    char*   rgb24 = m_cm->m_rgb;
    int nx,ny;
    for(int sy = 0;sy < 288 - 88;++sy){
        for(int sx = 0;sx < 352;++sx){
            nx = m_tables[sy][sx].x;
            ny = m_tables[sy][sx].y;
            Y = sget_y(nx,ny);
            U = sget_u(nx,ny);
            V = sget_v(nx,ny);
//            R = 1.164 * (Y-16) + 1.596*(U - 128);
//            G = 1.164 * (Y-16) - 0.813*(U - 128) - 0.392 * (V - 128);
//            B = 1.164 * (Y-16) + 2.017*(V - 128);

            R = Y + g_table_v1[U];
            G = Y - g_table_u1[U] + g_table_v2[V];
            B = Y + g_table_u2[V];
            A = 0;
            if (R>255)R=255;
            if (R<0)R=0;
            if (G>255)G=255;
            if (G<0)G=0;
            if (B>255)B=255;
            if (B<0)B=0;
            *(rgb24 + (sy * 288 * 4) + sx * 4+3) = 1;
            *(rgb24 + (sy * 288 * 4) + sx * 4)   = B;
            *(rgb24 + (sy * 288 * 4) + sx * 4+1) = G;
            *(rgb24 + (sy * 288 * 4) + sx * 4+2) = R;
        }
    }
}
void hLeftParemerProvide::uninit_for_image()
{
    m_cm->m_img.setPixel(0,0,0);
//    m_cm->m_dst_img.fill(Qt::black);
    point p1,p2,p3;
    point p4,p5,p6;
    int H,W;
    if(m_cm->m_isclip){
        p1 = m_cm->m_fg_sp;
        p2 = m_cm->m_fg_ep;
        p4 = m_cm->m_fg_sp1;
        p5 = m_cm->m_fg_ep1;
        int H = m_cm->m_dst_img.height();
        int W = m_cm->m_dst_img.width();
        for(int y=0;y < H;++y){
            for(int x = 0;x < W;++x){
//                p3.x = x;p3.y = y;
//                p6.x = x;p6.y = y;
//                float S = (p1.x - p3.x) * (p2.y - p3.y) - (p1.y - p3.y) * (p2.x - p3.x);
//                if(S <0.0f){
//                    continue;
//                }
//                S = (p4.x - p6.x) * (p5.y - p6.y) - (p4.y - p6.y) * (p5.x - p6.x);
//                if(S > 0.0f)
//                    continue;
//                if(m_tables[y][x+m_cm->m_clip_left.x].x == 0 && m_tables[y][x+m_cm->m_clip_left.x].y == 0){
//                    continue;
//                }
                m_cm->m_dst_img.setPixel(x,y,m_cm->m_img.pixel(m_tables[y][x+m_cm->m_clip_left.x].x,m_tables[y][x+m_cm->m_clip_left.x].y));
//                int nx = m_tables[y][x].x;
//                int ny = m_tables[y][x].y;
//                uchar r1,r2,r3,r4,g1,g2,g3,g4,b1,b2,b3,b4;
//                QRgb    p1,p2,p3,p4,p5;
//                p1 = m_cm->m_img.pixel(nx-1,ny);
//                p2 = m_cm->m_img.pixel(nx+1,ny);
//                p3 = m_cm->m_img.pixel(nx-1,ny-1);
//                p4 = m_cm->m_img.pixel(nx+1,ny+1);


//                r1 = qRed(p1);
//                r2 = qRed(p2);
//                r3 = qRed(p3);
//                r4 = qRed(p4);
//                g1 = qGreen(p1);g2 = qGreen(p2);g3 = qGreen(p3);g4 = qGreen(p4);
//                b1 = qBlue(p1);b2 = qBlue(p2);b3 = qBlue(p3);b4 = qBlue(p4);

//                uchar   r5,g5,b5;
//                r5 = r1 * 0.25 + r2 * 0.25 + r3 * 0.25 + r4 * 0.25;
//                g5 = g1 * 0.25 + g2 * 0.25 + g3 * 0.25 + g4 * 0.25;
//                b5 = b1 * 0.25 + b2 * 0.25 + b3 * 0.25 + b4 * 0.25;

//                r5 = ((r5 > 255)?255 : r5);
//                g5 = ((g5 > 255)?255 : g5);
//                b5 = ((b5 > 255)?255 : b5);
//                p4 = qRgb(r5,g5,b5);


//                m_cm->m_dst_img.setPixel(x,y,p4);
            }
        }
    }else{
        H = m_cm->m_dst_img.height();
        W = m_cm->m_dst_img.width();
        for(int y=0;y < H;++y){
            for(int x=0;x < W;++x){
                m_cm->m_dst_img.setPixel(x,y,m_cm->m_img.pixel(m_tables[y][x+m_cm->m_clip_left.x].x,m_tables[y][x+m_cm->m_clip_left.x].y));
            }
        }
    }
    for(int y=0;y < m_cm->m_img.height();++y){
        for(int x=0;x < m_cm->m_img.width();++x){
            m_cm->m_init_img.setPixel(x,y,m_cm->m_img.pixel(m_uninit_tables[y][x].x,m_uninit_tables[y][x].y));
        }
    }
}
void hRightParemerProvide::uninit_for_image()
{
    m_cm->m_img.setPixel(0,0,0);
//    m_cm->m_dst_img.fill(Qt::black);
    point p1,p2,p3;
    point p4,p5,p6;
    int H,W;
    if(m_cm->m_isclip){
        p1 = m_cm->m_fg_sp;
        p2 = m_cm->m_fg_ep;
        p4 = m_cm->m_fg_sp1;
        p5 = m_cm->m_fg_ep1;
        int H = m_cm->m_dst_img.height();
        int W = m_cm->m_dst_img.width();
        for(int y=0;y < H;++y){
            for(int x=0;x < W;++x){
                p3.x = x;p3.y = y;
                p6.x = x;p6.y = y;
                float S = (p1.x - p3.x) * (p2.y - p3.y) - (p1.y - p3.y) * (p2.x - p3.x);
                if(S <0.0f){
                    continue;
                }
                S = (p4.x - p6.x) * (p5.y - p6.y) - (p4.y - p6.y) * (p5.x - p6.x);
                if(S > 0.0f)
                    continue;
                if(m_tables[y][x+m_cm->m_clip_left.x].x == 0 && m_tables[y][x+m_cm->m_clip_left.x].y == 0){
                    continue;
                }
                m_cm->m_dst_img.setPixel(x,y,m_cm->m_img.pixel(m_tables[y][x+m_cm->m_clip_left.x].x,m_tables[y][x+m_cm->m_clip_left.x].y));
//                int nx = m_tables[y][x].x;
//                int ny = m_tables[y][x].y;
//                uchar r1,r2,r3,r4,g1,g2,g3,g4,b1,b2,b3,b4;
//                QRgb    p1,p2,p3,p4,p5;
//                p1 = m_cm->m_img.pixel(nx-1,ny);
//                p2 = m_cm->m_img.pixel(nx+1,ny);
//                p3 = m_cm->m_img.pixel(nx-1,ny-1);
//                p4 = m_cm->m_img.pixel(nx+1,ny+1);


//                r1 = qRed(p1);
//                r2 = qRed(p2);
//                r3 = qRed(p3);
//                r4 = qRed(p4);
//                g1 = qGreen(p1);g2 = qGreen(p2);g3 = qGreen(p3);g4 = qGreen(p4);
//                b1 = qBlue(p1);b2 = qBlue(p2);b3 = qBlue(p3);b4 = qBlue(p4);

//                uchar   r5,g5,b5;
//                r5 = r1 * 0.25 + r2 * 0.25 + r3 * 0.25 + r4 * 0.25;
//                g5 = g1 * 0.25 + g2 * 0.25 + g3 * 0.25 + g4 * 0.25;
//                b5 = b1 * 0.25 + b2 * 0.25 + b3 * 0.25 + b4 * 0.25;

//                r5 = ((r5 > 255)?255 : r5);
//                g5 = ((g5 > 255)?255 : g5);
//                b5 = ((b5 > 255)?255 : b5);
//                p4 = qRgb(r5,g5,b5);


//                m_cm->m_dst_img.setPixel(x,y,p4);
            }
        }
    }else{
        H = m_cm->m_dst_img.height();
        W = m_cm->m_dst_img.width();
        for(int y=0;y < H;++y){
            for(int x=0;x < W;++x){
                m_cm->m_dst_img.setPixel(x,y,m_cm->m_img.pixel(m_tables[y][x+m_cm->m_clip_left.x].x,m_tables[y][x+m_cm->m_clip_left.x].y));
            }
        }
    }
    for(int y=0;y < m_cm->m_img.height();++y){
        for(int x=0;x < m_cm->m_img.width();++x){
            m_cm->m_init_img.setPixel(x,y,m_cm->m_img.pixel(m_uninit_tables[y][x].x,m_uninit_tables[y][x].y));
        }
    }
}
void hFrontParemerProvide::uninit_for_image()
{
    point p1,p2,p3,p4,p5,p6;
    m_cm->m_img.setPixel(0,0,0);
//    m_cm->m_dst_img.fill(Qt::black);
    unsigned    char*   src ;
    unsigned    char*   dst = m_cm->m_dst_img.bits();
    if(m_cm->m_isclip){
        p1 = m_cm->m_fg_sp;
        p2 = m_cm->m_fg_ep;
        p4 = m_cm->m_fg_sp1;
        p5 = m_cm->m_fg_ep1;
        for(int y= m_cm->m_clip_top.y;y < m_cm->m_clip_bottom.y;++y){
            for(int x= m_cm->m_clip_left.x; x < m_cm->m_clip_right.x;++x){
                if(m_tables[y][x].x ==0 &&  m_tables[y][x].y==0){
                    continue;
                }
                p3.x = x;p3.y = y;
                p6.x = x;p6.y = y;
                float S = (p1.x - p3.x) * (p2.y - p3.y) - (p1.y - p3.y) * (p2.x - p3.x);
                if(S >0.0f){
                    continue;
                }
                S = (p4.x - p6.x) * (p5.y - p6.y) - (p4.y - p6.y) * (p5.x - p6.x);
                if(S >0.0f)
                    continue;
//                int nx = m_tables[y][x].x - tan(-32 * 3.14/180);
//                int nx = m_tables[y][x].x;
//                int ny = m_tables[y][x].y;
//                uchar r1,r2,r3,r4,g1,g2,g3,g4,b1,b2,b3,b4;
//                QRgb    p1,p2,p3,p4,p5;
//                p1 = m_cm->m_img.pixel(nx-1,ny);
//                p2 = m_cm->m_img.pixel(nx+1,ny);
//                p3 = m_cm->m_img.pixel(nx-1,ny-1);
//                p4 = m_cm->m_img.pixel(nx+1,ny+1);


//                r1 = qRed(p1);
//                r2 = qRed(p2);
//                r3 = qRed(p3);
//                r4 = qRed(p4);
//                g1 = qGreen(p1);g2 = qGreen(p2);g3 = qGreen(p3);g4 = qGreen(p4);
//                b1 = qBlue(p1);b2 = qBlue(p2);b3 = qBlue(p3);b4 = qBlue(p4);

//                uchar   r5,g5,b5;
//                r5 = r1 * 0.25 + r2 * 0.25 + r3 * 0.25 + r4 * 0.25;
//                g5 = g1 * 0.25 + g2 * 0.25 + g3 * 0.25 + g4 * 0.25;
//                b5 = b1 * 0.25 + b2 * 0.25 + b3 * 0.25 + b4 * 0.25;

//                r5 = ((r5 > 255)?255 : r5);
//                g5 = ((g5 > 255)?255 : g5);
//                b5 = ((b5 > 255)?255 : b5);
//                p4 = qRgb(r5,g5,b5);
//                m_cm->m_dst_img.setPixel(x,y,p4);
                m_cm->m_dst_img.setPixel(x,y,m_cm->m_img.pixel(m_tables[y][x].x,m_tables[y][x].y));
            }
        }
    }else{
        int H = m_cm->m_dst_img.height();
        int W = m_cm->m_dst_img.width();
        for(int y=0;y < H;++y){
            for(int x=0;x < W;++x){
                m_cm->m_dst_img.setPixel(x,y,m_cm->m_img.pixel(m_tables[y][x+m_cm->m_clip_left.x].x,m_tables[y][x+m_cm->m_clip_left.x].y));
            }
        }
    }


    for(int y=0;y < m_cm->m_img.height();++y){
        for(int x=0;x < m_cm->m_img.width();++x){
            m_cm->m_init_img.setPixel(x,y,m_cm->m_img.pixel(m_uninit_tables[y][x].x,m_uninit_tables[y][x].y));
        }
    }
}

void hBackParemerProvide::uninit_for_image()
{
    m_cm->m_img.setPixel(0,0,0);
    point p1,p2,p3,p4,p5,p6;
//    m_cm->m_dst_img.fill(Qt::black);
    unsigned    char*   src ;
    unsigned    char*   dst = m_cm->m_dst_img.bits();
    if(m_cm->m_isclip){
        p1 = m_cm->m_fg_sp;
        p2 = m_cm->m_fg_ep;
        p4 = m_cm->m_fg_sp1;
        p5 = m_cm->m_fg_ep1;
        int H = m_cm->m_dst_img.height();
        int W = m_cm->m_dst_img.width();
        for(int y= 0;y < H;++y){
            for(int x= 0;x < W;++x){
                if(m_tables[y][x].x ==0 &&  m_tables[y][x].y==0){
                    continue;
                }
                p3.x = x;p3.y = y;
                p6.x = x;p6.y = y;
                float S = (p1.x - p3.x) * (p2.y - p3.y) - (p1.y - p3.y) * (p2.x - p3.x);
                if(S >0.0f){
                    continue;
                }
                S = (p4.x - p6.x) * (p5.y - p6.y) - (p4.y - p6.y) * (p5.x - p6.x);
                if(S >0.0f)
                    continue;
//                src = m_cm->m_img.bits() + (m_tables[y][x].y * 352 * 4) + m_tables[y][x].x *4;
//                uchar   t0 = (src[0] + src[4] + src[352 * 4] + src[352 * 4 + 4]) / 4;
//                uchar   t1 = (src[1] + src[5] + src[352 * 4 + 1] + src[352 * 4 + 5]) / 4;
//              f  uchar   t2 = (src[2] + src[6] + src[352 * 4 + 2] + src[352 * 4 + 6]) / 4;
//                int nx = m_tables[y][x].x - m_tables[y][x].y * tan(-17 * 3.14 / 180);
                int nx = m_tables[y][x].x;
                m_cm->m_dst_img.setPixel(W-x-1,H-y-1,m_cm->m_img.pixel(nx,m_tables[y][x].y));
            }
        }
    }else{
        int H = m_cm->m_dst_img.height();
        int W = m_cm->m_dst_img.width();
        for(int y=0;y < H;++y){
            for(int x=0;x < W;++x){
//                src = m_cm->m_img.bits() + (m_tables[y][x].y * 352 * 4) + m_tables[y][x].x *4;
//                uchar   t0 = (src[0] + src[4] + src[352 * 4] + src[352 * 4 + 4]) / 4;
//                uchar   t1 = (src[1] + src[5] + src[352 * 4 + 1] + src[352 * 4 + 5]) / 4;
//                uchar   t2 = (src[2] + src[6] + src[352 * 4 + 2] + src[352 * 4 + 6]) / 4;
               // int nx = m_tables[y][x].x - m_tables[y][x].y * tan(-10 * 3.14 / 180);
               // m_cm->m_dst_img.setPixel(351-x,287-y,m_cm->m_img.pixel(nx,m_tables[y][x].y));
                m_cm->m_dst_img.setPixel(W-x-1,H-y-1,m_cm->m_img.pixel(m_tables[y][x+m_cm->m_clip_left.x].x,m_tables[y][x+m_cm->m_clip_left.x].y));
            }
        }
    }


    for(int y=0;y < m_cm->m_img.height();++y){
        for(int x=0;x < m_cm->m_img.width();++x){
            m_cm->m_init_img.setPixel(x,y,m_cm->m_img.pixel(m_uninit_tables[y][x].x,m_uninit_tables[y][x].y));
        }
    }
}


void hLeftParemerProvide::uninit()
{
    uninit_for_image();
    return;
    unsigned    char    *y,*u,*v;
    y = m_cm->m_y;
    u = m_cm->m_u;
    v = m_cm->m_v;
//    hElapsedTimer2  tm;
//    tm.start();
    int R,G,B,Y,U,V,A;
    unsigned    char*   rgb24 = m_cm->m_rgb;
    int nx,ny;
    for(int sy = 0;sy < 352;++sy){
        for(int sx = 0;sx < 288;++sx){
            nx = m_tables[sy][sx].x;
            ny = m_tables[sy][sx].y;
            Y = sget_y(nx,ny);
            U = sget_u(nx,ny);
            V = sget_v(nx,ny);
//            R = 1.164 * (Y-16) + 1.596*(U - 128);
//            G = 1.164 * (Y-16) - 0.813*(U - 128) - 0.392 * (V - 128);
//            B = 1.164 * (Y-16) + 2.017*(V - 128);

            R = Y + g_table_v1[U];
            G = Y - g_table_u1[U] + g_table_v2[V];
            B = Y + g_table_u2[V];
            A = 0;
            if (R>255)R=255;
            if (R<0)R=0;
            if (G>255)G=255;
            if (G<0)G=0;
            if (B>255)B=255;
            if (B<0)B=0;
            *(rgb24 + (sy * 288 * 4) + sx * 4+3) = 1;
            *(rgb24 + (sy * 288 * 4) + sx * 4)   = B;
            *(rgb24 + (sy * 288 * 4) + sx * 4+1) = G;
            *(rgb24 + (sy * 288 * 4) + sx * 4+2) = R;
        }
    }
}

void hBackParemerProvide::init()
{
    float   rx = m_rx;
    float   ry = m_ry;
    float   zx = m_z;
    float   zy = m_z;
    int     xo = m_xo;
    int     yo = m_yo;
    for(int v=0;v < 288;++v){
        for(int u=0;u<352;++u){
            int x = xo + rx *  (u-xo) / sqrt(zx*zx + ((u-xo)*(u-xo)) + ((v-yo) * (v-yo)));
            int y = yo + ry *  (v-yo) / sqrt(zy*zy + ((u-xo)*(u-xo)) + ((v-yo) * (v-yo)));
            if(x<0 || y<0 || x>=352 || y>=288){
                m_uninit_tables[v][u].x = 0;
                m_uninit_tables[v][u].y = 0;
                continue;
            }
            m_uninit_tables[v][u].x = x;//  - y * tan(3 * 3.14/180);
            m_uninit_tables[v][u].y = y;// - x * tan(2.5 * 3.14/180);
        }
    }

    int H = 288;
    int W = 352;
    float   b = m_b * 3.14 / 180;
    float   a = m_a * 3.14 / 180;
    float   y = m_y * 3.14 / 180;
    float   h = m_h ;
    float   m = m_m;
    float   n = m_n;
    float   N = 352;
    float   M = H;
    float   mb = m / M;
    float   nb = n / N;

    for(int i =0; i < H /2;++i){
        for(int j =0; j <W;++j){
            float   px = j * nb - (n / 2);
            float   py = (m / 2) - i * mb + (h / tan(y));

            float   wx = (px * W) / (2 * tan(b) * sqrt(h * h + py * py));
            float   wy = (py * H - H * h * tan(y)) / (2 * py * tan(y) * tan(a) + 2 * h * tan(a));

            int     pi = m_boy  - wy;
            int     pj = m_box  + wx;



            if(pi <0 || pj <0 || pi>=H || pj >=W){
                m_tables[i][j].x = m_tables[i][j].y = 0;
                continue;
            }else{
                m_tables[i][j].x = m_uninit_tables[pi][pj].x;
                m_tables[i][j].y = m_uninit_tables[pi][pj].y;
                continue;
            }
        }
    }
}

void hFrontParemerProvide::init()
{
    float   rx = m_rx;
    float   ry = m_ry;
    float   zx = m_z;
    float   zy = m_z;
    int     xo = m_xo;
    int     yo = m_yo;
    for(int v=0;v < 288;++v){
        for(int u=0;u<352;++u){
            int x = xo + rx *  (u-xo) / sqrt(zx*zx + ((u-xo)*(u-xo)) + ((v-yo) * (v-yo)));
            int y = yo + ry *  (v-yo) / sqrt(zy*zy + ((u-xo)*(u-xo)) + ((v-yo) * (v-yo)));
            if(x<0 || y<0 || x>=352 || y>=288){
                m_uninit_tables[v][u].x = 0;
                m_uninit_tables[v][u].y = 0;
                continue;
            }
            m_uninit_tables[v][u].x = x;
            m_uninit_tables[v][u].y = y;
        }
    }
    int H = 288;
    int W = 352;
    int H1= 288;
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

    for(int i =0; i < H;++i){
        for(int j =0; j <W;++j){
            float   px = j * nb - (n / 2);
            float   py = (m / 2) - i * mb + (h / tan(y));

            float   wx = (px * W) / (2 * tan(b) * sqrt(h * h + py * py));
            float   wy = (py * H - H * h * tan(y)) / (2 * py * tan(y) * tan(a) + 2 * h * tan(a));

            int     pi =  m_boy  - wy;
            int     pj =  m_box  + wx;



            if(pi <0 || pj <0 || pi>=H || pj >=W){
                m_tables[i][j].x = m_tables[i][j].y = 0;
                continue;
            }else{
                m_tables[i][j].x = m_uninit_tables[pi][pj].x;
                m_tables[i][j].y = m_uninit_tables[pi][pj].y;
                continue;
            }
        }
    }
}

void hRightParemerProvide::init()
{
#define TABLE1  m_tables
#define TABLE2  m_uninit_tables
#define TABLE3  m_rotate_tables
#define TABLE4  m_bri_tables
    float   rx = m_rx;
    float   ry = m_ry;
    float   zx = m_z;
    float   zy = m_z;
    int     xo = m_xo;
    int     yo = m_yo;
    for(int v=0;v < 288;++v){
        for(int u=0;u<352;++u){
            int x = xo + rx *  (u-xo) / sqrt(zx*zx + ((u-xo)*(u-xo)) + ((v-yo) * (v-yo)));
            int y = yo + ry *  (v-yo) / sqrt(zy*zy + ((u-xo)*(u-xo)) + ((v-yo) * (v-yo)));
            if(x<0 || y<0 || x>=352 || y>=288){
                TABLE2[v][u].x = 0;
                TABLE2[v][u].y = 0;
                continue;
            }
            TABLE2[288-v][352-u].x = x;
            TABLE2[288-v][352-u].y = y;
        }
    }
    float theta = 90 * 3.14 / 180;
    int newWidth  = 288;
    int newHeight = 352;
    int oldWidth  = 352;
    int oldHeight = 288;
    float dx = -0.5*newWidth*cos(theta) - 0.5*newHeight*sin(theta) + 0.5*oldWidth;
    float dy = 0.5*newWidth*sin(theta) - 0.5*newHeight*cos(theta) + 0.5*oldHeight;
    int x,jy;

    for (int i=0; i<newHeight; i++)
    {
        for (int j=0; j<newWidth; j++)
        {
            x = float(j)*cos(theta) + float(i)*sin(theta) + dx;
            jy = float(-j)*sin(theta) + float(i)*cos(theta) + dy;

            if ((x<0) || (x>=oldWidth) || (jy<0) || (jy>=oldHeight))
            {
                    TABLE1[i][j].x = 0;
                    TABLE1[i][j].y = 0;
            }
            else
            {
                    TABLE1[i][j].x =TABLE2[jy][x].x;
                    TABLE1[i][j].y =TABLE2[jy][x].y;
            }
        }
    }
}


void hLeftParemerProvide::init()
{
#define TABLE1  m_tables
#define TABLE2  m_uninit_tables
#define TABLE3  m_rotate_tables
#define TABLE4  m_bri_tables
    float   rx = m_rx;
    float   ry = m_ry;
    float   zx = m_z;
    float   zy = m_z;
    int     xo = m_xo;
    int     yo = m_yo;
    for(int v=0;v < 288;++v){
        for(int u=0;u<352;++u){
            int x = xo + rx *  (u-xo) / sqrt(zx*zx + ((u-xo)*(u-xo)) + ((v-yo) * (v-yo)));
            int y = yo + ry *  (v-yo) / sqrt(zy*zy + ((u-xo)*(u-xo)) + ((v-yo) * (v-yo)));
            if(x<0 || y<0 || x>=352 || y>=288){
                TABLE2[v][u].x = 0;
                TABLE2[v][u].y = 0;
                continue;
            }
            TABLE2[288-v][352-u].x = x;
            TABLE2[288-v][352-u].y = y;
        }
    }
    float theta = -90 * 3.14 / 180;
    int newWidth  = 288;
    int newHeight = 352;
    int oldWidth  = 352;
    int oldHeight = 288;
    float dx = -0.5*newWidth*cos(theta) - 0.5*newHeight*sin(theta) + 0.5*oldWidth;
    float dy = 0.5*newWidth*sin(theta) - 0.5*newHeight*cos(theta) + 0.5*oldHeight;
    int x,jy;

    for (int i=0; i<newHeight; i++)
    {
        for (int j=0; j<newWidth; j++)
        {
            x = float(j)*cos(theta) + float(i)*sin(theta) + dx;
            jy = float(-j)*sin(theta) + float(i)*cos(theta) + dy;

            if ((x<0) || (x>=oldWidth) || (jy<0) || (jy>=oldHeight))
            {
                    TABLE1[i][j].x = 0;
                    TABLE1[i][j].y = 0;
            }
            else
            {
                    TABLE1[i][j].x =TABLE2[jy][x].x;
                    TABLE1[i][j].y =TABLE2[jy][x].y;
            }
        }
    }


}

void hBackParemerProvide::uninit()
{
    uninit_for_image();
    return;
    unsigned    char    *y,*u,*v;
    y = m_cm->m_y;
    u = m_cm->m_u;
    v = m_cm->m_v;
//    hElapsedTimer2  tm;
//    tm.start();
    int R,G,B,Y,U,V,A;
    unsigned    char*   rgb24 = m_cm->m_rgb;
    int nx,ny;
    for(int sy = 0;sy < 288 - 88;++sy){
        for(int sx = 0;sx < 352;++sx){
            nx = m_tables[sy][sx].x;
            ny = m_tables[sy][sx].y;
            Y = sget_y(nx,ny);
            U = sget_u(nx,ny);
            V = sget_v(nx,ny);
//            R = 1.164 * (Y-16) + 1.596*(U - 128);
//            G = 1.164 * (Y-16) - 0.813*(U - 128) - 0.392 * (V - 128);
//            B = 1.164 * (Y-16) + 2.017*(V - 128);

            R = Y + g_table_v1[U];
            G = Y - g_table_u1[U] + g_table_v2[V];
            B = Y + g_table_u2[V];
            A = 0;
            if (R>255)R=255;
            if (R<0)R=0;
            if (G>255)G=255;
            if (G<0)G=0;
            if (B>255)B=255;
            if (B<0)B=0;
            *(rgb24 + (sy * 288 * 4) + sx * 4+3) = 1;
            *(rgb24 + (sy * 288 * 4) + sx * 4)   = B;
            *(rgb24 + (sy * 288 * 4) + sx * 4+1) = G;
            *(rgb24 + (sy * 288 * 4) + sx * 4+2) = R;
        }
    }
}

void hCamerParemerProvide::uninit()
{
    unsigned    char    *y,*u,*v;
    y = m_cm->m_y;
    u = m_cm->m_u;
    v = m_cm->m_v;
//    hElapsedTimer2  tm;
//    tm.start();
    int R,G,B,Y,U,V,A;
    unsigned    char*   rgb24 = m_cm->m_rgb;
    int nx,ny;
    for(int sy = 0;sy < 288;++sy){
        for(int sx = 0;sx < 352;++sx){
            nx = m_uninit_tables[sy][sx].x;
            ny = m_uninit_tables[sy][sx].y;
            Y = sget_y(nx,ny);
            U = sget_u(nx,ny);
            V = sget_v(nx,ny);
//            R = 1.164 * (Y-16) + 1.596*(U - 128);
//            G = 1.164 * (Y-16) - 0.813*(U - 128) - 0.392 * (V - 128);
//            B = 1.164 * (Y-16) + 2.017*(V - 128);

            R = Y + g_table_v1[U];
            G = Y - g_table_u1[U] + g_table_v2[V];
            B = Y + g_table_u2[V];




            A = 0;
            if (R>255)R=255;
            if (R<0)R=0;
            if (G>255)G=255;
            if (G<0)G=0;
            if (B>255)B=255;
            if (B<0)B=0;
            *(rgb24 + (sy * 352 * 4) + sx * 4+3) = 1;
            *(rgb24 + (sy * 352 * 4) + sx * 4)   = B;
            *(rgb24 + (sy * 352 * 4) + sx * 4+1) = G;
            *(rgb24 + (sy * 352 * 4) + sx * 4+2) = R;
        }
    }
//    LOGE("to rgb time = %d\n",tm.elapsed());
}

void hStitchingLine::draw()
{
    int ystart = dst->m_fg_sp.y;
    int yend   = dst->m_fg_ep.y;
    int width  = src->m_dst_img.width();
    int H = yend - ystart;
    int ph = H / 2;
//    for(int y=ystart; y < ystart+ph;++y){
//        for(int x=0;x < width;++x){
//            dst->m_dst_img.setPixel(x,y,0x00FF0000);
//        }
//    }
//    for(int y=0;y < 30;++y){
//        for(int x=0;x < width;++x){
//            src->m_dst_img.setPixel(x,y+src->m_pp.y,0x0000FF00);
//        }
//    }

//    for(int y=src->m_pp.y+ph;y < src->m_pp.y+H;++y){
//        for(int x=0;x < width;++x){
//            src->m_dst_img.setPixel(x,y,0x0000FF00);
//        }
//    }

}
hStitchingLine::hStitchingLine(hCamer *s, hCamer *d, int stitchingLineCount)
{
    src = s;dst = d;
    line_count = stitchingLineCount;
    line_size = src->m_dst_img.width()+1;
    all_size  = line_size * line_count;
    data = new stitchingline_data[all_size];
    float   xl = ((float)src->m_fg_ep.y - (float)src->m_fg_sp.y) / ((float)src->m_fg_ep.x - (float)src->m_fg_sp.x);
    int     parent = (src->m_pp.y - dst->m_pp.y);
    int     ys = 0;
    for(int a=0;a < line_count;++a){
        ys = src->m_fg_sp.y + a;
        for(int x=0;x < line_size;++x){
            int px = x;int py = ys + x * xl;
            data[a * line_size+x].src.x = px;
            data[a * line_size+x].src.y = py;
            data[a * line_size+x].dst.x = px;
            data[a * line_size+x].dst.y = py + a+parent/2;
        }
    }
}

void hCamerarchive::load(const char *filename, hCamer *p)
{
    ifstream    archive(filename);
    if(!archive.is_open()){
        printf("open save file error\n");
        fflush(stdout);
    }
    archive.read(reinterpret_cast<char*>(&p->m_clip_left),sizeof(p->m_clip_left));
    archive.read(reinterpret_cast<char*>(&p->m_clip_top),sizeof(p->m_clip_top));
    archive.read(reinterpret_cast<char*>(&p->m_clip_right),sizeof(p->m_clip_right));
    archive.read(reinterpret_cast<char*>(&p->m_clip_bottom),sizeof(p->m_clip_bottom));

    archive.read(reinterpret_cast<char*>(&p->m_fg_sp),sizeof(p->m_fg_sp));
    archive.read(reinterpret_cast<char*>(&p->m_fg_ep),sizeof(p->m_fg_ep));
    archive.read(reinterpret_cast<char*>(&p->m_fg_sp1),sizeof(p->m_fg_sp1));
    archive.read(reinterpret_cast<char*>(&p->m_fg_ep1),sizeof(p->m_fg_ep1));

    archive.read(reinterpret_cast<char*>(&p->m_pp),sizeof(point));

    archive.read(reinterpret_cast<char*>(&p->m_helper->m_rx),sizeof(p->m_helper->m_rx));
    archive.read(reinterpret_cast<char*>(&p->m_helper->m_ry),sizeof(p->m_helper->m_ry));
    archive.read(reinterpret_cast<char*>(&p->m_helper->m_z),sizeof(p->m_helper->m_z));
    archive.read(reinterpret_cast<char*>(&p->m_helper->m_xo),sizeof(p->m_helper->m_xo));
    archive.read(reinterpret_cast<char*>(&p->m_helper->m_yo),sizeof(p->m_helper->m_yo));
    archive.read(reinterpret_cast<char*>(&p->m_helper->m_a),sizeof(p->m_helper->m_a));
    archive.read(reinterpret_cast<char*>(&p->m_helper->m_b),sizeof(p->m_helper->m_b));
    archive.read(reinterpret_cast<char*>(&p->m_helper->m_y),sizeof(p->m_helper->m_y));
    archive.read(reinterpret_cast<char*>(&p->m_helper->m_h),sizeof(p->m_helper->m_h));
    archive.read(reinterpret_cast<char*>(&p->m_helper->m_m),sizeof(p->m_helper->m_m));
    archive.read(reinterpret_cast<char*>(&p->m_helper->m_n),sizeof(p->m_helper->m_n));
    archive.read(reinterpret_cast<char*>(&p->m_helper->m_box),sizeof(p->m_helper->m_box));
    archive.read(reinterpret_cast<char*>(&p->m_helper->m_boy),sizeof(p->m_helper->m_boy));


    archive.read(reinterpret_cast<char*>(&p->m_helper->m_tables),sizeof(p->m_helper->m_tables));
    archive.close();
}

void hCamerarchive::save(const char *filename, hCamer *p)
{
    ofstream    archive(filename,std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
    if(!archive.is_open()){
        printf("open save file error\n");
        fflush(stdout);
    }
    archive.write(reinterpret_cast<char*>(&p->m_clip_left),sizeof(p->m_clip_left));
    archive.write(reinterpret_cast<char*>(&p->m_clip_top),sizeof(p->m_clip_top));
    archive.write(reinterpret_cast<char*>(&p->m_clip_right),sizeof(p->m_clip_right));
    archive.write(reinterpret_cast<char*>(&p->m_clip_bottom),sizeof(p->m_clip_bottom));

    archive.write(reinterpret_cast<char*>(&p->m_fg_sp),sizeof(p->m_fg_sp));
    archive.write(reinterpret_cast<char*>(&p->m_fg_ep),sizeof(p->m_fg_ep));
    archive.write(reinterpret_cast<char*>(&p->m_fg_sp1),sizeof(p->m_fg_sp1));
    archive.write(reinterpret_cast<char*>(&p->m_fg_ep1),sizeof(p->m_fg_ep1));

    archive.write(reinterpret_cast<char*>(&p->m_pp),sizeof(point));

    archive.write(reinterpret_cast<char*>(&p->m_helper->m_rx),sizeof(p->m_helper->m_rx));
    archive.write(reinterpret_cast<char*>(&p->m_helper->m_ry),sizeof(p->m_helper->m_ry));
    archive.write(reinterpret_cast<char*>(&p->m_helper->m_z),sizeof(p->m_helper->m_z));
    archive.write(reinterpret_cast<char*>(&p->m_helper->m_xo),sizeof(p->m_helper->m_xo));
    archive.write(reinterpret_cast<char*>(&p->m_helper->m_yo),sizeof(p->m_helper->m_yo));
    archive.write(reinterpret_cast<char*>(&p->m_helper->m_a),sizeof(p->m_helper->m_a));
    archive.write(reinterpret_cast<char*>(&p->m_helper->m_b),sizeof(p->m_helper->m_b));
    archive.write(reinterpret_cast<char*>(&p->m_helper->m_y),sizeof(p->m_helper->m_y));
    archive.write(reinterpret_cast<char*>(&p->m_helper->m_h),sizeof(p->m_helper->m_h));
    archive.write(reinterpret_cast<char*>(&p->m_helper->m_m),sizeof(p->m_helper->m_m));
    archive.write(reinterpret_cast<char*>(&p->m_helper->m_n),sizeof(p->m_helper->m_n));
    archive.write(reinterpret_cast<char*>(&p->m_helper->m_box),sizeof(p->m_helper->m_box));
    archive.write(reinterpret_cast<char*>(&p->m_helper->m_boy),sizeof(p->m_helper->m_boy));


    archive.write(reinterpret_cast<char*>(&p->m_helper->m_tables),sizeof(p->m_helper->m_tables));
    archive.close();
}
