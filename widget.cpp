#include "widget.h"
#include "ui_widget.h"
#include "hTools.h"

extern hUsbDeviceOperator*  g_device;
bool    g_start = false;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_cur_camer = 0;
//    m_img = QImage(704,576,QImage::Format_ARGB32);
    m_img.fill(Qt::black);
    this->resize(1366,768);
//    m_show_all = true;
    m_show_quanjing = true;
    m_cur_point = 0;
    m_quanjing = QImage(600,600,QImage::Format_RGB32);
    point   rp,fp,lp,bp;
    fp.x = 50;      fp.y = 0;
    lp.x = fp.x;   lp.y = fp.y+8;
    rp.x = fp.x + 229;rp.y = m_fp.y + 30;
    bp.x = fp.x + 5  ;bp.y = fp.y + 342;
    m_car_img = QImage("/media/quanjing_data/car1.png");
    m_fp = fp;
    m_lp = lp;
    m_rp = rp;
    m_bp = bp;
   // m_yuv_file.setFileName("/media/ram/yuvfile");
   // m_yuv_file.open(QIODevice::ReadWrite);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    m_dev = new hUsbDeviceOperator();
    g_device = m_dev;
    m_dev->start();
    connect(&m_tm,SIGNAL(timeout()),this,SLOT(slot_time()));
    connect(&m_sm,SIGNAL(timeout()),this,SLOT(slot_stime()));
//    m_img = QImage(g_device->all_camer()->m_rgb,352,288,QImage::Format_ARGB32);
    m_tm.start(50);
//    m_sm.start(5000);
    g_start = true;
}

void Widget::paintEvent(QPaintEvent *)
{
    if(g_start){
        if(m_show_quanjing){
            QPainter    dc(this);
            dc.drawImage(0,0,m_quanjing);
            if(m_cur_camer)
                dc.drawImage(500,0,m_cur_camer->m_init_img);
            return;
        }
        if(m_show_all){
            QPainter    dc(this);
            dc.drawImage(0,0,g_device->ch_data(0)->m_img);
            dc.drawImage(353,0,g_device->ch_data(1)->m_img);
            dc.drawImage(0,353,g_device->ch_data(2)->m_img);
            dc.drawImage(353,353,g_device->ch_data(3)->m_img);
        }else{
            QPainter    dc(this);
            dc.drawImage(0,0,m_cur_camer->m_img);
            dc.drawImage(352,0,m_cur_camer->m_dst_img);
        }
    }
}
void Widget::slot_stime()
{
    on_pushButton_2_clicked();
}
void Widget::draw_img(QImage *dst, point *pt)
{
    for(int y=0;y < dst->height();++y){
        for(int x=0;x < dst->width();++x){
            int color = dst->pixel(x,y);
            if(color == 0xFF000000 || color == 0x0)
                continue;
            int scolor = m_quanjing.pixel(pt->x + x,pt->y + y);
            if(scolor == 0xFF000000 || scolor == 0x0){
                m_quanjing.setPixel(pt->x + x,pt->y + y,color);
            }else{
                  m_quanjing.setPixel(pt->x + x,pt->y + y,color);
            }
        }
    }
}

void Widget::slot_time()
{
//    g_device->all_camer()->toRgb();

//    static  int p = 0;
//    if(p+1 >=1000){
//        m_yuv_file.close();
//        exit(111);
//    }else{
//        ++p;
//        m_yuv_file.write((const char*)g_device->ch_data(2)->m_y,352 * 288);
//        m_yuv_file.write((const char*)g_device->ch_data(2)->m_u,176 * 144);
//        m_yuv_file.write((const char*)g_device->ch_data(2)->m_v,176 * 144);
//    }



    for(int x=0;x < 4;++x){
        g_device->ch_data(x)->toRgb();
        g_device->ch_data(x)->un_init();
    }
//    g_device->all_camer()->toRgb();

    //check;



    draw_img(&g_device->ch_data(3)->m_dst_img,&g_device->ch_data(3)->m_pp);
    draw_img(&g_device->ch_data(0)->m_dst_img,&g_device->ch_data(0)->m_pp);
    draw_img(&g_device->ch_data(1)->m_dst_img,&g_device->ch_data(1)->m_pp);
    draw_img(&g_device->ch_data(2)->m_dst_img,&g_device->ch_data(2)->m_pp);

         point   cp;
         cp.x =  120;
         cp.y =  84;
         draw_img(&m_car_img,&cp);
//         m_show_quanjing=true;
         g_start = true;
//         hCamer*    f = g_device->ch_data(1);
//         hCamer*    l = g_device->ch_data(3);
//         hCamer*    r = g_device->ch_data(0);
//         int ystart = f->m_fg_sp.y;
//         int yend   = f->m_fg_ep.y;
//         int xstart = f->m_pp.x;
//         int xend   = l->m_dst_img.width()+xstart;
//         for(int y=ystart;y < yend;++y){
//             for(int x=xstart;x < xend;++x){
//                 int s1,s2,s3,s4,s5,s6,s7,s8,s9;
//                 s1 = m_quanjing.pixel(x-1,y-1);
//                 s2 = m_quanjing.pixel(x,y-1);
//                 s3 = m_quanjing.pixel(x+1,y-1);
//                 s4 = m_quanjing.pixel(x-1,y);
//                 s5 = m_quanjing.pixel(x,y);
//                 s6 = m_quanjing.pixel(x+1,y);
//                 s7 = m_quanjing.pixel(x-1,y+1);
//                 s8 = m_quanjing.pixel(x,y+1);
//                 s9 = m_quanjing.pixel(x+1,y+1);
//                 uchar r = (qRed(s1)+qRed(s2)+qRed(s3)+qRed(s4)+qRed(s5)+qRed(s6)+qRed(s7)+qRed(s8)+qRed(s9))/9 ;
//                 uchar g = (qGreen(s1)+qGreen(s2)+qGreen(s3)+qGreen(s4)+qGreen(s5)+qGreen(s6)+qGreen(s7)+qGreen(s8)+qGreen(s9))/9;
//                 uchar b = (qBlue(s1) +qBlue(s2) +qBlue(s3) +qBlue(s4) +qBlue(s5) +qBlue(s6) +qBlue(s7) +qBlue(s8) +qBlue(s9))/9;
//                 m_quanjing.setPixel(x,y,qRgb(r,g,b));
//             }
//         }
//         ystart = f->m_fg_ep1.y;
//         yend   = f->m_fg_sp1.y;
//         xstart = f->m_fg_sp1.x + f->m_pp.x;
//         xend   = xstart + r->m_dst_img.width();
//         for(int y=ystart;y < yend;++y){
//             for(int x=xstart;x < xend;++x){
//                 int s1,s2,s3,s4,s5,s6,s7,s8,s9;
//                 s1 = m_quanjing.pixel(x-1,y-1);
//                 s2 = m_quanjing.pixel(x,y-1);
//                 s3 = m_quanjing.pixel(x+1,y-1);
//                 s4 = m_quanjing.pixel(x-1,y);
//                 s5 = m_quanjing.pixel(x,y);
//                 s6 = m_quanjing.pixel(x+1,y);
//                 s7 = m_quanjing.pixel(x-1,y+1);
//                 s8 = m_quanjing.pixel(x,y+1);
//                 s9 = m_quanjing.pixel(x+1,y+1);
//                 uchar r = (qRed(s1)+qRed(s2)+qRed(s3)+qRed(s4)+qRed(s5)+qRed(s6)+qRed(s7)+qRed(s8)+qRed(s9))/9 ;
//                 uchar g = (qGreen(s1)+qGreen(s2)+qGreen(s3)+qGreen(s4)+qGreen(s5)+qGreen(s6)+qGreen(s7)+qGreen(s8)+qGreen(s9))/9;
//                 uchar b = (qBlue(s1) +qBlue(s2) +qBlue(s3) +qBlue(s4) +qBlue(s5) +qBlue(s6) +qBlue(s7) +qBlue(s8) +qBlue(s9))/9;
//                 m_quanjing.setPixel(x,y,qRgb(r,g,b));
//             }
//         }

    update();
}

void Widget::on_pushButton_2_clicked()
{
    static  int ch = 0;
    for(int x=0;x < 4;++x){
        QString str = QString("/media/img/%1_%2.png").arg(x).arg(ch);
        g_device->ch_data(x)->m_img.save(str,"png");
    }
    ch++;
}

void Widget::show_helper_info(hCamer *cm)
{
    ui->lineEdit->setText(QString::number(cm->m_helper->m_rx));
    ui->lineEdit_2->setText(QString::number(cm->m_helper->m_ry));
    ui->lineEdit_3->setText(QString::number(cm->m_helper->m_z));
    ui->lineEdit_4->setText(QString::number(cm->m_helper->m_xo));
    ui->lineEdit_5->setText(QString::number(cm->m_helper->m_yo));

    ui->lineEdit_6->setText(QString::number(cm->m_helper->m_a));
    ui->lineEdit_7->setText(QString::number(cm->m_helper->m_y));
    ui->lineEdit_8->setText(QString::number(cm->m_helper->m_b));
    ui->lineEdit_9->setText(QString::number(cm->m_helper->m_h));
    ui->lineEdit_10->setText(QString::number(cm->m_helper->m_m));
    ui->lineEdit_11->setText(QString::number(cm->m_helper->m_n));
}

void Widget::on_pushButton_5_clicked()
{
    m_cur_point = &m_fp;
    m_cur_camer = g_device->ch_data(hCamer::FRONT);
    show_helper_info(m_cur_camer);
}

void Widget::on_pushButton_6_clicked()
{
    m_cur_point = &m_bp;
    m_cur_camer = g_device->ch_data(hCamer::BACK);
    show_helper_info(m_cur_camer);
}

void Widget::on_pushButton_7_clicked()
{
    m_cur_point = &m_lp;
    m_cur_camer = g_device->ch_data(hCamer::LEFT);
    show_helper_info(m_cur_camer);
}

void Widget::on_pushButton_8_clicked()
{
    m_cur_point = &m_rp;
    m_cur_camer = g_device->ch_data(hCamer::RIGHT);
    show_helper_info(m_cur_camer);
}

void Widget::on_pushButton_9_clicked()
{
    m_cur_point = &m_cp;
}

void Widget::on_pushButton_3_clicked()
{
    int rx = ui->lineEdit->text().toInt();
    int ry = ui->lineEdit_2->text().toInt();
    int rz = ui->lineEdit_3->text().toInt();
    int xo = ui->lineEdit_4->text().toInt();
    int yo = ui->lineEdit_5->text().toInt();
    if(m_cur_camer == 0)
        return;
    m_cur_camer->m_helper->reset(rx,ry,rz,xo,yo,
                                 m_cur_camer->m_helper->m_a,
                                 m_cur_camer->m_helper->m_b,
                                 m_cur_camer->m_helper->m_y,
                                 m_cur_camer->m_helper->m_h,
                                 m_cur_camer->m_helper->m_m,
                                 m_cur_camer->m_helper->m_n);
    update();
}

void Widget::on_pushButton_4_clicked()
{
    float a = ui->lineEdit_6->text().toFloat();
    float y = ui->lineEdit_7->text().toFloat();
    float b = ui->lineEdit_8->text().toFloat();
    float h = ui->lineEdit_9->text().toFloat();
    float m = ui->lineEdit_10->text().toFloat();
    float n = ui->lineEdit_11->text().toFloat();

    if(m_cur_camer == 0)
        return ;
    m_cur_camer->m_helper->reset(m_cur_camer->m_helper->m_rx,
                                 m_cur_camer->m_helper->m_ry,
                                 m_cur_camer->m_helper->m_z,
                                 m_cur_camer->m_helper->m_xo,
                                 m_cur_camer->m_helper->m_yo,
                                 a,b,y,h,m,n);
    update();
}

void Widget::on_pushButton_10_clicked()
{
    m_show_all = !m_show_all;
}

void Widget::on_pushButton_11_clicked()
{
    if(m_sm.isActive()){
        m_sm.stop();
        ui->pushButton_11->setText("capture");
    }else{
        m_sm.start(5000);
        ui->pushButton_11->setText("stop");
    }
}

void Widget::on_pushButton_12_clicked()
{
    m_show_quanjing = !m_show_quanjing;
}

void Widget::on_pushButton_13_clicked()
{
    if(m_cur_point){
        m_cur_point->x++;
    }

}

void Widget::on_pushButton_14_clicked()
{
    if(m_cur_point){
        m_cur_point->x--;
    }
}

void Widget::on_pushButton_15_clicked()
{
    if(m_cur_point){
        m_cur_point->y++;
    }
}

void Widget::on_pushButton_16_clicked()
{
    if(m_cur_point){
        m_cur_point->y++;
    }
}
QString Widget::camer_to_name(hCamer *cm)
{
    switch(cm->m_ch){
    case hCamer::LEFT:
        return  "Left";
    case hCamer::RIGHT:
        return  "Right";
    case hCamer::FRONT:
        return  "Front";
    case hCamer::BACK:
        return  "Back";
    }
}

void Widget::on_pushButton_17_clicked()
{
    static  hLeftCamer* l = new hLeftCamer();
    static  hRightCamer*r = new hRightCamer();
    static  hBackCamer* b = new hBackCamer();
    static  hFrontCamer*f = new hFrontCamer();
    static  int     cur = 0;
    static  bool    first = true;
    if(first){
        l->init();
        r->init();
        b->init();
        f->init();
        l->create_helper();
        r->create_helper();
        b->create_helper();
        f->create_helper();
        hCamerarchive::load(camer_to_name(l).toStdString().c_str(),l);
        hCamerarchive::load(camer_to_name(r).toStdString().c_str(),r);
        hCamerarchive::load(camer_to_name(b).toStdString().c_str(),b);
        hCamerarchive::load(camer_to_name(f).toStdString().c_str(),f);
        first = false;
        f->m_pp.x = f->m_pp.y = 0;
        l->m_pp.x = f->m_pp.x;
        l->m_pp.y = f->m_pp.y + 25;
        r->m_pp.x = f->m_pp.x + 232;
        r->m_pp.y = f->m_pp.y + 30;
        b->m_pp.x = f->m_pp.x;
        b->m_pp.y = f->m_pp.y + 350;
    }
    QString lname =QString("/media/img/3_%1.png").arg(cur);
    QString rname =QString("/media/img/0_%1.png").arg(cur);
    QString fname =QString("/media/img/1_%1.png").arg(cur);
    QString bname =QString("/media/img/2_%1.png").arg(cur++);
    l->load(lname.toStdString().c_str());
    r->load(rname.toStdString().c_str());
    f->load(fname.toStdString().c_str());
    b->load(bname.toStdString().c_str());
    l->m_isclip = r->m_isclip = f->m_isclip = b->m_isclip = true;
    l->un_init();
    r->un_init();
    f->un_init();
    b->un_init();

    draw_img(&l->m_dst_img,&l->m_pp);
    draw_img(&r->m_dst_img,&r->m_pp);
    draw_img(&f->m_dst_img,&f->m_pp);
    draw_img(&b->m_dst_img,&b->m_pp);
    point   cp;
    cp.x = f->m_pp.x + 120;
    cp.y = f->m_pp.y + 84;
    draw_img(&m_car_img,&cp);
    m_show_quanjing=true;
    g_start = true;
    int ystart = f->m_fg_sp.y;
    int yend   = f->m_fg_ep.y;
    int xstart = f->m_pp.x;
    int xend   = l->m_dst_img.width()+xstart;
    for(int y=ystart;y < yend;++y){
        for(int x=xstart;x < xend;++x){
            int s1,s2,s3,s4,s5,s6,s7,s8,s9;
            s1 = m_quanjing.pixel(x-1,y-1);
            s2 = m_quanjing.pixel(x,y-1);
            s3 = m_quanjing.pixel(x+1,y-1);
            s4 = m_quanjing.pixel(x-1,y);
            s5 = m_quanjing.pixel(x,y);
            s6 = m_quanjing.pixel(x+1,y);
            s7 = m_quanjing.pixel(x-1,y+1);
            s8 = m_quanjing.pixel(x,y+1);
            s9 = m_quanjing.pixel(x+1,y+1);
            uchar r = (qRed(s1)+qRed(s2)+qRed(s3)+qRed(s4)+qRed(s5)+qRed(s6)+qRed(s7)+qRed(s8)+qRed(s9))/9 ;
            uchar g = (qGreen(s1)+qGreen(s2)+qGreen(s3)+qGreen(s4)+qGreen(s5)+qGreen(s6)+qGreen(s7)+qGreen(s8)+qGreen(s9))/9;
            uchar b = (qBlue(s1) +qBlue(s2) +qBlue(s3) +qBlue(s4) +qBlue(s5) +qBlue(s6) +qBlue(s7) +qBlue(s8) +qBlue(s9))/9;
            m_quanjing.setPixel(x,y,qRgb(r,g,b));
        }
    }
    ystart = f->m_fg_ep1.y;
    yend   = f->m_fg_sp1.y;
    xstart = f->m_fg_sp1.x + f->m_pp.x;
    xend   = xstart + r->m_dst_img.width();
    for(int y=ystart;y < yend;++y){
        for(int x=xstart;x < xend;++x){
            int s1,s2,s3,s4,s5,s6,s7,s8,s9;
            s1 = m_quanjing.pixel(x-1,y-1);
            s2 = m_quanjing.pixel(x,y-1);
            s3 = m_quanjing.pixel(x+1,y-1);
            s4 = m_quanjing.pixel(x-1,y);
            s5 = m_quanjing.pixel(x,y);
            s6 = m_quanjing.pixel(x+1,y);
            s7 = m_quanjing.pixel(x-1,y+1);
            s8 = m_quanjing.pixel(x,y+1);
            s9 = m_quanjing.pixel(x+1,y+1);
            uchar r = (qRed(s1)+qRed(s2)+qRed(s3)+qRed(s4)+qRed(s5)+qRed(s6)+qRed(s7)+qRed(s8)+qRed(s9))/9 ;
            uchar g = (qGreen(s1)+qGreen(s2)+qGreen(s3)+qGreen(s4)+qGreen(s5)+qGreen(s6)+qGreen(s7)+qGreen(s8)+qGreen(s9))/9;
            uchar b = (qBlue(s1) +qBlue(s2) +qBlue(s3) +qBlue(s4) +qBlue(s5) +qBlue(s6) +qBlue(s7) +qBlue(s8) +qBlue(s9))/9;
            m_quanjing.setPixel(x,y,qRgb(r,g,b));
        }
    }
//    ystart = b->m_fg_sp.y;
//    yend   = b->m_fg_ep.y;
//    xstart = b->m_pp.x;
//    xend   = l->m_dst_img.width()+xstart;
//    for(int y=ystart;y < yend;++y){
//        for(int x=xstart;x < xend;++x){
//            int s1,s2,s3,s4,s5,s6,s7,s8,s9;
//            s1 = m_quanjing.pixel(x-1,y-1);
//            s2 = m_quanjing.pixel(x,y-1);
//            s3 = m_quanjing.pixel(x+1,y-1);
//            s4 = m_quanjing.pixel(x-1,y);
//            s5 = m_quanjing.pixel(x,y);
//            s6 = m_quanjing.pixel(x+1,y);
//            s7 = m_quanjing.pixel(x-1,y+1);
//            s8 = m_quanjing.pixel(x,y+1);
//            s9 = m_quanjing.pixel(x+1,y+1);
//            uchar r = (qRed(s1)+qRed(s2)+qRed(s3)+qRed(s4)+qRed(s5)+qRed(s6)+qRed(s7)+qRed(s8)+qRed(s9))/9 ;
//            uchar g = (qGreen(s1)+qGreen(s2)+qGreen(s3)+qGreen(s4)+qGreen(s5)+qGreen(s6)+qGreen(s7)+qGreen(s8)+qGreen(s9))/9;
//            uchar b = (qBlue(s1) +qBlue(s2) +qBlue(s3) +qBlue(s4) +qBlue(s5) +qBlue(s6) +qBlue(s7) +qBlue(s8) +qBlue(s9))/9;
//            m_quanjing.setPixel(x,y,qRgb(r,g,b));
//        }
//    }



    update();
}
