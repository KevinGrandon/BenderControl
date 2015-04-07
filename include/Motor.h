#pragma once
#include <iostream>
#include <stdint.h>
#include "BlackLib.h"

using namespace std;
using namespace BlackLib;

class Motor{
	public:
		
		float Position;
		float PositionMax;
		float PositionMin;
		int Direction;
		float Speed;
		float SpeedMax;
		float GearR;
		int MicroStepping;
		int DirOffset;
		int Divisions;
		int StepPosition;
		void setPos(float , float);
//		bool posControl(float , float);
		void test();
		BlackGPIO& PulsePin;
		BlackGPIO& DirPin;

		Motor(BlackLib::BlackGPIO* PulsePin ,BlackLib::BlackGPIO* DirPin) : PulsePin(*PulsePin) , DirPin(*DirPin)
		{
		}
		~Motor();

//	private:
		void Pulse();
		void setDir(float);
		bool CheckLimits(float , float);
		void Pdelay();

};


extern Motor *M1,*M2,*M3,*M4,*M5;
