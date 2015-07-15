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

protected:
    void timerEvent(QTimerEvent *e);
    ros::Publisher pub_m;
    ros::NodeHandle p;

private Q_SLOTS:
    void on_Get_para_clicked();


private:
    Ui::sksVision_MainWindow *ui;
    int auto_check;
};

#endif // SKSVISION_MAINWINDOW_H
