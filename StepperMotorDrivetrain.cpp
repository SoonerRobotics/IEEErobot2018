#include "StepperMotorDrivetrain.h"

StepperMotorDrivetrain::StepperMotorDrivetrain()
{
	this->leftSteps = 0;
	this->leftCounter = 0;
	
	this->rightSteps = 0;
	this->rightCounter = 0;
	
	//Set this to a medium speed
	this->rpm = 25;
}

void StepperMotorDrivetrain::operator=(const StepperMotorDrivetrain& drivetrain)
{
	this->leftIN1 = drivetrain.leftIN1;
	this->leftIN2 = drivetrain.leftIN2;
	this->leftIN3 = drivetrain.leftIN3;
	this->leftIN4 = drivetrain.leftIN4;
	
	this->rightIN1 = drivetrain.rightIN1;
	this->rightIN2 = drivetrain.rightIN2;
	this->rightIN3 = drivetrain.rightIN3;
	this->rightIN4 = drivetrain.rightIN4;
	
	this->rpm = drivetrain.rpm;
	
	this->leftSteps = drivetrain.leftSteps;
	this->rightSteps = drivetrain.rightSteps;
	
	this->leftCounter = drivetrain.leftCounter;
	this->rightCounter = drivetrain.rightCounter;
}


void StepperMotorDrivetrain::initLeft(int in1, int in2, int in3, int in4)
{
	this->leftIN1 = in1;
	this->leftIN2 = in2;
	this->leftIN3 = in3;
	this->leftIN4 = in4;
	
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
}

void StepperMotorDrivetrain::initRight(int in1, int in2, int in3, int in4)
{
	this->rightIN1 = in1;
	this->rightIN2 = in2;
	this->rightIN3 = in3;
	this->rightIN4 = in4;
	
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
}

void StepperMotorDrivetrain::setRPM(float speed)
{
	this->rpm = abs(speed);
}

void StepperMotorDrivetrain::step(int left, int right)
{
	
	bool millisecond_interval = false;
	
	//We basically force left and right to be equal here, because they should be.
	//NO CURVE TURNS ALLOWED (Down with tank steer)
	int steps = min(abs(left), abs(right));
	
	int rightDirection = -right < 0 ? -1 : 1;
	int leftDirection = -left < 0 ? -1 : 1;
		
	//Determine how many microseconds we want to wait, and convert to an integer
	double totalTime = (static_cast<double>(steps) / STEPS_PER_REVOLUTION) / this->rpm * 60.0 * 1000.0 * 1000.0;
	double T = (totalTime / steps) / 2;
	
	//Convert to milliseconds if delay would be greater than 5,000 us.
	if(T > 5000)
	{
		T /= 1000;
		millisecond_interval = true;
	}
	
	unsigned long stepWait = static_cast<int>(T);
	
	for(int i = 0; i < steps; ++i)
	{
		this->leftSteps += leftDirection;
		this->rightSteps += rightDirection;
		
		this->leftCounter += leftDirection;
		this->rightCounter += rightDirection;
		
		//Constrain the counters to the step boundaries
		//Right
		this->rightCounter = this->rightCounter < 0 ? STEPS_PER_REVOLUTION - 1 : this->rightCounter;
		this->rightCounter = this->rightCounter >= STEPS_PER_REVOLUTION ? 0 : this->rightCounter;
		
		//Left
		this->leftCounter = this->leftCounter < 0 ? STEPS_PER_REVOLUTION - 1 : this->leftCounter;
		this->leftCounter = this->leftCounter >= STEPS_PER_REVOLUTION ? 0 : this->leftCounter;
		

		if(millisecond_interval)
		{
			singleStep(stepWait);
		}
		else
		{
			singleStep_us(stepWait);
		}
	}
}

bool StepperMotorDrivetrain::stepToAngle(float target, float current)
{
	
	if (target > 0)
	{
		if (inRange(target, current, ANGLETHRESHOLD))
		{
			step(0, 0);
			return true;
		}
		else
		{
<<<<<<< HEAD
			step(-1, 1);
			return false;
=======
			if (current < target) 
			{
				step(-1, 1);
				return false;
			}
			else
			{
				step(1, -1);
				return false;
			}
>>>>>>> 8533d8619f274a2e129dfe0dc15c2fe56fa705c9
		}

	}
	else //(target < 0)
	{
		if (inRange(target, current, ANGLETHRESHOLD))
		{
			step(0, 0);
			return true;
		}
		else
		{
<<<<<<< HEAD
			step(1, -1);
			return false;
=======
			if (current > target)
			{
				step(1, -1);
				return false;
			}
			else
			{
				step(-1, 1);
				return false;
			}
>>>>>>> 8533d8619f274a2e129dfe0dc15c2fe56fa705c9
		}
	}
	
}

void StepperMotorDrivetrain::resetStepCounter()
{
	leftSteps = 0;
	rightSteps = 0;
}

long StepperMotorDrivetrain::getLeftSteps()
{
	return this->leftSteps;
}

long StepperMotorDrivetrain::getRightSteps()
{
	return this->rightSteps;
}

int StepperMotorDrivetrain::convertInchesToSteps(float inches)
{
	//number of steps / circumference of wheel = ratio
	return static_cast<int>((STEPS_PER_REVOLUTION/(2.875*3.141592653589))*inches);
}
//Private Functions

void StepperMotorDrivetrain::singleStep(unsigned int stepWait)
{
	sendStepSignalToRight(rightCounter % 4);
	sendStepSignalToLeft(leftCounter % 4);	
    delay(stepWait); // Wait
}

void StepperMotorDrivetrain::singleStep_us(unsigned int stepWait)
{
	sendStepSignalToRight(rightCounter % 4);
	sendStepSignalToLeft(leftCounter % 4);
    delayMicroseconds(stepWait); // Wait
}

void StepperMotorDrivetrain::sendStepSignalToLeft(int stepID)
{
	switch (stepID) {
		case 0:  // 1010
			digitalWrite(leftIN1, HIGH);
			digitalWrite(leftIN2, LOW);
			digitalWrite(leftIN3, HIGH);
			digitalWrite(leftIN4, LOW);
			break;
		case 1:  // 0110
			digitalWrite(leftIN1, LOW);
			digitalWrite(leftIN2, HIGH);
			digitalWrite(leftIN3, HIGH);
			digitalWrite(leftIN4, LOW);
			break;
		case 2:  //0101
			digitalWrite(leftIN1, LOW);
			digitalWrite(leftIN2, HIGH);
			digitalWrite(leftIN3, LOW);
			digitalWrite(leftIN4, HIGH);
			break;
		case 3:  //1001
			digitalWrite(leftIN1, HIGH);
			digitalWrite(leftIN2, LOW);
			digitalWrite(leftIN3, LOW);
			digitalWrite(leftIN4, HIGH);
			break;
    }
}

void StepperMotorDrivetrain::sendStepSignalToRight(int stepID)
{
	switch (stepID) {
		case 0:  // 1010
			digitalWrite(rightIN1, HIGH);
			digitalWrite(rightIN2, LOW);
			digitalWrite(rightIN3, HIGH);
			digitalWrite(rightIN4, LOW);
			break;
		case 1:  // 0110
			digitalWrite(rightIN1, LOW);
			digitalWrite(rightIN2, HIGH);
			digitalWrite(rightIN3, HIGH);
			digitalWrite(rightIN4, LOW);
			break;
		case 2:  //0101
			digitalWrite(rightIN1, LOW);
			digitalWrite(rightIN2, HIGH);
			digitalWrite(rightIN3, LOW);
			digitalWrite(rightIN4, HIGH);
			break;
		case 3:  //1001
			digitalWrite(rightIN1, HIGH);
			digitalWrite(rightIN2, LOW);
			digitalWrite(rightIN3, LOW);
			digitalWrite(rightIN4, HIGH);
			break;
    }
	
}

bool StepperMotorDrivetrain::inRange(float variable, float constant, float range)
{
	if (abs(variable - constant) < range )
	{
		return true;
	}
	else 
	{
		return false;
	}	
}