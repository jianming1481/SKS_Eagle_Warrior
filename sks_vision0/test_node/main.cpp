#include "testnode_mainwindow.h"
#include <QApplication>
using namespace cv;
int main(int argc, char *argv[])
{
    ros::init(argc,argv,"test_node");
    ros::spinOnce();
    //------------------camera
    CvCapture *camera = cvCaptureFromCAM( -1 );
    assert(camera);
    IplImage *image = cvQueryFrame(camera);
    assert(image);

    QApplication a(argc, argv);
    testnode_MainWindow w(camera);
//    testNode_MainWindow w;
    w.show();

    return a.exec();
}
