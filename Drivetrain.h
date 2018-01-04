#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <Arduino.h>
#include <Collection.h>
#include <BasicDrive.h>
#include <PIDController.h>
#include "Gyro.h"
#include "DriveConstants.h"
#include "LineDecisions.h"
#include "IRMatrix.h"

class Drivetrain : public BasicDrive
{
	public:
		Drivetrain();
		
		void begin(Motor leftMot, Motor rightMot, Encoder leftEnc, Encoder rightEnc, Gyro gyro, IRMatrix matrix);
		void initializeTurnPID(Collection<float> turnK);
		void initializeDistancePID(Collection<float> distanceK);
		void setConstants(DriveConstants k);
		void setDecisions(LineDecisions lineDecisions);
		
		void drive(float distance, float angle);
		void turnToAngle(float angle);
		
		void followLine();
		

	private:
		Gyro gyro;
		IRMatrix irMatrix;
		
		PIDController turnPID;
		PIDController distancePID; 
		
		DriveConstants constants;
		LineDecisions decisions;
		
		void arcadeDrive(float Y, float X);
		
		void makeDecision();
}


#endif