#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <geometry_msgs/Vector3.h>
#include <std_msgs/Char.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Twist.h>
#include <math.h>
#define target_one_x  1.21 //1.0
#define target_one_y  5.11 //-5.48
#define target_two_x  -0.36
#define target_two_y  2.78
#define goal_one_x    0.51
#define goal_one_y    2.47
#define cancel_one_x  0.51
#define cancel_one_y  2.27
#define goal_two_x    -1.5
#define goal_two_y    5.11
#define cancel_two_x  -1.3
#define cancel_two_y  5.11
#define swap_area_x   0.51
#define swap_area_y   2.67
#define forward_speed 20
double last_speed_angular;

//float posx,posy;
geometry_msgs::Twist sp;
geometry_msgs::Twist sp2;
geometry_msgs::PoseStamped cu_pos;
void current_pos(const geometry_msgs::PoseStamped::ConstPtr& pos){
	cu_pos.pose.position.x = pos->pose.position.x;
	cu_pos.pose.position.y = pos->pose.position.y;
}
void vec2speed(const geometry_msgs::Vector3::ConstPtr& vec){	
	sp.angular.z = vec->z;	
}
int main(int argc, char** argv){
	ros::init(argc, argv, "goal_manager");
	ros::NodeHandle n;
	
	ros::Subscriber position_sub = n.subscribe("/slam_out_pose",10,current_pos);
	ros::Subscriber vec_sub      = n.subscribe("/sks_vision",1,vec2speed);
	ros::Publisher  arm_command  = n.advertise<std_msgs::Char>("/cmd_pos",1);
	ros::Publisher  speed_com    = n.advertise<geometry_msgs::Twist>("/cmd_vel",10);

	std_msgs::Char  com;
	
	actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> Client("move_base", true);
	Client.waitForServer(); //will wait for infinite time
	move_base_msgs::MoveBaseGoal goal;
	goal.target_pose.header.frame_id = "map";
	bool goal_one_send = false,goal_two_send = false,goal_one_cancel = false,goal_two_cancel = false,swap_detect = false,startup=false;
	
	ros::Rate rate(1000);
	
	while(ros::ok()){
		if(cu_pos.pose.position.x<0.5&&cu_pos.pose.position.y<0.5&& !startup){
			//ros::Duration(0.5).sleep();
			ROS_INFO("startup");
			sp2.linear.x  = 0.32;
			sp2.angular.z =	-0.4;			
			speed_com.publish(sp2);
			ros::Duration(1.0).sleep();		
			startup=true;
		}
		if( cu_pos.pose.position.x>-1.3 && cu_pos.pose.position.x<1.4 &&
	   	    cu_pos.pose.position.y>2.47 && cu_pos.pose.position.y<5.21){
			if((cu_pos.pose.position.x<target_one_x+0.2&&cu_pos.pose.position.x>target_one_x-0.2)&&
	   		   (cu_pos.pose.position.y<target_one_y+0.2&&cu_pos.pose.position.y>target_one_y-0.2)&& !goal_one_send){
				ROS_INFO("arrived first target, sending first goal...");
				sp2.linear.x  = 0.30;
				sp2.angular.z =	0.0;			
				speed_com.publish(sp2);
				ros::Duration(1.0).sleep();
				sp2.linear.x  = 0.0;
				sp2.angular.z =	0.0;			
				speed_com.publish(sp2);
				ros::Duration(1.0).sleep();
				goal.target_pose.pose.position.x = goal_one_x;
				goal.target_pose.pose.position.y = goal_one_y;	
				goal.target_pose.pose.position.z = 0.0;
				goal.target_pose.pose.orientation.x = 0.0;
				goal.target_pose.pose.orientation.y = 0.0;
				goal.target_pose.pose.orientation.z = 0.0;
				goal.target_pose.pose.orientation.w = 2.57;
				ros::Duration(1.0).sleep(); //sleep for one sec
				Client.sendGoal(goal);
				goal_one_send = true;
			}
			if((cu_pos.pose.position.x<target_two_x+0.2&&cu_pos.pose.position.x>target_two_x-0.2)&&
	   		   (cu_pos.pose.position.y<target_two_y+0.2&&cu_pos.pose.position.y>target_two_y-0.2)&& !goal_two_send){
				ROS_INFO("arrived second target, sending second goal...");
				sp2.linear.x  = 0.30;
				sp2.angular.z =	0.0;			
				speed_com.publish(sp2);
				ros::Duration(1.0).sleep();
				sp2.linear.x  = 0.0;
				sp2.angular.z =	0.0;			
				speed_com.publish(sp2);
				ros::Duration(1.0).sleep();
				goal.target_pose.pose.position.x = goal_two_x;
				goal.target_pose.pose.position.y = goal_two_y;	
				goal.target_pose.pose.position.z = 0.0;
				goal.target_pose.pose.orientation.x = 0.0;
				goal.target_pose.pose.orientation.y = 0.0;
				goal.target_pose.pose.orientation.z = 0.0;
				goal.target_pose.pose.orientation.w = 4.14;
				ros::Duration(1.0).sleep(); //sleep for one sec
				Client.sendGoal(goal);
				goal_two_send = true;
			}
			if((cu_pos.pose.position.x<cancel_one_x+0.2&&cu_pos.pose.position.x>cancel_one_x-0.2)&&
	   		   (cu_pos.pose.position.y<cancel_one_y+0.2&&cu_pos.pose.position.y>cancel_one_y-0.2)&& !goal_one_cancel){
				ROS_INFO("arrived first goal!");
				Client.cancelAllGoals();
				goal_one_cancel = true;
			}
			if((cu_pos.pose.position.x<cancel_two_x+0.1&&cu_pos.pose.position.x>cancel_two_x-0.1)&&
	   		   (cu_pos.pose.position.y<cancel_two_y+0.1&&cu_pos.pose.position.y>cancel_two_y-0.1)&& !goal_two_cancel){
				ROS_INFO("arrived second goal!");
				Client.cancelAllGoals();
				goal_two_cancel = true;		
			}
			if((cu_pos.pose.position.x<target_one_x+0.1&&cu_pos.pose.position.x>target_one_x-0.1)&&
	   		   (cu_pos.pose.position.y<target_one_y+0.1&&cu_pos.pose.position.y>target_one_y-0.1)&& !swap_detect){
            			ROS_INFO("swap behavior actived...");
				com.data = 'g';
				arm_command.publish(com);
				swap_detect=true;
			}
		}else{			
			
			//ROS_INFO("line!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			if(sp.angular.z<0.15&&sp.angular.z>-0.15){	
				sp2.linear.x  = 1.2;
				sp2.angular.z =	sp.angular.z;				
				speed_com.publish(sp2);
			}else if((sp.angular.z>0.15&&sp.angular.z<0.25)||(sp.angular.z<-0.15&&sp.angular.z>-0.25)){
				sp2.linear.x  = 1;
				sp2.angular.z =	sp.angular.z;				
				speed_com.publish(sp2);
			}else if((sp.angular.z>0.25&&sp.angular.z<0.35)||(sp.angular.z<-0.25&&sp.angular.z>-0.35)){
				sp2.linear.x  = 0.9;
				sp2.angular.z =	sp.angular.z;				
				speed_com.publish(sp2);
			}else if((sp.angular.z>0.35&&sp.angular.z<0.5)||(sp.angular.z<-0.35&&sp.angular.z>-0.5)){
				sp2.linear.x  = 0.8;
				sp2.angular.z =	sp.angular.z;				
				speed_com.publish(sp2);
			}else if((sp.angular.z>0.5&&sp.angular.z<0.65)||(sp.angular.z<-0.5&&sp.angular.z>-0.65)){
				sp2.linear.x  = 0.65;
				sp2.angular.z =	sp.angular.z;				
				speed_com.publish(sp2);
			}else if((sp.angular.z>0.65&&sp.angular.z<0.8)||(sp.angular.z<-0.65&&sp.angular.z>-0.8)){
				sp2.linear.x  = 0.45;
				sp2.angular.z =	sp.angular.z;				
				speed_com.publish(sp2);
			}else if(sp.angular.z==999){		 
				sp2.linear.x  = 0.10;
				sp2.angular.z = last_speed_angular;
				speed_com.publish(sp2);
			
			}else{
				sp2.linear.x  = 0;
				sp2.angular.z =	sp.angular.z;				
				speed_com.publish(sp2);	
			}
			if(sp.angular.z != 999)last_speed_angular = sp.angular.z;
		}
		ros::spinOnce();
		rate.sleep();
	}
	return 0;
}
