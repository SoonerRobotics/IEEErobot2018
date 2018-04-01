#include "StepperMotorDrivetrain.h"

StepperMotor::StepperMotorDrivetrain()
{
	this->STEP1 = NULL;
	this->DIR1 = NULL;
	this->enable1 = NULL;
	
	this->STEP2 = NULL;
	this->DIR2 = NULL;
	this->enable2 = NULL;
}

void StepperMotorDrivetrain::operator=(const StepperMotor& motor1, const StepperMotor& motor2)
{
	this->STEP1 = motor1.STEP;
	this->DIR1 = motor1.DIR;
	this->enable1 = motor1.enable;
	
	this->STEP2 = motor2.STEP;
	this->DIR2 = motor2.DIR;
	this->enable2 = motor2.enable;
	
	this->rpm1 = motor1.rpm;
	this->rpm2 = motor2.rpm;
	this->currentSteps = motor.currentSteps;
	this->stepsPerRotation = motor.stepsPerRotation;
}


void StepperMotorDrivetrain::begin(int step1, int step2, int dir1, int dir2, int enable1, int enable2, int stepsPerRotation)
{
	this->STEP1 = step1;
	this->STEP2 = step2;
	this->DIR1 = dir1;
	this->DIR2 = dir2;
	this->enable1 = enable1;
	htis->enable2 = enable2;
	this->stepsPerRotation = stepsPerRotation;
	
	pinMode(STEP, OUTPUT);
	pinMode(DIR, OUTPUT);
	
	if(enable1 != -1)
	{
		pinMode(enable1, OUTPUT);
		digitalWrite(enable1, HIGH);
	}
	if(enable2 != -1)
	{
		pinMode(enable2, OUTPUT);
		digitalWrite(enable2, HIGH);
	}
}

void StepperMotorDrivetrain::setRPM1(float speed)
{
	this->rpm = abs(speed);
}

void StepperMotorDrivetrain::setRPM2(float speed)
{
	this->rpm2 = abs(speed);
}

void StepperMotorDrivetrain::step(int steps1, int steps2)
{
	//Enable the motor for movement
	if(this->enable1 != -1)
	{
		digitalWrite(this->enable1, LOW);
	}
	if(this->enable2 != -1)
	{
		digitalWrite(this->enable2, LOW);
	}
	
	int direction = 0;
	bool millisecond_interval = true;
	
	if(steps1 < 0 && steps2 < 0)
	{
		digitalWrite(this->DIR1, LOW);
		digitalWrite(this->DIR2, LOW);
		direction = -1;
	}
	else if(steps1 > 0 && steps2 > 0)
	{
		digitalWrite(this->DIR1, HIGH);
		digitalWrite(this->DIR2, HIGH);
		direction = 1;
	}
	else if(steps1 < 0 && steps2 > 0)
	{
		digitalWrite(this->DIR1, LOW);
		digitalWrite(this->DIR2, HIGH);
		direction = 0;
	}
	else
	{
		digitalWrite(this->DIR1, HIGH);
		digitalWrite(this->DIR2, LOW);
		direction = 0;
	}
	
	//Determine how many microseconds we want to wait, and convert to an integer
	
	//Probably want to change this if there are different steps for left and right motors
	//because this is all based on left motor right now (I'm assuming we're turning in place)
	double totalTime1 = (static_cast<double>(steps1) / this->stepsPerRotation) / this->rpm1 * 60.0 * 1000.0;
	double T1 = (totalTime1 / steps1) / 2;
	
	//Convert to microseconds if delay would be 0.
	if(T1 < 1)
	{
		T1*=1000;
		millisecond_interval = false;
	}
	
	unsigned long stepWait = static_cast<int>(T1);
	
	for(int i = 0; i < abs(steps1); ++i)
	{
		if(millisecond_interval)
		{
			singleStep(stepWait);
		}
		else
		{
			singleStep_us(stepWait);
		}
		
		this->currentSteps1 += direction1;
		this->currentSteps2 += direction2;
	}
	
	//Disable the motor to let it cool off
	if(this->enable1 != -1)
	{
		digitalWrite(this->enable1, HIGH);
	}
	if(this->enable2 != -1)
	{
		digitalWrite(this->enable2, HIGH);
	}
}

int StepperMotorDrivetrain::getCurrentSteps1()
{
	return this->currentSteps1;
}

int StepperMotorDrivetrain::getCurrentSteps2()
{
	return this->currentSteps2;
}

int StepperMotorDrivetrain::convertInchesToSteps(float inches)
{
	//number of steps / circumference of wheel = ratio
	return (200/(2.875*3.14))*inches;
}
//Private Functions

void StepperMotorDrivetrain::singleStep(unsigned int stepWait)
{
	digitalWrite(this->STEP1, HIGH); // Output high
	digitalWrite(this->STEP2, HIGH);
    delay(stepWait); // Wait
    digitalWrite(this->STEP1, LOW); // Output low
	digitalWrite(this->STEP2, LOW);
    delay(stepWait); // Wait
}

void StepperMotorDrivetrain::singleStep_us(unsigned int stepWait)
{
	digitalWrite(this->STEP1, HIGH); // Output high
	digitalWrite(this->STEP2, HIGH);
    delayMicroseconds(stepWait); // Wait
    digitalWrite(this->STEP1, LOW); // Output low
	digitalWrite(this->STEP2, LOW);
    delayMicroseconds(stepWait); // Wait
}