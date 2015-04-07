#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include <boost/thread.hpp>
#include "Motor.h"
#include "BlackLib.h"
#include "ros/ros.h"

using namespace std;
using namespace BlackLib;

bool Motor::CheckLimits(float pos, float speed)
{
	bool check=false;
	if(pos <= PositionMax && pos >= PositionMin && speed <= SpeedMax && speed > 0)
		check=true;

	return check;
}


void Motor::Pdelay()
{
	float StepsPerSec = Divisions * MicroStepping * GearR * Speed;
	float time = 1000000/StepsPerSec;
//	usleep(time);
	boost::this_thread::sleep( boost::posix_time::microseconds(time) );
}

void Motor::setPos(float pos, float speed)
{
	if(CheckLimits(pos,speed))
	{
		Speed=speed;
		setDir(pos);

		int StepDiff=(pos-Position)/(2*3.1416) * (Divisions * MicroStepping * GearR);
		//cout<<"Stepdiff: "<<StepDiff<<" Position: "<<Position<<" pos: "<<pos<<endl;
		for(int i=0; i < abs(StepDiff); i++)
		{
			Pulse();
			Pdelay();
			if(StepDiff>0)
			{
				StepPosition++;
				Position = StepPosition * (2*3.1416) / (Divisions * MicroStepping * GearR);
				//cout<< "Position: " << Position << endl;
			}
			else
			{
				StepPosition--;
				Position = StepPosition * (2*3.1416) / (Divisions * MicroStepping * GearR);
			}

			ros::spinOnce();
		}

	}
	cout<< "Position: " << Position << endl;
	
	boost::this_thread::yield();

}

void Motor::test(){

	cout<<"Test Function";
}

void Motor::setDir(float pos)
{
	float change =(pos-Position)*DirOffset;

	if (change >= 0) {
		DirPin.setValue(high);
		cout << "Positive Direction check: " << std::boolalpha << DirPin.isHigh() << endl;	}
	else {
		DirPin.setValue(low);
		cout << "Negative Direction check: " << std::boolalpha << !DirPin.isHigh() << endl;	}
}


void Motor::Pulse()
{
	PulsePin.setValue(high);
//	cout << "Pulse ON check: " << std::boolalpha << PulsePin.isHigh() << endl;
	PulsePin.setValue(low);
//	cout << "Pulse OFF check: " << std::boolalpha << !PulsePin.isHigh() << endl;
}



Motor::~Motor()
{
}

BlackLib::BlackGPIO pulse1 = BlackGPIO(BlackLib::GPIO_47, BlackLib::output, BlackLib::FastMode);
BlackLib::BlackGPIO dir1 = BlackGPIO(BlackLib::GPIO_46, BlackLib::output, BlackLib::FastMode);

BlackLib::BlackGPIO pulse2 = BlackGPIO(BlackLib::GPIO_23, BlackLib::output, BlackLib::FastMode);
BlackLib::BlackGPIO dir2 = BlackGPIO(BlackLib::GPIO_26, BlackLib::output, BlackLib::FastMode);

BlackLib::BlackGPIO pulse3  = BlackGPIO(BlackLib::GPIO_45, BlackLib::output, BlackLib::FastMode);
BlackLib::BlackGPIO dir3 = BlackGPIO(BlackLib::GPIO_44, BlackLib::output, BlackLib::FastMode);

BlackLib::BlackGPIO pulse4 = BlackGPIO(BlackLib::GPIO_69, BlackLib::output, BlackLib::FastMode);
BlackLib::BlackGPIO dir4 = BlackGPIO(BlackLib::GPIO_68, BlackLib::output, BlackLib::FastMode);

BlackLib::BlackGPIO pulse5 = BlackGPIO(BlackLib::GPIO_66, BlackLib::output, BlackLib::FastMode);
BlackLib::BlackGPIO dir5 = BlackGPIO(BlackLib::GPIO_67, BlackLib::output, BlackLib::FastMode);


Motor *M1 = new Motor(&pulse1,&dir1);
Motor *M2 = new Motor(&pulse2,&dir2);
Motor *M3 = new Motor(&pulse3,&dir3);
Motor *M4 = new Motor(&pulse4,&dir4);
Motor *M5 = new Motor(&pulse5,&dir5);



