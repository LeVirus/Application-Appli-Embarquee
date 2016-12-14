#include "RobotControl.h"

int motorLTIN1 = 2;
int motorLTIN2 = 3;
int motorLTPWM = 0;
int motorRTIN1 = 4;
int motorRTIN2 = 5;
int motorRTPWM = 1;
int motorLDIN1 = 13;
int motorLDIN2 = 14;
int motorLDPWM = 12;
int motorRDIN1 = 10;
int motorRDIN2 = 11;
int motorRDPWM = 6;

void RobotControl::move()
{
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