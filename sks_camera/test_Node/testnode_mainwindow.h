#ifndef TESTNODE_MAINWINDOW_H
#define TESTNODE_MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <opencv2/highgui.hpp>
#include <opencv/cv.h>
#include <iostream>
#include "func.h"

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

namespace Ui {
class testNode_MainWindow;
}

class testNode_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit testNode_MainWindow(CvCapture *, QWidget *parent = 0);//------------------camera
//    explicit testNode_MainWindow(QWidget *parent = 0);
    ~testNode_MainWindow();

    QImage* IplImageToQImage(const IplImage *pIplImage);

//    void imageCallback(const sensor_msgs::ImageConstPtr& msg);
    Ui::testNode_MainWindow *ui1;

protected:
    void timerEvent(QTimerEvent *e);
    int m_nTimerId;

private:
    Ui::testNode_MainWindow *ui;
    CvCapture *m_camera;//------------------camera

//    ros::NodeHandle n1;
//    image_transport::ImageTransport *it1;
//    image_transport::Subscriber *sub1;
};

#endif // TESTNODE_MAINWINDOW_H
