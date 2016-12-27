#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H


#include <iostream>
#include <wiringPi/wiringPi.h>
#include <wiringPi/softPwm.h>

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
		static int motorLTIN1;
		static int motorLTIN2;
		static int motorLTPWM;
		static int motorRTIN1;
		static int motorRTIN2;
		static int motorRTPWM;
		static int motorLDIN1;
		static int motorLDIN2;
		static int motorLDPWM;
		static int motorRDIN1;
		static int motorRDIN2;
		static int motorRDPWM;
};

#endif
