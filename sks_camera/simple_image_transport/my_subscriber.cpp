#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <sensor_msgs/image_encodings.h>

using namespace cv;
using namespace std;

static const char OPENCV_WINDOW[] = "Image window";

cv::Mat test;
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  cv_bridge::CvImagePtr cv_ptr;//new
  try
  {
//    cv::imshow(OPENCV_WINDOW, cv_bridge::toCvShare(msg, "bgr8")->image);
    cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
  test = cv_ptr->image;
//  test = cv_bridge::toCvShare(msg,"bgr8")->image;
  IplImage* img = cvGetImage(&test, cvCreateImageHeader(test.size(), test.depth(), test.channels()));
  cvShowImage("view",img);
  cv::waitKey(1);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
//  cv::namedWindow(OPENCV_WINDOW);
  cvNamedWindow("view",0);
  cv::startWindowThread();

  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub;
  sub = it.subscribe("camera/image", 1, imageCallback);

  ros::spin();
//  cv::destroyWindow(OPENCV_WINDOW);
//  cvDestroyWindow("view");
}//*/

//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------

/*
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"//*
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio.hpp"

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <time.h>
#include <ctime>
#include <fstream>

#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QMessageBox>
#include <QGraphicsView>
#include <QStackedWidget>
#include <QGraphicsScene>
#include <QDataStream>

#include <ros/ros.h>
#include <std_msgs/String.h>

using namespace std;
using namespace cv;

namespace enc = sensor_msgs::image_encodings;

static const char WINDOW[] = "Image window";

#define Frame_Cols 659
#define Frame_Rows 493

class ImageConverter
{
  cv::Mat *BLUE;

  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
//  image_transport::Publisher image_pub_;

  ros::NodeHandle *n;
  ros::Publisher chatter_pub;

public:
  ImageConverter()
    : it_(nh_)
  {
      BLUE = new cv::Mat(cv::Size(Frame_Cols,Frame_Rows),CV_8UC3);

      n = new ros::NodeHandle;
//    chatter_pub = n->advertise<vision::Info>(Vision_Segmentation_Topic,1);
//    image_pub_ = it_.advertise("/Image_pkg/Show_image", 1);
    image_sub_ = it_.subscribe("/camera/image", 1, &ImageConverter::imageCb, this);

    //cv::namedWindow(WINDOW);
  }

  ~ImageConverter()
  {
    //cv::destroyWindow(WINDOW);
  }

void imageCb(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    //ros::Rate loop_rate(10);

    cv::Mat frame = cv_ptr->image;

//    cv::imshow(WINDOW, *BLUE);
//    IplImage* img = cvGetImage(&frame, cvCreateImageHeader(frame.size(), frame.depth(), frame.channels()));
//    cvShowImage("view",img);
//    cv::waitKey();
     }
};

int main( int argc, char *argv[] )
{
  ros::init(argc, argv, "my_subscriber");

  ImageConverter ic;
  ros::spin();

  return 0;
}
//*/
