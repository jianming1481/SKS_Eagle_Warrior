#ifndef SKSVISION_MAINWINDOW_H
#define SKSVISION_MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QImage>
#include <QObject>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include "func.hpp"
#include "qsks_vision.hpp"
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/PoseStamped.h>

#define PI 3.14159

namespace Ui {
class sksVision_MainWindow;
}

class sksVision_MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit sksVision_MainWindow(QWidget *parent = 0);
    ~sksVision_MainWindow();

    void Showimg(cv::Mat frame);
    int sensorPoint(QImage oframe, QImage *frame, int height);
    void current_pos(const geometry_msgs::PoseStamped::ConstPtr& pos);

protected:
    void timerEvent(QTimerEvent *e);
    ros::Publisher pub_m;
    ros::NodeHandle p;
    ros::Subscriber position_sub;

private:
    Ui::sksVision_MainWindow *ui;
};

#endif // SKSVISION_MAINWINDOW_H
