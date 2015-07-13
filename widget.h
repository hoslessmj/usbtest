#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "husbdeviceoperator.h"

#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void    show_helper_info(hCamer*    cm);

    virtual void paintEvent(QPaintEvent*);
    QString camer_to_name(hCamer *cm);
    void    draw_img(QImage *dst,point* pt);

private slots:
    void on_pushButton_clicked();
    void slot_time();
    void slot_stime();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

private:
    Ui::Widget *ui;
    hUsbDeviceOperator*     m_dev;
    QImage                  m_img;
    QImage                  m_car_img;
    QTimer                  m_tm;
    QTimer                  m_sm;

    QImage                  m_quanjing;

    point*                  m_cur_point;
    point                  m_fp,m_bp,m_lp,m_rp,m_cp;

    hCamer*                 m_cur_camer;

    bool                    m_show_all;
    QImage                  m_dst;
    QImage                  m_dst2;
    bool                    m_show_quanjing;

    QFile                   m_yuv_file;
};

#endif // WIDGET_H
