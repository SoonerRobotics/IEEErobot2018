#include "Drivetrain.h"

Drivetrain::Drivetrain()
{
	
}

void Drivetrain::begin(Motor leftMot, Motor rightMot, Encoder leftEnc, Encoder rightEnc, Gyro gyro)
{
	BasicDrive::begin(leftMot, rightMot, leftEnc, rightEnc);
	this->gyro = gyro;
}

void Drivetrain::initializeTurnPID(Collection<float> turnK)
{
	this->turnPID.initialize(0, turnK);
}

void Drivetrain::initializeDistancePID(Collection<float> distanceK)
{
	this->distancePID.initialize(0, distanceK);
}

void Drivetrain::setConstants(DriveConstants k)
{
	this->constants = k;
}


void Drivetrain::drive(float targetDistance, float targetAngle)
{
	float Y = 0.0, X = 0.0, distance = 0.0, yaw = 0.0, gyroError;
	
	//Reset Encoders
	BasicDrive::leftEncoder.reset();
	BasicDrive::rightEncoder.reset();
	
	//Reset Gyro
	this->gyro.reset();
	
	//Set the 'in-range' and 'complete' flags to false
	bool distanceInRange = false, angleInRange = false;
	bool driveComplete = false, turnComplete = false;
	
	//Start a timer
	long timer = millis();
	long timeoutClock = 0;
	
	//Setup angle and distance timers
	long angleTimer, distanceTimer;
	
	//Setup variables to track elapsed time
	long angleTimerElapsed, distanceTimerElapsed;
	
	while(!driveComplete || !turnComplete)
	{
		//Calculate Distance
		distance = (BasicDrive::leftEncoder.getTicks() + BasicDrive::rightEncoder.getTicks()) / 2;
		
		//Calculate Gyro Error
		yaw = this->gyro.getAngle();
		gyroError = yaw - targetAngle;
		
		//Wrap the gyro error to [-180, 180]
		if(gyroError > 180)
		{
			gyroError = -(360 - gyroError);
		}
		
		//Calculate Y and X values (straight vector and turn vector respectively)
		Y = drivePID.getOutput(targetDistance, distance);
		X = turnPID.getOutput(0, gyroError);
		
		//Check to see if the distance is in range and if the drive is completed
		if(abs(distance - targetDistance) < this->constants.distanceThreshold && abs(Y) < this->constants.stopSpeedThreshold)
		{
			//If this is the first time being in range
			if(!distanceInRange)
			{
				//Start the timer
				distanceTimer= millis();
				
				//Set to true to keep timer reference point
				distanceInRange = true;
			}
			
			//Calculate time elapsed while on target
			distanceTimerElapsed = millis() - distanceTimer;
			
			//If we have been on target for a long enough amount of time, we have completed the drive
			if(distanceTimerElapsed > this->constants.setpointTimerThreshold)
			{
				driveComplete = true;
			}
		}
		else
		{
			driveComplete = false;
			distanceInRange = false;
			distanceTimer = millis();
		}
		
		//Check to see if the angle is 'in-range' and if the turn is completed
		if(abs(gyroError) < constants.angleThreshold && abs(X) < this->constants.stopSpeedThreshold)
		{
			//If this is the first time we have been at the angle goal, reset the timer
			if(!angleInRange)
			{
				angleTimer = millis();
				angleInRange = true;
			}
			
			//Calculate how long we have been at the target
			angleTimerElapsed = angleTimer - millis();
			
			//If we have been at the target long enough, the turn is complete
			if(angleTimerElapsed > this->constants.setpointTimeout)
			{
				turnComplete = true;
			}
		}
		else
		{
			turnComplete = false;
			angleInRange = false;
			angleTimer = millis();
		}

		//Set the robot output
		arcadeDrive(Y, X);
		
		//Calculate the time taken in this process
		timeoutClock = millis() - timer;
		
		//Check to make sure the timeout has not been exceeded. A timeout of -1 implies infinite time to reach setpoint
		if(timeoutClock > this->constants.timeout && this->constants.timeout != -1)
		{
			//Timeout has been exceeded, so the drive is finished
			driveComplete = true;
			turnComplete = true;
		}
	}
}

/**
 * Private Functions Below
 */

void arcadeDrive(float Y, float X)
{
	float right, left;
	
	//Calculate motor output
	if(Y > 0)
	{
		if(X > 0)
		{
			right = Y - X;
			left = max(Y, X);
		}
		else
		{
			right = max(Y, -X);
			left = Y + X;
		}
	}
	else
	{
		if(X > 0)
		{
			right = (-1) * max(-Y, X);
			left = Y + X;
		}
		else
		{
			right = Y - X;
			left = (-1) * max(-X, -Y);
		}
	}
	
	//Output to the motors
	BasicDrive::setOutput(left, right);
}