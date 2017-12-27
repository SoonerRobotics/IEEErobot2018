#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <Arduino.h>
#include <Collection.h>
#include <BasicDrive.h>
#include <PIDController.h>
#include "Gyro.h"
#include "DriveConstants.h"

class Drivetrain : public BasicDrive
{
	public:
		Drivetrain();
		
		void begin(Motor leftMot, Motor rightMot, Encoder leftEnc, Encoder rightEnc, Gyro gyro);
		void initializeTurnPID(Collection<float> turnK);
		void initializeDistancePID(Collection<float> distanceK);
		void setConstants(DriveConstants k);
		
		void drive(float distance, float angle);
		void turnToAngle(float angle);

	private:
		Gyro gyro;
		
		PIDController turnPID;
		PIDController distancePID; 
		
		DriveConstants constants;
		
		
		void arcadeDrive(float Y, float X);
}


#endif