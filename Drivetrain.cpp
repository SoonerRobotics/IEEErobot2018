#include "Drivetrain.h"

Drivetrain::Drivetrain()
{
	angleTicks = 0;
	this->lastLeft = 0;
	this->lastRight = 0;
	this->lastDirection = NONE;
}

void Drivetrain::begin(Motor& leftMot, Motor& rightMot, Encoder& leftEnc, Encoder& rightEnc, DigitalDevice& mDetector)
{
	BasicDrive::begin(leftMot, rightMot, leftEnc, rightEnc);
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
		
		if(targetDistance == 0)
		{
			turnPID.initialize(0, TURN_KP, TURN_KI, TURN_KD);
		}
		else
		{
			turnPID.initialize(0, IN_PLACE_KP, IN_PLACE_KI, IN_PLACE_KD);
		}
	}
	
	if(!movementComplete)
	{
		//Calculate Distance
		this->distance = (BasicDrive::getLeftEncoder().getValue() + BasicDrive::getRightEncoder().getValue()) / 2;
		
		Serial.print(distance);
		
		//Calculate Gyro Error
		gyroError = targetAngle - inputYaw;
		
		//Wrap the gyro error to [-180, 180]
		if(gyroError > 180)
		{
			gyroError = -(360 - gyroError);
		}
		
		//Calculate Y and X values (straight vector and turn vector respectively)
		if (targetDistance != 0)
		{
			Y = distancePID.getOutput2(targetDistance, distance);
				if (Y >= highD) { Y = highD; }
				else if (Y <= lowD) { Y = lowD; }
		}
		else
		{
			Y = 0;
		}
			
		//if (targetAngle != 0)
		//{			
		X = -turnPID.getOutput2(inputYaw, targetAngle);
		if (X >= highT) 
		{ 
			X = highT; 
		}
		else if (X <= lowT) 
		{
			X = lowT;
		}
		//}
		//else 
		//{
		//	X = 0;
		//}
		
		/*Serial.print("PID X:  ");
		Serial.print(X);
		Serial.print("PID Y:  ");
		Serial.print(Y);
		Serial.println("\t");*/
		
		if (targetDistance != 0) 
		{
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
		}
		else
		{
			driveComplete = true;
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
	
	if(movementComplete)
	{
		arcadeDrive(0, 0);
	}
	
	//if(millis() >= driveTimeout)
	//{
	//	return true;
	//}
	
	return movementComplete;
}
/*
bool Drivetrain::turn(float targetAngle, float inputyaw, bool reinitialize)
{
	if(reinitialize)
	{	
		//Set the 'in-range' and 'complete' flags to false
		angleInRange = false;
		driveComplete = false;
		turnComplete = false;
		
		this->targetAngle = targetAngle;
	}
	
	if(!turnComplete)
	{		
		//Calculate Gyro Error
		gyroError = targetAngle - inputYaw;
		
		//Wrap the gyro error to [-180, 180]
		if(gyroError > 180)
		{
			gyroError = -(360 - gyroError);
		}
				
		X = -turnPID.getOutput2(inputYaw, targetAngle);
		if (X >= highT) 
		{ 
			X = highT; 
		}
		else if (X <= lowT) 
		{
			X = lowT;
		}
		
		if(abs(gyroError) < angleThreshold && abs(Y) < stopSpeedThreshold)
		{
			turnComplete = true;
		}
	}
	
	if(turnComplete)
	{
		arcadeDrive(0, 0);
	}
	
	
}
*/
void Drivetrain::followLine(int density, int raw, float yaw)
{	
	//No sensors see a line
	if (raw == 0)
	{
		if (lastDirection == NONE)
		{
			//Drive forward until line is seen
			driveIndefinitely(.2, 0, yaw, true);
		}
		else if (lastDirection == RIGHT)
		{
			driveIndefinitely(.2, 5, yaw, true);
		}
		else if (lastDirection == LEFT)
		{
			driveIndefinitely(.2, -5, yaw, true);
		}
	}
	//A line is seen somewhere
	else
	{
		//Line is to the right
		if(raw < 12) //Flipped due to flipped sensor bar
		{
			driveIndefinitely(.2, 5, yaw, true);
			lastDirection = RIGHT;
		}
		//line is to the left
		else if(raw > 12) //Flipped due to flipped sensor bar
		{
			driveIndefinitely(.2, -5, yaw, true);
			lastDirection = LEFT;
		}
	}

}

bool Drivetrain::pathFollower(int density, int raw)
{
	if(density < 3)
	{
		float turnConstant = rawToIDAverage(raw) / 4;
		
		float turnSpeed = PATH_FOLLOW_TURN_MAX  * turnConstant;
		float driveSpeed = turnConstant <= 0 ? PATH_FOLLOW_SPEED : 0;
		
		arcadeDrive(driveSpeed, turnSpeed);
	}
}


bool Drivetrain::followLineUntilCoin(int density, int position, float yaw) 
{
	Serial.println("metDetector: ");
	Serial.println(metDetector.read());
	if(metDetector.read() == LOW)
	{		
		followLine(density, position, yaw);
		return false;
	}
	else if(metDetector.read() == HIGH)
	{
		return true;
	}
}

void Drivetrain::driveIndefinitely(float speed, float targetAngle, float inputYaw, bool reinitialize)
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
		
		turnPID.initialize(0, TURN_KP, TURN_KI, TURN_KD);
	}
		
	//Calculate Gyro Error
	gyroError = targetAngle - inputYaw;
		
	//Wrap the gyro error to [-180, 180]
	if(gyroError > 180)
	{
		gyroError = -(360 - gyroError);
	}
		
	Y = speed;
	
	X = -turnPID.getOutput2(inputYaw, targetAngle);
			if (X >= highT) { X = highT; }
			else if (X <= lowT) { X = lowT; }
	/*
	Serial.print("PID X:  ");
	Serial.print(X);
	Serial.print("PID Y:  ");
	Serial.print(Y);
	Serial.print("\t");
	*/
	//Set the robot output
	arcadeDrive(Y, X);
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
	/*
	Serial.print("LEFT: ");
	Serial.print(left);
	Serial.print("\t");
	Serial.print("RIGHT: ");
	Serial.println(right);
	*/
	globalLeft = left;
	globalRight = right;
	BasicDrive::setOutput(-left, right);
}

bool Drivetrain::searchForward(int density, float yaw)
{
	Serial.println(density);
	if(density < 3)
	{
		driveIndefinitely(.23, 0 , yaw, true);
		return false;
	}
	else
	{
		arcadeDrive(0,0);
		return true;
	}
}

float Drivetrain::getLeftOutput()
{
	return globalLeft;
}

float Drivetrain::getRightOutput()
{
	return globalRight;
}

void Drivetrain::followLineGyro()
{	
	
}

float Drivetrain::rawToIDAverage(int raw)
{
	//IR Configuration
	//Left: -4  -3  -2  -1   1   2   3   4 :Right
	//Binary:
	// MSB: Right, LSB: Left
	
	float total = 0.0, avg = 0.0;
	int counter = 0, i = -4;
	
	while(raw > 0)
	{
		if(raw & 1)
		{
			counter++;
			total += i;
		}
		
		i++;
		if(i == 0)
		{
			i = 1;
		}
		
		raw = raw >> 1;
	}
	
	avg = total / counter;
	
	return avg;
}