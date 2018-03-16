#include "Drivetrain.h"

Drivetrain::Drivetrain()
{
	
}

void Drivetrain::begin(Motor& leftMot, Motor& rightMot, Encoder& leftEnc, Encoder& rightEnc, IRMatrix& mat, DigitalDevice& mDetector)
{
	BasicDrive::begin(leftMot, rightMot, leftEnc, rightEnc);
	this->irMatrix = mat;			//Doesn't like this reference 
	this->metDetector = mDetector;
	
	//Set params for PID's
	//turnPID_v1(gyroError, turnPIDOutput, targetDistance, TURN_KP, TURN_KI, TURN_KD, 0);
	//distancePID_v1(distance, distancePIDOutput, 0, DIST_KP, DIST_KI, DIST_KD, 0);
	
	//Set PID modes
	//turnPID_v1.SetMode(AUTOMATIC);
	//distancePID_v1.SetMode(AUTOMATIC);
	
	//Clamp max and min values, from -1 to 1 to match motor outputs
	//turnPID_v1.SetOutputLimits(-1, 1);
	//distancePID_v1.SetOutputLimits(-1, 1);
}

void Drivetrain::initializeTurnPID(float turnKp, float turnKi, float turnKd, float high, float low)
{
	this->turnPID.initialize(0, turnKp, turnKi, turnKd);
	this->turnPID.setOutputRange(high, low);
	this->highT = high;
	this->lowT = low;
}

void Drivetrain::initializeDistancePID(float distanceKp, float distanceKi, float distanceKd, float high, float low)
{
	this->distancePID.initialize(0, distanceKp, distanceKi, distanceKd);
	this->distancePID.setOutputRange(high, low);	
	this->highD = high;
	this->lowD = low;
}


bool Drivetrain::drive(float targetDistance, float targetAngle, float inputYaw, bool reinitialize, long timeout)
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
		
		this->targetDistance = targetDistance;
		this->targetAngle = targetAngle;
		
		//Start a timer
		timer = millis();
		timeoutClock = 0;
	}
	
	if(!movementComplete)
	{
		//Calculate Distance
		this->distance = (BasicDrive::getLeftEncoder().getValue() + BasicDrive::getRightEncoder().getValue()) / 2;
		
		//Calculate Gyro Error
		gyroError = inputYaw - targetAngle;
		
		//Wrap the gyro error to [-180, 180]
		if(gyroError > 180)
		{
			gyroError = -(360 - gyroError);
		}
		
		//Calculate Y and X values (straight vector and turn vector respectively)
		//Y = distancePID.getOutput(targetDistance, distance);
		//X = -turnPID.getOutput(0, gyroError);
		
		Y = distancePID.getOutput2(targetDistance, distance);
		if (Y >= highD) { Y = highD; }
		else if (Y <= lowD) { Y = lowD; }
		X = -turnPID.getOutput2(0, gyroError);
		if (X >= highT) { X = highT; }
		else if (X <= lowT) { X = lowT; }
		
		//Check to see if the distance is in range and if the drive is completed
		if(abs(distance - targetDistance) < distanceThreshold && abs(Y) < stopSpeedThreshold)
		{
			//If this is the first time being in range
			if(!distanceInRange)
			{
				//Start the timer
				distanceTimer = millis();
				
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
		if(abs(gyroError) < angleThreshold && abs(Y) < stopSpeedThreshold)
		{
			//If this is the first time we have been at the angle goal, reset the timer
			if(!angleInRange)
			{
				angleTimer = millis();
				angleInRange = true;
			}
			
			//Calculate how long we have been at the target
			angleTimerElapsed = millis() - angleTimer;
			
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
		arcadeDrive(X, Y);
		
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
	
	if(movementComplete)
	{
		arcadeDrive(0, 0);
	}
	
	return movementComplete;
}


void Drivetrain::followLine()
{
	int irMatrixValue = this->irMatrix.readToBinary();
	
	float driveSpeed = lineFollowSpeed;
	float turnSpeed = 0;
	
	
	turnSpeed = getPositionSpark();
	
	Serial.print("\tspd: ");
	Serial.print(turnSpeed);
	Serial.println();
	
	if(abs(turnSpeed) > 0)
	{
		arcadeDrive(turnSpeed, 0);
	}
	else
	{
		arcadeDrive(0, 0);
	}
}

void Drivetrain::followLineUntilCoin() 
{
	while(metDetector.read() == LOW)
	{
		int irMatrixValue = this->irMatrix.readToBinary();
		
		float driveSpeed = lineFollowSpeed;
		float turnSpeed = 0;
		
		turnSpeed = getPositionSpark();

		if(abs(turnSpeed) > 0)
		{
			arcadeDrive(turnSpeed, 0);
		}
		else
		{
			arcadeDrive(0, driveSpeed);
		}
		
	}
}

//get data from IR as a vector to the average of detected points
//example, if center bits detect line (ir2, ir3, ir4), the position will be 0
float Drivetrain::getPositionSpark()
{
	int irMatrixValue = this->irMatrix.readToBinary();
	int bitsCounted = 0;
	int accumulator = 0;
	float turnSpeed = 0;
	
	//count bits
	for(int i = 0; i < 8; i++)
	{
		if ( (irMatrixValue >> i) & 1 == 1)
		{
			bitsCounted++;
		}
	}
	
	//Sparkfun does bits differently, with 0 at the center.
	//We need to convert our center IR to be the least significant.
	//This is done in the IR matrix Class
	
	//positive bits 
	for (int i = 7; i > 3; i--)
	{
		if ( (irMatrixValue >> i ) & 1 == 1)
		{
			accumulator += ((-32* (i - 1)) + 1);
		}
	}
	
	//negative bits (ir3, ir2, ir1)
	for (int i = 0; i < 4; i++)
	{
		if ( (irMatrixValue >> i ) & 1 == 1)
		{
			accumulator += ((32 * (3 - i)) - 1);
		}
	} 
	
	float positionValue = 0;
	if(bitsCounted > 0)
	{	
		//position value in a range from -127 to 127
		positionValue = accumulator / bitsCounted;
	}

	Serial.print("\tpos: ");
	Serial.print(positionValue);
	
	if (positionValue > 50)
	{
		turnSpeed = lineTurnSpeed;
	}
	else if (positionValue < -50)
	{
		turnSpeed = -lineTurnSpeed;
	}
	else
	{
		turnSpeed = 0;
	}
	
	return turnSpeed;
}

float Drivetrain::getTurnSpeed()
{
	int irMatrixValue = this->irMatrix.readToBinary();
	int left = 0, right = 0;
	
	for(int r = 0; r < 8; ++r)
	{
		Serial.print((irMatrixValue >> r) & 1);
		Serial.print("\t");
	}
	
	if(irMatrixValue == 1)
	{
		return 0;
	}
	else
	{
		for(int i = 0; i < 4; ++i)
		{
			right += ((irMatrixValue >> i) & 1) * (4-i);
		}
		
		for(int i = 7; i > 3; --i)
		{
			left += ((irMatrixValue >> i) & 1) * (i-3);
		}
		
		Serial.print(left);
		Serial.print(" vs ");
		Serial.print(right);
		
		if(right - left > 1)
		{
			return -lineTurnSpeed;
		}
		else if(right - left < -3)
		{
			return lineTurnSpeed;
		}
		return 0;
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
	
	//IF all center IR senor aren't on line
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

void Drivetrain::printIRMatrix()
{
	this->irMatrix.printRawToSerial();
}
