#include "testnode_mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "test_Node");
    //------------------camera
    CvCapture *camera = cvCaptureFromCAM( 200 );
    assert(camera);
    IplImage *image = cvQueryFrame(camera);
    assert(image);

    QApplication a(argc, argv);
    testNode_MainWindow w(camera);
//    testNode_MainWindow w;
    w.show();

    return a.exec();
}
