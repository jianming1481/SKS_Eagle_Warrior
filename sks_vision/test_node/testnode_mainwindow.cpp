#include "testnode_mainwindow.h"
#include "ui_testnode_mainwindow.h"
#include <QImage>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QDebug>
#include <math.h>
#include <ros/ros.h>
#include "geometry_msgs/Twist.h"
#define pi 3.14159265359

//double Hue(double rr,double gg,double bb);
//double Saturation(double rr,double gg,double bb);
//double Value(double rr,double gg,double bb);
//double minimum(double a,double b,double c);
//double maximum(double a,double b,double c);
/*int pushbutton,pushbutton2,pushbutton3,pushbutton4,pushbutton5,pushbutton6,pushbutton7,pushbutton8;
int check;
int xmax,xmin,ymax,ymin,area,bigarea;
static int xminsave,xmaxsave,yminsave,ymaxsave;*/
double bigerall,h_all;
int h;
int pushbutton;
double biger_x;
float final_w,final_h;
double ang;
//QImage img(640,480,QImage::Format_RGB888);
//QImage img2(640,480,QImage::Format_RGB888);
cv::Mat frame_camera;

using namespace cv;
using namespace std;
//testNode_MainWindow::testNode_MainWindow(QWidget *parent) :
testnode_MainWindow::testnode_MainWindow(CvCapture *cam, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::testnode_MainWindow)
{
    ui->setupUi(this);
    m_camera = cam;
    startTimer(1);
}
testnode_MainWindow::~testnode_MainWindow()
{
    delete ui;
}

void testnode_MainWindow::timerEvent(QTimerEvent *)
{
    ros::Publisher pub = n->advertise<geometry_msgs::Twist>("/topic/name",1);
    ros::Rate loop_rate(100);
    geometry_msgs::Twist vector3;

    int S[2][640*480]={0};
    int P[2][100]={0};
    double white,black,w;
    int xmax1,xmin1;
    int big_min,big_max;
    int big;
    int biger;
    int push=0,pop=0;
    int push_p;

    ui->line_num->setNum(ui->line_bar->value());
    if(ui->tabWidget->currentIndex()==1)
    {
        IplImage *showCamera = cvQueryFrame(m_camera);
        QImage* frame = IplImageToQImage(showCamera);
        QImage* img = IplImageToQImage(showCamera);

      //  img2 = *frame;
//        img2.load("/home/iclab/catkin_ws/src/testQT/whiteline2.jpg");
        ui->showImg2->setPixmap(QPixmap::fromImage(*frame));
        if(ui->checkBox->isChecked())
        {
            int gray;
            for(int h=0;h<frame->height();h++){
                for(int w=0;w<frame->width();w++){
                    gray=(qRed(frame->pixel(w,h))+qGreen(frame->pixel(w,h))+qBlue(frame->pixel(w,h)))/3;
                    frame->setPixel(w,h,QColor(gray,gray,gray).rgb());
                }
            }
            ui->medium_num->setNum(ui->medium_bar->value());
            for(int h=0;h<frame->height();h++){
                for(int w=0;w<frame->width();w++){
                    if(qRed(frame->pixel(w,h))>ui->medium_bar->value()){
                        frame->setPixel(w,h,QColor(255,255,255).rgb());
                    }else{
                        frame->setPixel(w,h,QColor(0,0,0).rgb());
                    }
                 }
            }
        }else{
            static int color_num1,color_num2,color1,color2,color;
            for(int h=0;h<frame->height();h++)
            {
                for(int w=0;w<frame->width();w++)
                {
                    color+=qRed(frame->pixel(w,h));
                }
            }
            static int all_0=color/(640*480);
            for(int h=0;h<frame->height();h++)
            {
                for(int w=0;w<frame->width();w++)
                {
                    if(qRed(frame->pixel(w,h))>all_0)
                    {
                        color1 += qRed(frame->pixel(w,h));
                        color_num1++;
                    }else{
                        color2 += qRed(frame->pixel(w,h));
                        color_num2++;
                    }
            }
        }
        static int all_1=color1/color_num1;
        static int all_2=color2/color_num2;
        double all=(all_1+all_2)/2;
        for(int h=0;h<frame->height();h++)
        {
            for(int w=0;w<frame->width();w++)
            {
                if(qRed(frame->pixel(w,h))>all)
                {
                    frame->setPixel(w,h,QColor(255,255,255).rgb());
                }else{
                    frame->setPixel(w,h,QColor(0,0,0).rgb());
                }
            }
        }
    }
    *img=*frame;
    int i;
    for(int dot=1;dot<frame->height()-1;dot+=(frame->height()/ui->line_bar->value()))
    {
        for( i=0;i<frame->width();i++)
        {
            frame->setPixel(i,dot,QColor(0,143,0).rgb());
        }
    }
    for(h=1;h<frame->height()-1;h=h+(frame->height()/ui->line_bar->value()))
    {
        black=0;
        white=0;
        for( w=0;w<img->width();w++)
        {
            if(qRed(img->pixel(w,h))==255)
            {
                white++;
            }else{
                black++;
            }
        }
        if(black>white&&black/white<=10)
        {
            for( w=0;w<img->width()-1;w++)
            {
                if(qRed(img->pixel(w,h))==255)
                {
                    //img.setPixel(w,h,QColor(0,0,0).rgb());
                    S[0][push]=w;
                    S[1][push]=h;
                    xmin1=w;xmax1=w;
                    push++;

                    while(1)
                    {
                        if(S[0][pop]<639&&S[1][pop]<480)
                        {
                            if(qRed(img->pixel(S[0][pop]+1,S[1][pop]))==255)
                            {
                                //img.setPixel(S[0][pop]+1,S[1][pop],QColor(0,0,0).rgb());
                                S[0][push]=S[0][pop]+1;
                                S[1][push]=S[1][pop];
                                push++;
                            }
                         }
                          pop++;
                          if(pop>=push)
                          {
                              break ;
                          }
                    }
                    for(int i=0;i<push;i++)
                    {
                        if(xmax1<S[0][i]) xmax1=S[0][i];
                        if(xmin1>S[0][i]) xmin1=S[0][i];
                     }
                    big=xmax1-xmin1;
                    if(big>biger)
                    {
                        biger=big;
                        big_max=xmax1;
                        big_min=xmin1;
                    }
                    pop=0;
                    push=0;
                }
            }
            biger_x=(big_max+big_min)/2;

            frame->setPixel(biger_x-1,h,QColor(255,0,0).rgb());
            frame->setPixel(biger_x+1,h,QColor(255,0,0).rgb());
            frame->setPixel(biger_x,h,QColor(255,0,0).rgb());
            frame->setPixel(biger_x,h-1,QColor(255,0,0).rgb());
            frame->setPixel(biger_x,h+1,QColor(255,0,0).rgb());
            frame->setPixel(biger_x-1,h-1,QColor(255,0,0).rgb());
            frame->setPixel(biger_x+1,h-1,QColor(255,0,0).rgb());
            frame->setPixel(biger_x+1,h+1,QColor(255,0,0).rgb());
            frame->setPixel(biger_x-1,h+1,QColor(255,0,0).rgb());

            P[0][push_p]=biger_x;
            P[1][push_p]=h;
            push_p++;

            biger=0;
        }
        else if(black<white&&white/black<=10)
        {
            for( w=0;w<img->width()-1;w++)
            {
                if(qRed(img->pixel(w,h))==0)
                {
                    //img.setPixel(w,h,QColor(255,255,255).rgb());
                    S[0][push]=w;
                    S[1][push]=h;
                    xmin1=w;xmax1=w;
                    push++;
                    while(1)
                    {
                        if(S[0][pop]<639&&S[1][pop]<480)
                        {
                            if(qRed(img->pixel(S[0][pop]+1,S[1][pop]))==0)
                            {
                                //img.setPixel(S[0][pop]+1,S[1][pop],QColor(255,255,255).rgb());
                                S[0][push]=S[0][pop]+1;
                                S[1][push]=S[1][pop];
                                push++;
                            }
                        }
                        pop++;
                        if(pop>=push)
                        {
                            break ;
                        }
                    }
                    for(int i=0;i<push;i++)
                    {
                        if(xmax1<S[0][i]) xmax1=S[0][i];
                        if(xmin1>S[0][i]) xmin1=S[0][i];
                    }
                    big=xmax1-xmin1;
                    if(big>biger)
                    {
                        biger=big;
                        big_max=xmax1;
                        big_min=xmin1;
                    }
                    pop=0;
                    push=0;
                }
            }
            biger_x=(big_max+big_min)/2;

            frame->setPixel(biger_x-1,h,QColor(255,0,0).rgb());
            frame->setPixel(biger_x+1,h,QColor(255,0,0).rgb());
            frame->setPixel(biger_x,h,QColor(255,0,0).rgb());
            frame->setPixel(biger_x,h-1,QColor(255,0,0).rgb());
            frame->setPixel(biger_x,h+1,QColor(255,0,0).rgb());
            frame->setPixel(biger_x-1,h-1,QColor(255,0,0).rgb());
            frame->setPixel(biger_x+1,h-1,QColor(255,0,0).rgb());
            frame->setPixel(biger_x+1,h+1,QColor(255,0,0).rgb());
            frame->setPixel(biger_x-1,h+1,QColor(255,0,0).rgb());

            P[0][push_p]=biger_x;
            P[1][push_p]=h;
            push_p++;

            biger=0;
        }
    }//h
    for(i=0;i<push_p;i++)
    {
        bigerall=bigerall+P[0][i];
        h_all=h_all+P[1][i];
    }
    double vector_x=bigerall/push_p;
    double vector_y=h_all/push_p;
    double vector_h=P[1][push_p-1]-vector_y;
    double vector_w=(P[0][push_p-1]-vector_x)/vector_h;

    for(int i=0;i<vector_h;i++){
        frame->setPixel(P[0][push_p-1]-vector_w*i,P[1][push_p-1]-i,QColor(0,0,255).rgb());
    }

    final_w=vector_x-P[0][push_p-1];
    final_h=P[1][push_p-1]-vector_y;

    ang=atan(final_w/final_h);

    ui->final_w_num->setNum(final_w);
    ui->final_h_num->setNum(final_h);
    ui->w_num->setNum(ang*180/pi);

    frame->setPixel(vector_x-1,vector_y-1,QColor(0,0,255).rgb());
    frame->setPixel(vector_x-1,vector_y,QColor(0,0,255).rgb());
    frame->setPixel(vector_x-1,vector_y+1,QColor(0,0,255).rgb());
    frame->setPixel(vector_x,vector_y-1,QColor(0,0,255).rgb());
    frame->setPixel(vector_x,vector_y,QColor(0,0,255).rgb());
    frame->setPixel(vector_x,vector_y+1,QColor(0,0,255).rgb());
    frame->setPixel(vector_x+1,vector_y-1,QColor(0,0,255).rgb());
    frame->setPixel(vector_x+1,vector_y,QColor(0,0,255).rgb());
    frame->setPixel(vector_x+1,vector_y+1,QColor(0,0,255).rgb());
    push_p=0;
    bigerall=0;
    h_all=0;

    vector3.linear.x=final_w;
    vector3.angular.z=ang;
    pub.publish(vector3);
    loop_rate.sleep();

    ui->showImg2->setPixmap(QPixmap::fromImage(*frame));
    }
}
//double Hue(double rr,double gg,double bb)
//{
//    double a=rr/255;
//    double b=gg/255;
//    double c=bb/255;
//    double MAX=maximum(rr,gg,bb)/255;
//    double MIN=minimum(rr,gg,bb)/255;
//    double delta1=MAX-MIN;
//    if(MAX==MIN){
//        return 0;
//    }else if(MAX==a&& b>=c){
//        return 60*((b-c)/delta1);
//    }else if(MAX==a&& b<c){
//        return 60*((b-c)/delta1)+360;
//    }else if(MAX==b){
//        return 60*((c-a)/delta1)+120;
//    }else if(MAX==c){
//        return 60*((a-b)/delta1)+240;
//    }
//}
//double Saturation(double rr,double gg,double bb)
//{
//    double MAX=maximum(rr,gg,bb)/255;
//    double MIN=minimum(rr,gg,bb)/255;
//    double delta1=MAX-MIN;
//    if(MAX==0){
//        return 0;
//    }else{
//        return (delta1/MAX)*100;
//    }
//}
//double Value(double rr,double gg,double bb)
//{
//    double MAX=maximum(rr,gg,bb)/255;
//    return MAX*100;
//}
//double maximum(double a,double b,double c)
//{
//    return std::max(std::max(a,b),c);
//}
//double minimum(double a,double b,double c)
//{
//    return min(min(a,b),c);
//}

void testnode_MainWindow::ros_pub()
{
    int count=0;
    ros::Publisher pub = n->advertise<geometry_msgs::Twist>("/topic/name",1);
    ros::Rate loop_rate(100);
   while (ros::ok())
   {
        geometry_msgs::Twist vector3;
        vector3.linear.x=final_w;
        vector3.angular.z=ang;
        pub.publish(vector3);
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
        cout<<vector3.linear.x<<endl;
    }
}

QImage* testnode_MainWindow::IplImageToQImage(const IplImage *pIplImage)
{
    int w = pIplImage->width;
    int h = pIplImage->height;
    int x, y;
    QImage *qImage;
    qImage = new QImage(w, h, QImage::Format_RGB32);
    for(x = 0; x < pIplImage->width; ++x)
    {
        for(y = 0; y < pIplImage->height; ++y)
        {
            CvScalar color = cvGet2D(pIplImage, y, x);

            int r = color.val[2];
            int g = color.val[1];
            int b = color.val[0];

            qImage->setPixel(x, y, qRgb(r,g,b));
        }
    }
    return qImage;
}
QImage testnode_MainWindow::MatToQImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

void testnode_MainWindow::on_start_clicked()
{
    ros_pub();

}
