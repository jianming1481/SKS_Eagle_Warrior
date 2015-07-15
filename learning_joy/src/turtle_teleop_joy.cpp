#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Char.h>
#include <math.h>
#include "std_msgs/String.h"
#include <sound_play.h>

class TeleopTurtle
{
public:
  TeleopTurtle();

private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
  
  ros::NodeHandle nh_;

  double linear_, angular_, setsp, speed, rotate;
  int l_scale_, a_scale_;
  ros::Publisher vel_pub_;
  ros::Publisher arm_pub_;
  ros::Subscriber joy_sub_;
  sound_play::SoundClient sc;
};


TeleopTurtle::TeleopTurtle():
  angular_(0),
  linear_(1)
{

  nh_.param("axis_linear", linear_, linear_);
  nh_.param("axis_angular", angular_, angular_);
  nh_.param("scale_angular", a_scale_, a_scale_);
  nh_.param("scale_linear", l_scale_, l_scale_);

  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  arm_pub_ = nh_.advertise<std_msgs::Char>("/cmd_pos",1);
  //arm_pub_ = nh_.advertise<std_msgs::Char>("/arm",1);

  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &TeleopTurtle::joyCallback, this);

}

void TeleopTurtle::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  geometry_msgs::Twist vel;
  std_msgs::Char arm;
  /********************************************************************
   robot motion
 *********************************************************************/
  double x=joy->axes[angular_];
  double y=joy->axes[linear_];
  rotate = 0;
//setsp = (X^2+Y^2)^(1/2) ->maxinum=60
  setsp = (sqrt(pow(x,2)+pow(y,2)));
  if(setsp>1)setsp=1;

  if(joy->axes[linear_]>0.2){		//axes Y > 0.2
	if(joy->axes[angular_]>0.2){		////axes X > 0.2 -> turn left
		rotate=-(joy->axes[angular_]);
		speed=setsp;
	}else if(joy->axes[angular_]<-0.2){	////axes X < -0.2 -> turn right
		rotate=(-joy->axes[angular_]);
		speed=setsp;
	}else{					////go forward
		speed=setsp;
	}
  }else if(joy->axes[linear_]<-0.2){	//axes Y < -0.2
	setsp = -setsp;
  	if(joy->axes[angular_]>0.2){	//axes X > 0.2 ->left back 
		rotate=joy->axes[angular_];
		speed = setsp;
	}else if(joy->axes[angular_]<-0.2){	//axes X <-0.2 ->right back
		rotate=joy->axes[angular_];
		speed = setsp;
	}else{				//go backward
		speed = setsp;
	}
  }else if(joy->axes[angular_]>0.2){	//x > 0.2 ->turn left
	rotate = -1;
	speed = setsp;
  }else if(joy->axes[angular_]<-0.2){	//X < -0.2 ->turn right
	rotate = 1;
	speed = setsp;
  }else{				//stop
  	speed = 0;
	rotate = 0;
  }
  if(joy->buttons[4]==1){
	speed= 0;	//button LB == 1 ->rotate left
	rotate =-1;
  }else if(joy->buttons[5]==1){			
	speed= 0;	//button RB == 1 ->rotate right
	rotate = 1;
  }
  vel.linear.x=speed;
  vel.angular.z=rotate;
  vel_pub_.publish(vel);
  /*******************************************************************
   arm motion        
*********************************************************************/
  double up = joy->buttons[3];
  double r_left = joy->buttons[2];
  double r_right = joy->buttons[1];
  double down = joy->buttons[0];
  double grip_close = joy->axes[2];
  double grip_open = joy->axes[5];
  double sound = joy->buttons[7];
  int ls_up=0,ls_down=0,ls_r_left=0,ls_r_right=0,ls_grip_close=0,ls_grip_open=0;
  if(sound ==1 ){
      sc.playWave("/home/iclab/sks_ws/src/audioclip.wav");
  }
  if(joy->axes[2] == -1) grip_close=1;
  else grip_close=0;
  if(joy->axes[5] == -1) grip_open=1;
  else grip_open=0;
  if(up==1 && up!=ls_up){                    //button Y ==1 -> up ->publish(w)
    arm.data='w';
    arm_pub_.publish(arm);
  }else if(down==1 && down!=ls_down){		//button A == 1 -> down ->->publish(s)
    arm.data='s';
    arm_pub_.publish(arm);
  }else if(r_left==1 && r_left!=ls_r_left){      	//button X ==1 ->rotate left ->->publish(a)
    arm.data='a';
    arm_pub_.publish(arm);
  }else if(r_right==1 && r_right!=ls_r_right){    	//button B ==1 ->rotate right ->->publish(d)
    arm.data='d';
    arm_pub_.publish(arm);
  }else if(grip_close==1 && (grip_close!=ls_grip_close)){	//LT !=1 ->grip close ->->publish(o)
    arm.data='o';
    arm_pub_.publish(arm);
  }else if(grip_open==1 && (grip_open!=ls_grip_open)){	//RT !=1 ->grip open ->->publish(p)
    arm.data='p';
    arm_pub_.publish(arm);
  }else{
    arm.data='0';
  }
  ls_up=up;
  ls_down=down;
  ls_r_left=r_left;
  ls_r_right=r_right;
  ls_grip_close=grip_close;
  ls_grip_open=grip_open;
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "teleop_turtle");
  TeleopTurtle teleop_turtle;

  ros::spin();
}
