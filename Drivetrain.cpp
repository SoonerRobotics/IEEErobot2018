#include "Drivetrain.h"


Drivetrain::Drivetrain()
{
	
}


//void Drivetrain::begin(Motor leftMot, Motor rightMot, Encoder leftEnc, Encoder rightEnc, Adafruit_BNO055 gyro, IRMatrix mat, DigitalDevice mDetector)
void Drivetrain::begin(Motor leftMot, Motor rightMot, Encoder leftEnc, Encoder rightEnc, IRMatrix matrix, DigitalDevice mDetector)
{
	BasicDrive::begin(leftMot, rightMot, leftEnc, rightEnc);
	////this->gyro = gyro;
	//this->irMatrix = mat;			//Doesn't like this reference 
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

/*
void Drivetrain::setDecisions(LineDecisions lineDecisions)
{
	this->decisions = lineDecisions;
}
*/

void Drivetrain::drive(float targetDistance, float targetAngle)
{
	
	//float Y = 0.0, X = 0.0, distance = 0.0, yaw = 0.0, gyroError;
	float Y = 0.0, X = 0.0, distance = 0.0, gyroError;
	
	//Reset Encoders
	BasicDrive::getLeftEncoder().reset();
	BasicDrive::getRightEncoder().reset();
	
	//Reset Gyro
	//this->gyro.reset();
	
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
		distance = (BasicDrive::getLeftEncoder().getTicks() + BasicDrive::getRightEncoder().getTicks()) / 2;
		
		//Calculate Gyro Error
		this->yaw = 0;//this->gyro.getYaw();
		gyroError = this->yaw - targetAngle;
		
		//Wrap the gyro error to [-180, 180]
		if(gyroError > 180)
		{
			gyroError = -(360 - gyroError);
		}
		
		//Calculate Y and X values (straight vector and turn vector respectively)
		Y = distancePID.getOutput(targetDistance, distance);
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
			if(distanceTimerElapsed > this->constants.setpointTimeout)
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


void Drivetrain::followLine()
{
	int irMatrixValue = this->irMatrix.readToBinary();
	
	float driveSpeed = this->constants.lineFollowSpeed;
	float turnSpeed = 0;
	//float turnSpeed = this->constants.lineTurnSpeed;
	
	//Check for special case situations before setting output
	if(irMatrixValue&3 == 1)
	{
		turnSpeed = 0.25;
		
	} else if(irMatrixValue&3 == 1)
		
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
		
		float driveSpeed = this->constants.lineFollowSpeed;
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

void Drivetrain::searchForward()
{
	while (this->irMatrix.readToBinary()>>3&0) 
	{
		drive(0.25,0.0);
		delay(50);
	}
	
}

void Drivetrain::setYaw(double newYaw)
{
	return this->gyro;
}

void Drivetrain::followLineGyro(float targetAngle,float inputAngle){
	//IF all center IR senor arn't on line
	//CHeck current angle against angle
	if(irMatrixValue&21 == 1)//If all center IR sensor are on line
	{
		arcadeDrive(lineFollowSpeed,0);
	}
	else{
		int diff = inputAngle - targetAngle);
		if(diff > 0){
			arcadeDrive(lineFollowSpeed,10);
		}
		else{
			arcadeDrive(lineFollowSpeed,-10);
		}
	}
}
	
	
}	

