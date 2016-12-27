#include "RobotControl.h"

int RobotControl::motorLTIN1 = 2;
int RobotControl::motorLTIN2 = 3;
int RobotControl::motorLTPWM = 0;
int RobotControl::motorRTIN1 = 4;
int RobotControl::motorRTIN2 = 5;
int RobotControl::motorRTPWM = 1;
int RobotControl::motorLDIN1 = 13;
int RobotControl::motorLDIN2 = 14;
int RobotControl::motorLDPWM = 12;
int RobotControl::motorRDIN1 = 10;
int RobotControl::motorRDIN2 = 11;
int RobotControl::motorRDPWM = 6;

void RobotControl::move()
{
	std::cout << "RobotControl Move" << std::endl;
	wiringPiSetup();
	pinMode(motorLTIN1, OUTPUT);
	pinMode(motorLTIN2, OUTPUT);
	pinMode(motorLTPWM, OUTPUT);
	pinMode(motorRTIN1, OUTPUT);
	pinMode(motorRTIN2, OUTPUT);
	pinMode(motorRTPWM, OUTPUT);
	pinMode(motorLDIN1, OUTPUT);
	pinMode(motorLDIN2, OUTPUT);
	pinMode(motorLDPWM, OUTPUT);
	pinMode(motorRDIN1, OUTPUT);
	pinMode(motorRDIN2, OUTPUT);
	pinMode(motorRDPWM, OUTPUT);
	
	pwmWrite(motorLTPWM, 50);
	pwmWrite(motorRTPWM, 50);
	pwmWrite(motorLDPWM, 50);
	pwmWrite(motorRDPWM, 50);
	
	
	digitalWrite(motorLTPWM, HIGH);
	digitalWrite(motorRTPWM, HIGH);
	digitalWrite(motorLDPWM, HIGH);
	digitalWrite(motorRDPWM, HIGH);
}

void RobotControl::stop()
{
	wiringPiSetup();
	
	digitalWrite(motorLTPWM, LOW);
	digitalWrite(motorRTPWM, LOW);
	digitalWrite(motorLDPWM, LOW);
	digitalWrite(motorRDPWM, LOW);
	
	pwmWrite(motorLTPWM, 0);
	pwmWrite(motorRTPWM, 0);
	pwmWrite(motorLDPWM, 0);
	pwmWrite(motorRDPWM, 0);
}

void RobotControl::forward()
{
	wiringPiSetup();
	
	digitalWrite(motorLTIN1, HIGH);
	digitalWrite(motorLTIN2, LOW);
	digitalWrite(motorRTIN1, HIGH);
	digitalWrite(motorRTIN2, LOW);
	digitalWrite(motorLDIN1, HIGH);
	digitalWrite(motorLDIN2, LOW);
	digitalWrite(motorRDIN1, HIGH);
	digitalWrite(motorRDIN2, LOW);
}

void RobotControl::reverse()
{
	wiringPiSetup();
	
	digitalWrite(motorLTIN1, LOW);
	digitalWrite(motorLTIN2, HIGH);
	digitalWrite(motorRTIN1, LOW);
	digitalWrite(motorRTIN2, HIGH);
	digitalWrite(motorLDIN1, LOW);
	digitalWrite(motorLDIN2, HIGH);
	digitalWrite(motorRDIN1, LOW);
	digitalWrite(motorRDIN2, HIGH);
}

void RobotControl::setSpeed(int speed)
{
	
}
