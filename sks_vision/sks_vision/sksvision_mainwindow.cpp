#include "sksvision_mainwindow.hpp"
#include "sks_vision/ui_sksvision_mainwindow.h"
#include "memory.h"
#include <geometry_msgs/PoseStamped.h>
//#define THRESHOLD_MAX 37
//#define THRESHOLD_MIN 35
#define BLACK_BLOCK 305000
#define ROBOT_CENTER 367

cv::VideoCapture cap(200);
cv::Mat cvframe,cvGray;
QImage frame(640,480,QImage::Format_RGB888);
QImage org_frame(640,480,QImage::Format_RGB888);
//QImage test_frame(640,480,QImage::Format_RGB888);
int bw_sw;
//int h_max;

geometry_msgs::Vector3 vec3;
geometry_msgs::PoseStamped cu_pos;

char compareBlackWhite(QImage img, int height);
void Segmentation1(QImage Inimg, QImage *Outimg, int h, int threshold);

int Storage[2][800]={0};
int Position[2][800]={0};
int check_xmax[800]={0};
int check_xmin[800]={0};
int check_size[800]={0};
int push=0,pop=0,xmax=0,xmin=0,check_count=0,push_p=0;
int total_x,total_y;
float final_w,final_h,ang;
int total_black=0,total_white=0;

void sksVision_MainWindow::current_pos(const geometry_msgs::PoseStamped::ConstPtr& pos){
    cu_pos.pose.position.x = pos->pose.position.x;
    cu_pos.pose.position.y = pos->pose.position.y;
}

sksVision_MainWindow::sksVision_MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::sksVision_MainWindow)
{
    pub_m = p.advertise<geometry_msgs::Vector3>("/sks_vision",1);
//    position_sub= p.subscribe("/slam_out_pose",10,current_pos);
    ui->setupUi(this);
    startTimer(1);
}

sksVision_MainWindow::~sksVision_MainWindow()
{
    delete ui;
}

void sksVision_MainWindow::timerEvent(QTimerEvent *)
{
    cap >> cvframe;

    //Gray
//    cv::cvtColor(cvframe, cvGray, CV_RGB2GRAY);
//    cv::equalizeHist(cvGray,cvGray);
//    frame = Mat2QImage(cvGray);
    frame = MatToQImage(cvframe);
//    frame = func_Gray(frame);
    frame = func_DeepGray(frame);
//==============================================================
    int blacknum=0;
//    for(int h=0;h<frame.height();h+=2){
//        for(int w=0;w<frame.width();w+=2){
//            if(qRed(frame.pixel(w,h))<=64&&qRed(frame.pixel(w,h))>=27) blacknum++;//60,20
//        }
//    }
//   std::cout<<blacknum<<std::endl;
//===============================================================
    //Threshold_bar --> Threshold
    int gray;
    if(ui->checkbox_threshold->isChecked()){
        gray = ui->bar_threshold->value();
        ui->shownum_autoThreshold->setNum(0);
    }else{
        if(blacknum>=BLACK_BLOCK){
             gray=70;
             ui->shownum_autoThreshold->setNum(70);
         }else{
            gray = Average_Threshold(frame);
//            gray = Qtsu(frame);
//            if(cu_pos.pose.position.x<-1.07+0.2&&cu_pos.pose.position.x>-3.14-0.2&&
//               cu_pos.pose.position.y<3.87+0.2&&cu_pos.pose.position.y>1.13-0.2/*&& !goal_one_send*/)
            if(gray>40)
                 gray = 27;
            else gray = 55;

            ui->shownum_autoThreshold->setNum(gray);
        }
    }
    frame = func_Threshold(frame,gray);

    org_frame = frame;
    for(int dot=2;dot < frame.height()-2;dot+=(frame.height()/16)){
        for(int i=0;i<frame.width();i++){
            frame.setPixel(i,dot,QColor(0,255,0).rgb());
        }
    }
    //reset
    memset(Position,0,sizeof(Position)/sizeof(Position[0][0]));
    //Segmentation
    for(int h=2;h < frame.height()-2;h+=(frame.height()/16)){
        bw_sw = compareBlackWhite(org_frame,h);
        if(bw_sw==0){
            Segmentation1(org_frame,&frame,h,255);
        }else if(bw_sw==1){
            Segmentation1(org_frame,&frame,h,0);
        }
    }
    if(push_p>2){
        for(int i=0;i<push_p;i++){
            total_x += Position[0][i];
            total_y += Position[1][i];
        }
        double vector_x=total_x/push_p;
        double vector_y=total_y/push_p;
        double vector_h=frame.height()-1-vector_y;
        double vector_w=(ROBOT_CENTER-1-vector_x)/vector_h;

        for(int i=0;i<vector_h;i++){
            frame.setPixel(ROBOT_CENTER-1-vector_w*i,frame.height()-1-i,QColor(0,0,255).rgb());
        }

        final_w = vector_x - ROBOT_CENTER;

        final_h = frame.height() - 1 - vector_y;

        int sensor_x,sensor_y=100;
        sensor_x = sensorPoint(org_frame,&frame,sensor_y);
        final_w += sensor_x;
        if(sensor_x!=0) final_h += sensor_y;
        sensor_y=380;
        sensor_x = sensorPoint(org_frame,&frame,sensor_y);
        final_w += sensor_x;
        if(sensor_x!=0) final_h += sensor_y;

//        std::cout<<sensor_x<<std::endl;


        ang=atan(final_w/final_h);

        ui->shownum_avgVector->setText(tr("<font color=blue>(%1 : %2)</font>").arg(final_w).arg(final_h));
        ui->shownum_angular->setNum(ang/**180/3.14*/);

        frame.setPixel(vector_x-1,vector_y-1,QColor(0,0,255).rgb());
        frame.setPixel(vector_x-1,vector_y,QColor(0,0,255).rgb());
        frame.setPixel(vector_x-1,vector_y+1,QColor(0,0,255).rgb());
        frame.setPixel(vector_x,vector_y-1,QColor(0,0,255).rgb());
        frame.setPixel(vector_x,vector_y,QColor(0,0,255).rgb());
        frame.setPixel(vector_x,vector_y+1,QColor(0,0,255).rgb());
        frame.setPixel(vector_x+1,vector_y-1,QColor(0,0,255).rgb());
        frame.setPixel(vector_x+1,vector_y,QColor(0,0,255).rgb());
        frame.setPixel(vector_x+1,vector_y+1,QColor(0,0,255).rgb());
        vec3.z = ang;
    }else{
        ui->shownum_avgVector->setText(tr("<font color=blue>(%1 : %2)</font>").arg(999).arg(999));
        ui->shownum_angular->setNum(999);
        vec3.z = 999;
    }
    //reset
    total_x =0,total_y =0,push_p =0;//,h_max=0;//*/
    final_w =0,final_h =0;
    total_black=0;total_white=0;

    //ShowImg
    ui->showlabel->setPixmap(QPixmap::fromImage(frame));

    ui->shownum_linear->setNum(ui->bar_linear->value());
    ui->shownum_threshold->setNum(ui->bar_threshold->value());
    //ROS
    pub_m.publish(vec3);
    ros::Rate loop_rate(1000);
    loop_rate.sleep();
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
char compareBlackWhite(QImage img, int height){
//    int white_all=0,black_all=0;
//    for(int h=0;h<img.height();h++){
//        for(int w=0;w<img.width();w++){
//            if(qRed(img.pixel(w,h)==255)) white_all++;
//            else black_all++;
//        }
//    }
    int black=1,white=1;
    for(int w=0;w<img.width();w++){
        if(qRed(img.pixel(w,height))==255) white++;
        else    black++;
    }
    //if((white==img.width()+1) || (black==img.width()+1)) return 3;
    if(abs(white-img.width())<10 || abs(black-img.width())<10){
        return 3;
    }else if(white > black && white/black<=10){
        total_white++;
        return 1;
    }else if(black > white && black/white<=10){//white_all<black_all &&
        total_black++;
        return 0;
    }else{
        return 2;
    }
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
    
//    if(h_max<h) h_max=h;

    Position[0][push_p]=Xpos;
    Position[1][push_p]=h;
    push_p++;
}
void sksVision_MainWindow::Showimg(cv::Mat frame){
    QImage img(frame.cols,frame.rows,QImage::Format_RGB888);
    for(int i=0;i<frame.rows;i++){
        for(int j=0;j<frame.cols;j++){
            unsigned char color = frame.data[(i*frame.cols)+j];
            img.setPixel(j,i,QColor(color,color,color).rgb());
        }
    }
    ui->showlabel->setPixmap(QPixmap::fromImage(img));
}
int sksVision_MainWindow::sensorPoint(QImage oframe,QImage *frame,int height){
    int width_band=90;
    int avg_x=0,avg_y=0;
    for(int i=-3;i<=3;i++){
        if(i==0);
        else{
            frame->setPixel((ROBOT_CENTER)+(i*width_band)+0,height+0,QColor(0,0,255).rgb());
            frame->setPixel((ROBOT_CENTER)+(i*width_band)+1,height+0,QColor(0,0,255).rgb());
            frame->setPixel((ROBOT_CENTER)+(i*width_band)-1,height+0,QColor(0,0,255).rgb());
            frame->setPixel((ROBOT_CENTER)+(i*width_band)+0,height+1,QColor(0,0,255).rgb());
            frame->setPixel((ROBOT_CENTER)+(i*width_band)+0,height-1,QColor(0,0,255).rgb());
        }
    }
    for(int i=-3;i<=3;i++){
        if(i==0);
        else{
            if(total_white > total_black){

                if(qRed(oframe.pixel((ROBOT_CENTER)+(i*width_band),height)) == 0){
                    avg_x += width_band*i;
                    frame->setPixel((ROBOT_CENTER)+(i*width_band)+0,height+0,QColor(255,255,0).rgb());
                }
            }else if(total_black > total_white){
                if(qRed(oframe.pixel((ROBOT_CENTER)+(i*width_band),height)) == 255){
                    avg_x += width_band*i;
                    frame->setPixel((ROBOT_CENTER)+(i*width_band)+0,height+0,QColor(255,255,0).rgb());

                }
            }
        }
    }
    return avg_x;
}

void sksVision_MainWindow::on_Get_para_clicked()
{
    if(ui->checkbox_threshold->isChecked()){
        auto_check = 1;
    }else{
        auto_check = 0;
    }
    p.setParam("/SKS/vision/auto_gray",auto_check);
    p.setParam("/SKS/vision/gray_sum",ui->bar_threshold->value());
    p.setParam("/SKS/vision/linearr",ui->bar_linear->value());
}
