#ifndef INTAKECONSTANTS_H
#define INTAKECONSTANTS_H

struct IntakeConstants
{
	//Important heights in the pickup sequence
	float idleHeight;
	float pickUpHeight;
	float topHeight;
	float scanHeight;
	
	//Speeds for the motor to move at during the pickup sequence
	float motorSpeed;
	float stallSpeed;
	//TODO: Add more speeds to optimize the process
	
	//Time delays in the pickup sequence
	long magnetWaitTime;
};

#endif