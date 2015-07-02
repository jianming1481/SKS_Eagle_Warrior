#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> // for converting the command line parameter to integer
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define COUNTER 6
#define COMPRESSED_WIDTH 120
#define COMPRESSED_HEIGHT 90

using namespace std;
int camera_num,random_num;
char node_name[30];
void colorReduce8(cv::Mat &image, int div);
void colorReduce3(cv::Mat &image, int div);
void colorReduce(cv::Mat& image, int div);

cv::Mat frame;
sensor_msgs::ImagePtr msg;

int main(int argc, char** argv)
{
    //Random for creating Nodes
    srand(time(NULL));
    random_num = (rand()%100+1);
    sprintf(node_name,"image_publisher%d",random_num);
    ros::init(argc, argv, node_name);
    ros::NodeHandle nh;
    ros::Rate loop_rate(30);
    ros::spinOnce();
    image_transport::ImageTransport it(nh);

    // Check if video source has been passed as a parameter
    if(argv[1] == NULL){
        image_transport::Publisher pub = it.advertise("camera/image", COUNTER);

        cv::VideoCapture cap(200);
        cap.set(CV_CAP_PROP_FRAME_WIDTH,COMPRESSED_WIDTH);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT,COMPRESSED_HEIGHT);
        // Check if video device can be opened with the given index
        if(!cap.isOpened()){
            std::cout<<"camera isnt opened"<<std::endl;
            return 1;
        }
        while (nh.ok()) {
            cap >> frame;
            // Check if grabbed frame is actually full with some content
            if(!frame.empty()) {
                msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
                pub.publish(msg);
                cv::waitKey(1);
            }
            loop_rate.sleep();
        }
    }else if(argc==3){
        image_transport::Publisher pub = it.advertise(argv[2], COUNTER);

        camera_num = atoi(argv[1]);
        cv::VideoCapture cap(camera_num);
        cap.set(CV_CAP_PROP_FRAME_WIDTH,COMPRESSED_WIDTH);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT,COMPRESSED_HEIGHT);
        // Check if video device can be opened with the given index
        if(!cap.isOpened()){
            std::cout<<"camera isnt opened"<<std::endl;
            return 1;
        }
        while (nh.ok()) {
            cap >> frame;
            // Check if grabbed frame is actually full with some content
            if(!frame.empty()) {
                msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
                pub.publish(msg);
                cv::waitKey(1);
            }
            loop_rate.sleep();
        }
    }else if(argc==4){
        image_transport::Publisher pub = it.advertise(argv[2], COUNTER);
        camera_num = atoi(argv[1]);
        cv::VideoCapture cap(camera_num);
        cap.set(CV_CAP_PROP_FRAME_WIDTH,COMPRESSED_WIDTH);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT,COMPRESSED_HEIGHT);
        // Check if video device can be opened with the given index
        if(!cap.isOpened()){
        std::cout<<"camera isnt opened"<<std::endl;
        return 1;
        }
        while (nh.ok()) {
            cap >> frame;
            switch (atoi(argv[3])){
                case 0:colorReduce(frame,64);
                case 1:colorReduce3(frame,64);
                case 2:colorReduce8(frame,64);
            }
            // Check if grabbed frame is actually full with some content
            if(!frame.empty()) {
                msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
                pub.publish(msg);
                cv::waitKey(1);
            }
            loop_rate.sleep();
        }
    }else{
        ROS_INFO("\n\nPlease run:");
        ROS_INFO("\n$ rosrun workspace my_publisher camera_num ros_topic\n");
        ROS_INFO("\n$ rosrun workspace my_publisher camera_num ros_topic compress_number\n\n");
        ROS_INFO("//***NOTE***\n");
        ROS_INFO("//the first camera's num is 200,second one is 201,...etc\n");
        ROS_INFO("//compress_number: 0~2\n\n");
    }
}

void colorReduce(cv::Mat& image, int div=64)
{
    int nl = image.rows; // number of lines
    int nc = image.cols * image.channels(); // number of elements per line
    for (int j = 0; j < nl; j++)
    {
        // get the address of row j
        uchar* data = image.ptr<uchar>(j);
        for (int i = 0; i < nc; i++)
        {
            // process each pixel
           data[i] = data[i] / div * div + div / 2;
        }
    }
}
void colorReduce3(cv::Mat &image, int div = 64)
{
    int nl = image.rows;
    int nc = image.cols * image.channels();
    int n = static_cast<int>(log(static_cast<double>(div))/log(2.0)); //div=64, n=6
    uchar mask = 0xFF<<n; //e.g. div=64, mask=0xC0
    //遍历图像的每个像素
    for(int j=0; j<nl ;++j)
    {
        uchar *data = image.ptr<uchar>(j);
        for(int i=0; i<nc; ++i)
        {
            *data++ = *data&mask + div/2;
        }
    }
}
//减少循环次数，进行图像像素遍历，调用函数较少，效率最高。
void colorReduce8(cv::Mat &image, int div = 64)
{
    int nl = image.rows;
    int nc = image.cols;
    //判断是否是连续图像，即是否有像素填充
    if(image.isContinuous())
    {
        nc = nc*nl;
        nl = 1;
    }
    int n = static_cast<int>(log(static_cast<double>(div))/log(2.0));
    uchar mask = 0xFF<<n;
    //遍历图像的每个像素
    for(int j=0; j<nl ;++j)
    {
        uchar *data = image.ptr<uchar>(j);
        for(int i=0; i<nc; ++i)
        {
            *data++ = *data & mask +div/2;
            *data++ = *data & mask +div/2;
            *data++ = *data & mask +div/2;
        }
    }
}
