#include "Drivetrain.h"


Drivetrain::Drivetrain()
{
	
}

void Drivetrain::begin(Motor leftMot, Motor rightMot, Encoder leftEnc, Encoder rightEnc, IRMatrix matrix, DigitalDevice mDetector)
{
	BasicDrive::begin(leftMot, rightMot, leftEnc, rightEnc);
	this->irMatrix = mat;			//Doesn't like this reference 
	this->metDetector = mDetector;
}

void Drivetrain::initializeTurnPID(Collection<float> turnK)
{
	this->turnPID.initialize(0, turnK);
}

void Drivetrain::initializeDistancePID(Collection<float> distanceK)
{
	this->distancePID.initialize(0, distanceK);
}


bool Drivetrain::drive(float targetDistance, float targetAngle, float inputYaw, bool reinitialize)
{
	if(reinitialize)
	{
		Y = 0.0;
		X = 0.0; 
		distance = 0.0;
		
		//Reset Encoders
		BasicDrive::getLeftEncoder().reset();
		BasicDrive::getRightEncoder().reset();
		
		//Set the 'in-range' and 'complete' flags to false
		distanceInRange = false;
		angleInRange = false;
		driveComplete = false;
		turnComplete = false;
		movementComplete = false;
		
		//Start a timer
		timer = millis();
		timeoutClock = 0;
	}
	
	if(!movementComplete)
	{
		//Calculate Distance
		distance = (BasicDrive::getLeftEncoder().getTicks() + BasicDrive::getRightEncoder().getTicks()) / 2;
		
		//Calculate Gyro Error
		gyroError = inputYaw - targetAngle;
		
		//Wrap the gyro error to [-180, 180]
		if(gyroError > 180)
		{
			gyroError = -(360 - gyroError);
		}
		
		//Calculate Y and X values (straight vector and turn vector respectively)
		Y = distancePID.getOutput(targetDistance, distance);
		X = turnPID.getOutput(0, gyroError);
		
		//Check to see if the distance is in range and if the drive is completed
		if(abs(distance - targetDistance) < distanceThreshold && abs(Y) < stopSpeedThreshold)
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
			if(distanceTimerElapsed > setpointTimeout)
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
		if(abs(gyroError) < angleThreshold && abs(X) < stopSpeedThreshold)
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
			if(angleTimerElapsed > setpointTimeout)
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
		if(timeoutClock > timeout && timeout != -1)
		{
			//Timeout has been exceeded, so the drive is finished
			driveComplete = true;
			turnComplete = true;
		}
	}
	
	//Is the move complete or not?
	movementComplete = driveComplete && turnComplete; 
	
	return movementComplete;
	
}


void Drivetrain::followLine()
{
	int irMatrixValue = this->irMatrix.readToBinary();
	
	float driveSpeed = lineFollowSpeed;
	float turnSpeed = 0;
	
	//Check for special case situations before setting output
	if(irMatrixValue&3 == 1)
	{
		turnSpeed = 0.25;
		
	} 
	else if(irMatrixValue&3 == 1)	
	{
		turnSpeed = -0.25;
	}
	
	//Set the output to drive along the line
	arcadeDrive(driveSpeed, turnSpeed);
}

void Drivetrain::followLineUntilCoin() 
{
	while(metDetector.read() == LOW)
	{
		int irMatrixValue = this->irMatrix.readToBinary();
		
		float driveSpeed = lineFollowSpeed;
		float turnSpeed = 0;
		
		//Check for special case situations before setting output
		if(irMatrixValue&3 == 1)
		{
			turnSpeed = 0.25;
		
		} else if(irMatrixValue&3 == 1)
		
		{
			turnSpeed = -0.25;
		}
		
		arcadeDrive(driveSpeed, turnSpeed);
	}
}

/**
 * Private Functions Below
 */

 
void Drivetrain::arcadeDrive(float Y, float X)
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

void Drivetrain::searchForward(float inputYaw)
{
	bool complete = true;
	while (this->irMatrix.readToBinary()>>3&0) 
	{
		complete = drive(0.25, 0.0, inputYaw, complete);
		delay(50);
	}
}

void Drivetrain::followLineGyro(float targetAngle, float inputAngle)
{	
	this->irMatrixValue = irMatrix.readToBinary();
	
	//IF all center IR senor arn't on line
	//CHeck current angle against angle
	if(irMatrixValue&21 == 1)//If all center IR sensor are on line
	{
		arcadeDrive(lineFollowSpeed,0);
	}
	else
	{
		//Find difference between current angle and the target angle
		int diff = inputAngle - targetAngle;
		
		//Turn left or right based on the difference
		if(diff > 0)
		{
			arcadeDrive(lineFollowSpeed,10);
		}
		else
		{
			arcadeDrive(lineFollowSpeed,-10);
		}
	}
}

int Drivetrain::getIRMatrixValue()
{
	return this->irMatrix.readToBinary();
}
