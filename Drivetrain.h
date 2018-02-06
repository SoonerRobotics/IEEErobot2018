#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <Arduino.h>
#include <Collection.h>
#include <BasicDrive.h>
#include <PIDController.h>
#include <DigitalDevice.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>

#include "Gyro.h"
#include "DriveConstants.h"
#include "LineDecisions.h"
#include "IRMatrix.h"

class Drivetrain : public BasicDrive
{
	public:
		Drivetrain();
		
		void begin(Motor leftMot, Motor rightMot, Encoder leftEnc, Encoder rightEnc, Adafruit_BNO055 gyro, IRMatrix matrix, DigitalDevice mDetector);
		void initializeTurnPID(Collection<float> turnK);
		void initializeDistancePID(Collection<float> distanceK);
		void setConstants(DriveConstants k);
		void setDecisions(LineDecisions lineDecisions);
		
		void drive(float distance, float angle);
		void turnToAngle(float angle);
		
		void followLine();
		
		void followLineUntilCoin();
		
		void searchForward();
		
		Gyro& getGyro();
		

	private:
		Gyro gyro;
		IRMatrix irMatrix;
		DigitalDevice metDetector;
		
		PIDController turnPID;
		PIDController distancePID; 
		
		DriveConstants constants;
		LineDecisions decisions;
		
		void arcadeDrive(float Y, float X);
		
		void makeDecision();
		
};


#endif