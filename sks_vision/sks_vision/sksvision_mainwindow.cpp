#include "sksvision_mainwindow.h"
#include "ui_sksvision_mainwindow.h"
#include "memory.h"

cv::VideoCapture cap(200);
cv::Mat cvframe;
QImage frame(640,480,QImage::Format_RGB888);
QImage org_frame(640,480,QImage::Format_RGB888);
int bw_sw;

char compareBlackWhite(QImage img, int h);
void Segmentation1(QImage Inimg, QImage *Outimg, int h, int threshold);

int Storage[2][800]={0};
int Position[2][800]={0};
int check_xmax[800]={0};
int check_xmin[800]={0};
int check_size[800]={0};
int push=0,pop=0,xmax=0,xmin=0,check_count=0,push_p=0;
int total_x,total_y;
float final_w,final_h,ang;

sksVision_MainWindow::sksVision_MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::sksVision_MainWindow)
{
    ui->setupUi(this);
    startTimer(1);
}

sksVision_MainWindow::~sksVision_MainWindow()
{
    delete ui;
}

void sksVision_MainWindow::timerEvent(QTimerEvent *)
{
    ui->shownum_linear->setNum(ui->bar_linear->value());
    ui->shownum_threshold->setNum(ui->bar_threshold->value());
    cap >> cvframe;
    frame = MatToQImage(cvframe);

    frame = func_Gray(frame);
    int gray;
    if(ui->checkbox_threshold->isChecked()){
        gray = ui->bar_threshold->value();
    }else{
        gray = Average_Threshold(frame);
    }
    for(int h=0;h<frame.height();h++){
        for(int w=0;w<frame.width();w++){
            if(qRed(frame.pixel(w,h)) > gray)
                frame.setPixel(w,h,QColor(255,255,255).rgb());
            else
                frame.setPixel(w,h,QColor(0,0,0).rgb());
        }
    }
    org_frame = frame;
    for(int dot=2;dot < frame.height()-2;dot+=(frame.height()/ui->bar_linear->value()))
    {
        for(int i=0;i<frame.width();i++)   frame.setPixel(i,dot,QColor(0,255,0).rgb());
    }
//=================================================================================================
//=================================================================================================
    memset(Position,0,sizeof(Position)/sizeof(Position[0][0]));
    for(int h=2;h < frame.height()-2;h+=(frame.height()/ui->bar_linear->value())){
        bw_sw = compareBlackWhite(org_frame,h);
        if(bw_sw==0){
            Segmentation1(org_frame,&frame,h,255);
        }else if(bw_sw==1){
            Segmentation1(org_frame,&frame,h,0);
        }
    }
    if(push_p!=0){
        for(int i=0;i<push_p;i++){
            total_x += Position[0][i];
            total_y += Position[1][i];
        }
        double vector_x=total_x/push_p;
        double vector_y=total_y/push_p;
        double vector_h=Position[1][push_p-1]-vector_y;
        double vector_w=(Position[0][push_p-1]-vector_x)/vector_h;

        for(int i=0;i<vector_h;i++){
            frame.setPixel(Position[0][push_p-1]-vector_w*i,Position[1][push_p-1]-i,QColor(0,0,255).rgb());
        }
        final_w=vector_x-Position[0][push_p-1];
        final_h=Position[1][push_p-1]-vector_y;

        ang=atan(final_w/final_h);

        ui->shownum_avgVector->setText(tr("<font color=blue>(%1 : %2)</font>").arg(final_w).arg(final_h));
        ui->shownum_angular->setNum(ang*180/PI);

        frame.setPixel(vector_x-1,vector_y-1,QColor(0,0,255).rgb());
        frame.setPixel(vector_x-1,vector_y,QColor(0,0,255).rgb());
        frame.setPixel(vector_x-1,vector_y+1,QColor(0,0,255).rgb());
        frame.setPixel(vector_x,vector_y-1,QColor(0,0,255).rgb());
        frame.setPixel(vector_x,vector_y,QColor(0,0,255).rgb());
        frame.setPixel(vector_x,vector_y+1,QColor(0,0,255).rgb());
        frame.setPixel(vector_x+1,vector_y-1,QColor(0,0,255).rgb());
        frame.setPixel(vector_x+1,vector_y,QColor(0,0,255).rgb());
        frame.setPixel(vector_x+1,vector_y+1,QColor(0,0,255).rgb());
    }
    total_x =0,total_y =0,push_p =0;
    ui->showlabel->setPixmap(QPixmap::fromImage(frame));//*/
}

char compareBlackWhite(QImage img, int h){
    int black=1,white=1;
    for(int w=0;w<img.width();w++){
        if(qRed(img.pixel(w,h))==255) white++;
        else    black++;
    }
    if((white==img.width()+1) || (black==img.width()+1)) return 3;
    else if(white > black && white/black<=10) return 1;
    else if(black > white && black/white<=10) return 0;
    else return 2;
}
void Segmentation1(QImage Inimg,QImage *Outimg,int h,int threshold){
    check_count =0;
    int maxsize =0;
    for(int w=2;w<Inimg.width()-2;w++){
        pop=0;push=0;
        if(qRed(Inimg.pixel(w,h))==threshold){
            Storage[0][push]=w;
            Storage[1][push]=h;
            xmin=w;xmax=w;
            push++;
            while(1){
                if(Storage[0][pop] < 638 && Storage[1][pop] < 478){
                    if(qRed(Inimg.pixel(Storage[0][pop]+1,Storage[1][pop]))==threshold){
                        Storage[0][push] = Storage[0][pop]+1;
                        Storage[1][push] = Storage[1][pop];
                        if(threshold==0){
                            Inimg.setPixel(Storage[0][pop]+1,Storage[1][pop],QColor(0,0,0).rgb());
                        }else{
                            Inimg.setPixel(Storage[0][pop]+1,Storage[1][pop],QColor(255,255,255).rgb());
                        }
                        push++;
                    }
                }
                pop++;
                if(pop>push) break ;
            }
            for(int i=0;i<push;i++){
                if(xmax <= Storage[0][i]) xmax = Storage[0][i];
                if(xmin > Storage[0][i]) xmin = Storage[0][i];
            }
            check_xmin[check_count] = xmin;
            check_xmax[check_count] = xmax;
            check_size[check_count] = xmax-xmin;
            check_count++;
        }
    }
    //MaxSize Calculate
    int position;
    for(int i=0;i<check_count;i++){
        if(check_size[i]>maxsize){
            maxsize=check_size[i];
            position=i;
        }
    }
    //BALL_Current_Point
    int Xpos = (check_xmax[position]+check_xmin[position])*0.5;
    Outimg->setPixel(Xpos-1,h+0,QColor(255,0,0).rgb());
    Outimg->setPixel(Xpos+1,h+0,QColor(255,0,0).rgb());
    Outimg->setPixel(Xpos+0,h+0,QColor(255,0,0).rgb());
    Outimg->setPixel(Xpos+0,h-1,QColor(255,0,0).rgb());
    Outimg->setPixel(Xpos+0,h+1,QColor(255,0,0).rgb());
    Outimg->setPixel(Xpos-1,h-1,QColor(255,0,0).rgb());
    Outimg->setPixel(Xpos+1,h-1,QColor(255,0,0).rgb());
    Outimg->setPixel(Xpos+1,h+1,QColor(255,0,0).rgb());
    Outimg->setPixel(Xpos-1,h+1,QColor(255,0,0).rgb());

    Position[0][push_p]=Xpos;
    Position[1][push_p]=h;
    push_p++;
}