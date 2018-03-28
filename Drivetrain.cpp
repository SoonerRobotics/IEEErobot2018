#include "Drivetrain.h"

Drivetrain::Drivetrain()
{
	
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

		//Output to the motors
		/*
		Serial.print("X: ");
		Serial.print(X);
		Serial.print("\t");
		Serial.print("Y: ");
		Serial.println(Y);
		Serial.print("\t");
		*/
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
	
	return movementComplete;
}


void Drivetrain::followLine(int density, int raw, float yaw)
{	

	Serial.print("RAW: ");
	Serial.print(raw);
	Serial.print("\t");
	Serial.print("Density: ");
	Serial.println(density);

	//No sensors see a line
	if (raw == 0)
	{
		//Drive forward until line is seen
		driveIndefinitely(.2, 0, yaw, true);
		
	}
	//A line is seen somewhere
	else if(density > 0)
	{
		//Line is to the right
		if(raw >= 16)
		{
			driveIndefinitely(.2, 20, yaw, true);
			
			lastDirection = "RIGHT";
			
			//Moved Left last
			if(lastDirection.equals("LEFT"))
			{
				driveIndefinitely(.2, 40, yaw, true);
			}
		}
		//line is to the left
		else if(raw <= 8)
		{
			driveIndefinitely(.2, -20, yaw, true);
			
			lastDirection = "LEFT";
			
			//Moved Right lasr
			if(lastDirection.equals("RIGHT"))
			{
				driveIndefinitely(.2, -40, yaw, true);
			}
		}
		else
		{
			driveIndefinitely(.2, 0, yaw, true);
		}
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
	Serial.print("LEFT: ");
	Serial.print(left);
	Serial.print("\t");
	Serial.print("RIGHT: ");
	Serial.println(right);
	
	BasicDrive::setOutput(-left, right);
}

bool Drivetrain::searchForward(int density, float yaw)
{
	Serial.println(density);
	if(density < 3)
	{
		drive(1, 0 , yaw, true);
		return false;
	}
	else
	{
		return true;
	}
}

void Drivetrain::followLineGyro()
{	
	
}