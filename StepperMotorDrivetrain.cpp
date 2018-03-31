#include "StepperMotorDrivetrain.h"

StepperMotor::StepperMotorDrivetrain()
{
	this->STEP = NULL;
	this->DIR = NULL;
	this->enable = NULL;
}

void StepperMotorDrivetrain::operator=(const StepperMotor& motor, const StepperMotor& motor)
{
	this->STEP1 = motor1.STEP;
	this->DIR1 = motor1.DIR;
	this->enable1 = motor1.enable;
	
	this->STEP2 = motor2.STEP;
	this->DIR2 = motor2.DIR;
	this->enable2 = motor2.enable;
	
	this->rpm = motor.rpm;
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
	
	if(enable != -1)
	{
		pinMode(enable, OUTPUT);
		digitalWrite(enable, HIGH);
	}
}

void StepperMotorDrivetrain::setRPM(float speed)
{
	this->rpm = abs(speed);
}

void StepperMotorDrivetrain::step(int steps)
{
	//Enable the motor for movement
	if(this->enable != -1)
	{
		digitalWrite(this->enable, LOW);
	}
	
	int direction = 0;
	bool millisecond_interval = true;
	
	if(steps < 0)
	{
		digitalWrite(this->DIR, LOW);
		direction = -1;
	}
	else
	{
		digitalWrite(this->DIR, HIGH);
		direction = 1;
	}
	
	//Determine how many microseconds we want to wait, and convert to an integer
	double totalTime = (static_cast<double>(steps) / this->stepsPerRotation) / this->rpm * 60.0 * 1000.0; 	
	double T = (totalTime / steps) / 2;
	
	//Convert to microseconds if delay would be 0.
	if(T < 1)
	{
		T*=1000;
		millisecond_interval = false;
	}
	
	unsigned long stepWait = static_cast<int>(T);
	
	for(int i = 0; i < abs(steps); ++i)
	{
		if(millisecond_interval)
		{
			singleStep(stepWait);
		}
		else
		{
			singleStep_us(stepWait);
		}
		
		this->currentSteps += direction;
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