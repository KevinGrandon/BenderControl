#include <iostream>
#include <stdint.h>
#include <unistd.h>
#include "BlackLib.h"

using namespace std;
using namespace BlackLib;


int main()
{

	BlackGPIO pulsePin(GPIO_30, output, SecureMode);
	BlackGPIO dirPin(GPIO_60, output, SecureMode);
	while(true)
	{

		if( pulsePin.setValue(BlackLib::high) )
		{
			std::cout << "Gpio 30 set high successfully" << std::endl;
		}
		else
		{
			std::cout << "Gpio 30 couldn't set." << std::endl;
		}

		usleep(10000);

		if( pulsePin.setValue(BlackLib::low) )
		{
			std::cout << "Gpio 30 set low successfully" << std::endl;
		}
		else
		{
			std::cout << "Gpio 30 couldn't set." << std::endl;
		}

		usleep(100000);

	}

	cout<<"Test Done !!"<<endl;
	return 0;	
}
