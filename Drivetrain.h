#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <Arduino.h>
#include <Collection.h>
#include <BasicDrive.h>
#include <PIDController.h>
#include <DigitalDevice.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>

#include "DriveConstants.h"
#include "IRMatrix.h"

class Drivetrain : public BasicDrive
{
	public:
		Drivetrain();
		
		void begin(Motor leftMot, Motor rightMot, Encoder leftEnc, Encoder rightEnc, IRMatrix mat, DigitalDevice mDetector);
		void initializeTurnPID(Collection<float> turnK, float high = 1, float low = -1);
		void initializeDistancePID(Collection<float> distanceK, float high = 1, float low = -1);
		
		bool drive(float targetDistance, float targetAngle, float inputYaw, bool reinitialize, long timeout = -1);
		void turnToAngle(float angle);
		
		void followLine();
		
		void followLineUntilCoin();
		
		void searchForward(float inputYaw);
		
		void followLineGyro(float targetAngle, float inputAngle);
		
		int getIRMatrixValue();
		
		void printIRMatrix();

	private:
		IRMatrix irMatrix;
		DigitalDevice metDetector;
		
		PIDController turnPID;
		PIDController distancePID;
		
		void arcadeDrive(float Y, float X);
		
		void makeDecision();
		
		//drive() specific private vars
		float Y;
		float X;
		float distance;
		float gyroError;
		
		bool distanceInRange;
		bool angleInRange;
		bool driveComplete;
		bool turnComplete;
		
		long timer;
		long timeoutClock;
		long angleTimer;
		long distanceTimer;
		long angleTimerElapsed;
		long distanceTimerElapsed;
		
		bool movementComplete;
		
		//IR Matrix / Line follow
		int irMatrixValue;
		
};


#endif