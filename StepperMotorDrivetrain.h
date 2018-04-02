#ifndef STEPPERMOTORDRIVETRAIN_H
#define STEPPERMOTORDRIVETRAIN_H

#include <Arduino.h>

#define STEPS_PER_REVOLUTION 200

/*
 * (Taken from Arduino Stepper.h)
 * The sequence of control signals for 4 control wires is as follows:
 *
 * Step C0 C1 C2 C3
 *    1  1  0  1  0
 *    2  0  1  1  0
 *    3  0  1  0  1
 *    4  1  0  0  1
 */

class StepperMotorDrivetrain
{
	public:
		StepperMotorDrivetrain();
				
		//void begin(int step1, int step2, int dir1, int dir2, int enable1 = -1, int enable2, int stepsPerRotation = 200);

		void operator=(const StepperMotorDrivetrain& drivetrain);
		
		void initLeft(int in1, int in2, int in3, int in4);
		void initRight(int in1, int in2, int in3, int in4);
		
		void step(int left, int right);
		
		void setRPM(float speed);
		void resetStepCounter();
		
		long getLeftSteps();
		long getRightSteps();
		
		int convertInchesToSteps(float inches);
		
	private:
		//Left Motor
		int leftIN1, leftIN2, leftIN3, leftIN4;
		
		//Right Motor
		int rightIN1, rightIN2, rightIN3, rightIN4;
		
		//RPM of both motors
		float rpm;
		
		//Step counters
		long leftSteps;
		long rightSteps;
		
		int leftCounter, rightCounter;
		
		//Stepping functions
		void singleStep(unsigned int stepWait);
		void singleStep_us(unsigned int stepWait);
		
		void sendStepSignalToLeft(int stepID);
		void sendStepSignalToRight(int stepID);
};

#endif