#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> // for converting the command line parameter to integer
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace std;
int camera_num,random_num;
char node_name[30];

int main(int argc, char** argv)
{
    //Random for creating Nodes
    srand(time(NULL));
    random_num = (rand()%100+1);
    // Check if video source has been passed as a parameter
    if(argv[1] == NULL){// return 1;
        ros::init(argc, argv, "image_publisher0");
        ros::NodeHandle nh;
        image_transport::ImageTransport it(nh);
        image_transport::Publisher pub = it.advertise("camera/image", 1);

        // Convert the passed as command line parameter index for the video device to an integer
        //  std::istringstream video_sourceCmd(argv[1]);
        //  int video_source;
        //  // Check if it is indeed a number
        //  if(!(video_sourceCmd >> video_source)) return 1;

        //  cv::VideoCapture cap(video_source);
        cv::VideoCapture cap(200);
        // Check if video device can be opened with the given index
        if(!cap.isOpened()){
            std::cout<<"camera isnt opened"<<std::endl;
            return 1;
        }
        cv::Mat frame;
        sensor_msgs::ImagePtr msg;

        ros::Rate loop_rate(10000);
        while (nh.ok()) {
            cap >> frame;
            // Check if grabbed frame is actually full with some content
            if(!frame.empty()) {
                msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
                pub.publish(msg);
                cv::waitKey(1);
            }

            ros::spinOnce();
            loop_rate.sleep();
        }
    }else if(argc==3){
        sprintf(node_name,"image_publisher%d",random_num);
        ros::init(argc, argv, node_name);
        ros::NodeHandle nh;
        image_transport::ImageTransport it(nh);
        image_transport::Publisher pub = it.advertise(argv[2], 1);

        camera_num = atoi(argv[1]);
        cv::VideoCapture cap(camera_num);
        // Check if video device can be opened with the given index
        if(!cap.isOpened()){
            std::cout<<"camera isnt opened"<<std::endl;
            return 1;
        }
        cv::Mat frame;
        sensor_msgs::ImagePtr msg;

        ros::Rate loop_rate(10000);
        while (nh.ok()) {
            cap >> frame;
            // Check if grabbed frame is actually full with some content
            if(!frame.empty()) {
                msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
                pub.publish(msg);
                cv::waitKey(1);
            }
            ros::spinOnce();
            loop_rate.sleep();
        }
    }else{
        cout<<"\n\nPlease run:";
        cout<<"\n$ rosrun workspace my_publisher camera's_num ros_topic\n\n";
        cout<<"//***NOTE***"<<endl;
        cout<<"//the first camera's num is 200,second one is 201,...etc"<<endl<<endl;
    }
}
