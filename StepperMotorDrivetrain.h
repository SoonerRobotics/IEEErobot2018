#ifndef STEPPERMOTORDRIVETRAIN_H
#define STEPPERMOTORDRIVETRAIN_H

#include <Arduino.h>
#include <StepperMotor.h>
#include <MotorController.h>
#include "Collection.h"

class StepperMotorDrivetrain
{
	public:
		StepperMotorDrivetrain();
		
		void operator=(const StepperMotorDrivetrain& motor1, const StepperMotorDrivetrain& motor2);
		
		void begin(int step, int dir, int enable = -1, int stepsPerRotation = 200);
		
		void step(int steps1, int steps2);
		
		void setRPM(float speed);
		
		int getCurrentSteps();
		
		int convertInchesToSteps();
		
	private:
		int STEP1;
		int STEP2
		int DIR1;
		int DIR2;
		int enable1;
		int enable2;
		
		float rpm1;
		float rpm2;
		int stepsPerRotation;
		int currentSteps1;
		int currentSteps2;
		
		void singleStep(unsigned int stepWait);
		void singleStep_us(unsigned int stepWait);
}