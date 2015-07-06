#include <QApplication>
#include "sksvision_mainwindow.hpp"
#include "qsks_vision.hpp"
#include <geometry_msgs/Vector3.h>

int main(int argc, char *argv[])
{
    ros::init(argc,argv,"sks_vision");

    QApplication a(argc, argv);
    sksVision_MainWindow w;
    w.show();

    return a.exec();
}
