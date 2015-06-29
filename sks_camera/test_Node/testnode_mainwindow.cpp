#include "testnode_mainwindow.h"
#include "ui_testnode_mainwindow.h"

unsigned char gray_arr[256]={0};
unsigned char gtemp;
int threshold;
cv::Mat cvframe;
QImage sub_image(640,480,QImage::Format_RGB888);
QImage test_image("/home/iclab/Pictures/1.bmp");

QImage camera0,camera1;
cv::Mat cv00,cv01;

namespace enc = sensor_msgs::image_encodings;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    testNode_MainWindow *tM;
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
//        cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);
//        cvframe = cv_ptr->image;

//        cvframe = cv_bridge::toCvShare(msg, "bgr8")->image;

//        sub_image = MatToQImage(cvframe);
//        tM->ui1->sub_label->setPixmap(QPixmap::fromImage(test_image));
//        cv::waitKey(30);
    }
    catch (cv_bridge::Exception& e)
    {
         ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}
//testNode_MainWindow::testNode_MainWindow(QWidget *parent) :
testNode_MainWindow::testNode_MainWindow(CvCapture *cam, QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::testNode_MainWindow)
{
    ui->setupUi(this);
    m_camera = cam;
    m_nTimerId = startTimer(1);
}

testNode_MainWindow::~testNode_MainWindow()
{
    delete ui;
}

void testNode_MainWindow::timerEvent(QTimerEvent *)
{
    if(ui->tabWidget->currentIndex()==0){
        IplImage* showCamera = cvQueryFrame(m_camera);
        QImage *frame = IplImageToQImage(showCamera);

        ui->threshold_num->setNum(ui->threshold_bar->value());

        *frame = func_Gray(*frame);

        if(ui->check_Threshold->isChecked()){
            threshold = ui->threshold_bar->value();
        }else{
            threshold = Average_Threshold(*frame);
        }

        for(int h=0;h<frame->height();h++){
            for(int w=0;w<frame->width();w++){
                if(qRed(frame->pixel(w,h))<=threshold){
                    frame->setPixel(w,h,QColor(0,0,0).rgb());
                }else{
                    frame->setPixel(w,h,QColor(255,255,255).rgb());
                }
            }
        }
        ui->show_camera->setPixmap(QPixmap::fromImage(*frame));
    }else if(ui->tabWidget->currentIndex()==1){
        ros::NodeHandle n;
        image_transport::ImageTransport it(n);
        image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);
//        sub1 = it1->subscribe("/camera/image",1,imageCallback);

//        sub_image = MatToQImage(cvframe);
//        ui->sub_label->setPixmap(QPixmap::fromImage(sub_image));
        ros::spin();
        killTimer(m_nTimerId);
    }else if(ui->tabWidget->currentIndex()==2){
        cv::VideoCapture cap(201);
        cap>>cv00;
        cv::VideoCapture cap1(202);
        cap1>>cv01;

//        IplImage* showCamera = cvQueryFrame(m_camera);
//        QImage *camera2 = IplImageToQImage(showCamera);

        camera0 = MatToQImage(cv00);
        camera1 = MatToQImage(cv01);

        ui->camera01->setPixmap(QPixmap::fromImage(camera0));
        ui->camera02->setPixmap(QPixmap::fromImage(camera1));
    }
}

QImage* testNode_MainWindow::IplImageToQImage(const IplImage *pIplImage)
{
    QImage *qImage;

    int w = pIplImage->width;
    int h = pIplImage->height;

    qImage = new QImage(w, h, QImage::Format_RGB32);

    int x, y;
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
