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
				
		void begin(int step1, int step2, int dir1, int dir2, int enable1 = -1, int enable2, int stepsPerRotation = 200);
		
		void step(int steps1, int steps2);
		
		void setRPM1(float speed);
		
		void setRPM2(float speed);
		
		int getCurrentSteps1();
		
		int getCurrentSteps2();
		
		int convertInchesToSteps(float inches);
		
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