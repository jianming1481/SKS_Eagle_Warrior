#ifndef TESTNODE_MAINWINDOW_H
#define TESTNODE_MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <QObject>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <ros/ros.h>

namespace Ui {
class testnode_MainWindow;
}

class testnode_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit testnode_MainWindow(CvCapture *, QWidget *parent = 0);//------------------camera
//    explicit testNode_MainWindow(QWidget *parent = 0);
    ~testnode_MainWindow();

    QImage* IplImageToQImage(const IplImage *pIplImage);
    QImage MatToQImage(const cv::Mat& mat);

    void ros_pub();

//private Q_SLOTS:

protected:
    void timerEvent(QTimerEvent *e);
    void on_start_clicked();

private:
    Ui::testnode_MainWindow *ui;
    CvCapture *m_camera;//------------------camera
    ros::NodeHandle *n;
};

#endif // TESTNODE_MAINWINDOW_H
