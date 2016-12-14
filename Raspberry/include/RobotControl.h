#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H


#include <iostream>
#include <wiringPi.h>

using namespace std;

class RobotControl
{	
	public:
		static void move();
		static void stop();
		static void forward();
		static void reverse();
		static void setSpeed(int speed);
	
	private:
		int motorLTIN1;
		int motorLTIN2;
		int motorLTPWM;
		int motorRTIN1;
		int motorRTIN2;
		int motorRTPWM;
		int motorLDIN1;
		int motorLDIN2;
		int motorLDPWM;
		int motorRDIN1;
		int motorRDIN2;
		int motorRDPWM;
};

#endif