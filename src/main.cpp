#include <iostream>
#include <stdint.h>
#include <boost/thread.hpp>
#include "BlackLib.h"
#include "Motor.h"
#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"


using namespace std;
using namespace BlackLib;

void init_motors()
{
// -------------------------------------------------- Motor 1 parameters	
	M1->Position=0;
	M1->StepPosition=0;
	M1->PositionMax= 3.1416 / 2;
	M1->PositionMin= - 3.1416 / 2;
	M1->Direction=1;
	M1->Speed=0;
	M1->SpeedMax=0.5;
	M1->GearR=4.4;
	M1->Divisions=200;
	M1->MicroStepping=4;
	M1->PulsePin.setValue(low);
	M1->DirPin.setValue(low);
	M1->DirOffset=1;
// -------------------------------------------------- Motor 2 parameters	
	M2->Position=0;
	M2->StepPosition=0;
	M2->PositionMax=3.1416;
	M2->PositionMin=0;
	M2->Direction=1;
	M2->Speed=0;
	M2->SpeedMax=0.5;
	M2->GearR=4.4;
	M2->Divisions=200;
	M2->MicroStepping=4;
	M2->PulsePin.setValue(low);
	M2->DirPin.setValue(low);
	M2->DirOffset=-1;
// -------------------------------------------------- Motor 3 parameters	
	M3->Position=0;
	M3->StepPosition=0;
	M3->PositionMax= 3.1416 *3/2;
	M3->PositionMin=0;
	M3->Direction=1;
	M3->Speed=0;
	M3->SpeedMax=0.5;
	M3->GearR=6.0;
	M3->Divisions=200;
	M3->MicroStepping=4;
	M3->PulsePin.setValue(low);
	M3->DirPin.setValue(low);
	M3->DirOffset=1;
// -------------------------------------------------- Motor 4 parameters	
	M4->Position=0;
	M4->StepPosition=0;
	M4->PositionMax= 3.1416;
	M4->PositionMin= - 3.1416;
	M4->Direction=1;
	M4->Speed=0;
	M4->SpeedMax=0.5;
	M4->GearR=1;
	M4->Divisions=200;
	M4->MicroStepping=4;
	M4->PulsePin.setValue(low);
	M4->DirPin.setValue(low);
	M4->DirOffset=1;
// -------------------------------------------------- Motor 5 parameters	
	M5->Position=0;
	M5->StepPosition=0;
	M5->PositionMax= 3.1416 / 2;
	M5->PositionMin= -3.1416 / 2;
	M5->Direction=1;
	M5->Speed=0;
	M5->SpeedMax=0.5;
	M5->GearR=1;
	M5->Divisions=200;
	M5->MicroStepping=4;
	M5->PulsePin.setValue(low);
	M5->DirPin.setValue(low);
	M5->DirOffset=1;
}

/*
void M1_pos_control_call(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
	M1->setPos(msg->data[0],msg->data[1]);
}

void M2_pos_control_call(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
	M2->setPos(msg->data[0],msg->data[1]);
}

void M3_pos_control_call(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
	M3->setPos(msg->data[0],msg->data[1]);
}

void M4_pos_control_call(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
	M4->setPos(msg->data[0],msg->data[1]);
}

void M5_pos_control_call(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
	M5->setPos(msg->data[0],msg->data[1]);
}

*/
void pos_control_call(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
	boost::thread_group threads;
	threads.add_thread(new boost::thread(boost::bind(&Motor::setPos,M1,msg->data[0],msg->data[5])));
	threads.add_thread(new boost::thread(boost::bind(&Motor::setPos,M2,msg->data[1],msg->data[6])));
	threads.add_thread(new boost::thread(boost::bind(&Motor::setPos,M3,msg->data[2],msg->data[7])));
	threads.add_thread(new boost::thread(boost::bind(&Motor::setPos,M4,msg->data[3],msg->data[8])));
	threads.add_thread(new boost::thread(boost::bind(&Motor::setPos,M5,msg->data[4],msg->data[9])));

	threads.join_all();



}


//void Joint_state_pub(const ros::SingleSubscriberPublisher& joint_st)
void Joint_state_pub(const ros::Publisher& joint_st)
{

//	ros::Rate loop_rate(10);

	while(ros::ok())
	{
		std_msgs::Float32MultiArray msg;
		msg.data.resize(5);
		msg.data[0]=M1->Position;
		msg.data[1]=M2->Position;
		msg.data[2]=M3->Position;
		msg.data[3]=M4->Position;
		msg.data[4]=M5->Position;
		
		joint_st.publish(msg);
		ros::spinOnce();
		boost::this_thread::sleep( boost::posix_time::milliseconds(100) );
		//loop_rate.sleep();
	}
}

int main(int argc, char **argv)
{


	ros::init(argc, argv, "bender_node");
	ros::NodeHandle control_n, state_n;

	init_motors();
	ros::Subscriber sub=control_n.subscribe<std_msgs::Float32MultiArray>("pos_control", 1000, pos_control_call);

//	ros::Publisher  jnt_st=state_n.advertise<std_msgs::Float32MultiArray>("joint_state", 1000, (ros::SubscriberStatusCallback) Joint_state_pub);
	ros::Publisher  jnt_st=state_n.advertise<std_msgs::Float32MultiArray>("joint_state", 1000);

	boost::thread publisher_thread(&Joint_state_pub,jnt_st);
	publisher_thread.join();
	ros::spin();

	return 0;

}
