#include "Intake.h"

/**
 * NOTE: The following diagram depicts the ~relative~ encoder values at each special height:
 * The point of this diagram is to show that IDLE is 0, above IDLE is positive and below is negative
 *
 * 15 ------ RAISE
 * 12.5	------  STORE
 *
 *
 *  5	------ SCAN
 *
 *	 0	------ IDLE
 *
 * -5	------ GRAB
 */


Intake::Intake()
{
	this->pickUpState = IDLE;
	this->dropOffState = IDLEd;
	this->currentMotorOutput = 0;
	
	intakePID.initialize(0, intakeKp, intakeKi, intakeKd);
}


void Intake::begin(Motor motor, Encoder encoder, DigitalDevice metalDetector, DigitalDevice loLimitSwitch, DigitalDevice hiLimitSwitch, Electromagnet electromagnet, Turntable turnTable, int colorServoPinNumber)
{
	this->intakeMotor = motor;
	this->intakeEncoder = encoder;
	this->metalDetector = metalDetector;
	this->lowLimitSwitch = loLimitSwitch;
	this->highLimitSwitch = hiLimitSwitch;
	this->electromagnet = electromagnet;
	this->turnTable = turnTable;
	
	this->pickUpState = IDLE;
	this->dropOffState = IDLEd;
	
	this->lastColor = Color("none");
	
	this->colorServo.attach(colorServoPinNumber);
	
	this->lastHeight = idleHeight;
}


int Intake::pickUpSequence(Color color)
{	
	switch(this->pickUpState)
	{
		case IDLE:
			currentMotorOutput = resetSpeed;
		
			//If the intake is above the grab height, drop the magnet to that height
			if(this->lowLimitSwitch.read() != HIGH) // This MUST bottom out!
			{
				//Output <0 to go down
				this->intakeMotor.output(currentMotorOutput);
			}
			else //Otherwise it is time to grab the coin
			{
				this->pickUpState = GRAB;
			}
			return 0;
			
		case GRAB:
			//Stop the motor so we can do a pickup
			this->intakeMotor.output(stallSpeed);
			
			//Pick up the coin
			this->electromagnet.pickUp();
			
			//Wait to make sure it is picked up
			delay(magnetWaitTime);
			
			//Begin the process of scanning the coin
			this->pickUpState = SCAN;
			
			//Process not complete yet
			return 0;
			
		case SCAN:
			currentMotorOutput = intakePID.getOutput2(scanHeight, this->intakeEncoder.getValue());
			currentMotorOutput = coerce(currentMotorOutput, motorSpeed, -motorSpeed);
			
			//Raise the intake to scanning height
			if(abs(this->intakeEncoder.getValue() - scanHeight) < RP_TOLERANCE)
			{
				//Output >0 to go up
				this->intakeMotor.output(currentMotorOutput);
			}
			else
			{
				//Stop the motor so we can scan the coin
				this->intakeMotor.output(stallSpeed);
				
				//Deploy the RGB color sensor! (servo)
				this->colorServo.write(colorServoDeployAngle);
				delay(colorServoDelay);
				
				//Detect and save the color
				this->lastColor = color; 
				
				//Retract color sensor
				this->colorServo.write(colorServoIdleAngle);
				delay(colorServoDelay);
				
				//Move to the full raise state
				this->pickUpState = RAISE;
			}
			return 1;
			
		case RAISE:
			currentMotorOutput = intakePID.getOutput2(topHeight, this->intakeEncoder.getValue());
			currentMotorOutput = coerce(currentMotorOutput, motorSpeed, -motorSpeed);
			
			//While the intake is below the max height and the limit switch is not pressed
			if(abs(this->intakeEncoder.getValue() - topHeight) < RP_TOLERANCE || this->highLimitSwitch.read() == HIGH)
			{
				//Output >0 to go up
				this->intakeMotor.output(motorSpeed);
			}
			else
			{
				//Stop driving the motor upwards
				this->intakeMotor.output(stallSpeed);
				
				//We have reached the height, so time to store.
				this->pickUpState = STORE;
			}
			
			//Process unfinished
			return 1;
			
		case STORE:
			
			if(this->electromagnet.hasCoin())
			{
				//Turn the turntable so it is ready to receive the coin
				this->turnTable.setPosition(this->lastColor);
				delay(turnTableWaitMax);
				
				//Once the turntable is ready, drop the coin
				this->electromagnet.drop();
				
				//Wait for the coin to drop
				delay(magnetWaitTime);
				
				//Tell the turntable to go back to its idle state
				this->turnTable.setPosition();
				delay(turnTableWaitMax);
				
				//Process unfinished
				return 1;
			}
			//Check to see if the turntable is ready to let the intake drop
			else if(!this->electromagnet.hasCoin())
			{
				//Drive the intake down to the idle height
				if(this->intakeEncoder.getValue() > idleHeight)
				{
					//Output <0 to go down to the bottom
					this->intakeMotor.output(resetSpeed);
					
					//Almost done, but not quite.
					return 1;
				}
				else
				{
					//Stop driving the motor downwards
					this->intakeMotor.output(stallSpeed);
					
					//set the sequence to idle
					this->pickUpState = IDLE;
					
					//Pickup complete!
					return 2;
				}
			}
			
		default:
			this->pickUpState = IDLE;
			return 0;
	}
	
}

void Intake::pickUpSequenceA(Color color)
{	
	switch(this->pickUpState)
	{
		case IDLE:
			//If the intake is above the grab height, drop the magnet to that height
			if(this->intakeEncoder.getValue() > pickUpHeight)
			{
				//Output <0 to go down
				this->intakeMotor.output((-1) * motorSpeed);
			}
			else //Otherwise it is time to grab the coin
			{
				this->intakeMotor.output(0);
				this->pickUpState = GRAB;
			}
			
		case GRAB:
			//Stop the motor so we can do a pickup
			this->intakeMotor.output(0);
			
			//Pick up the coin
			this->electromagnet.pickUp();
			
			//Wait to make sure it is picked up
			delay(magnetWaitTime);
			
			//Begin the process of scanning the coin
			this->pickUpState = SCAN;
			
		case SCAN:
			//Raise the intake to scanning height
			if(this->intakeEncoder.getValue() < scanHeight)
			{
				//Output >0 to go up
				this->intakeMotor.output(motorSpeed);
			}
			else
			{
				//Stop the motor so we can scan the coin
				this->intakeMotor.output(0);
				
				//Deploy the RGB color sensor! (servo)
				this->colorServo.write(colorServoDeployAngle);
				delay(colorServoDelay);
				
				//Detect and save the color
				this->lastColor = color;
				
				//Retract color sensor
				this->colorServo.write(colorServoIdleAngle);
				delay(colorServoDelay);
				
				//Move to the full raise state
				this->pickUpState = RAISE;
			}
			
		case RAISE:
			//While the intake is below the max height and the limit switch is not pressed
			if(this->intakeEncoder.getValue() < topHeight && this->lowLimitSwitch.read() == LOW)
			{
				//Output >0 to go up
				this->intakeMotor.output(motorSpeed);
			}
			else
			{
				//Stop driving the motor upwards
				this->intakeMotor.output(0);
				
				//We have reached the height, so time to store.
				this->pickUpState = STORE;
			}
			
		case STORE:
			
			if(this->electromagnet.hasCoin())
			{
				//Turn the turntable so it is ready to receive the coin
				this->turnTable.setPosition(this->lastColor);
				delay(turnTableWaitMax);
				
				//Once the turntable is ready, drop the coin
				this->electromagnet.drop();
				
				//Wait for the coin to drop
				delay(magnetWaitTime);
				
				//Tell the turntable to go back to its idle state
				this->turnTable.setPosition();
				delay(turnTableWaitMax);
				
			}
			//Check to see if the turntable is ready to let the intake drop
			else if(!this->electromagnet.hasCoin())
			{
				//Drive the intake down to the idle height
				if(this->intakeEncoder.getValue() > idleHeight)
				{
					//Output <0 to go down
					this->intakeMotor.output((-1) * motorSpeed);
					
					//Almost done, but not quite.
					return false;
				}
				else
				{
					//Stop driving the motor downwards
					this->intakeMotor.output(stallSpeed);
					
					//set the sequence to idle
					this->pickUpState = IDLE;
					
					//Pickup complete!
					break;
				}
			}
			
		default:
			this->pickUpState = IDLE;
	}
	
}

void Intake::dropOffSequence(Color color)
{
	
	switch(this->dropOffState)
	{
		case IDLEd:
			//If the intake is above the grab height, drop the magnet to that height
			if(this->intakeEncoder.getValue() > storageHeight)
			{
				//Output <0 to go down
				this->intakeMotor.output((-1) * motorSpeed);
			}
			else //Otherwise it is time to grab the coin from storage
			{
				this->dropOffState = GRABd;
			}
			
		case GRABd:
			//Turn the turntable to the coin storage of given color
			this->turnTable.setPosition(color);
			delay(turnTableWaitMax);
			
			//Stop the motor so we can do a pickup
			this->intakeMotor.output(stallSpeed);
			
			//Pick up the coins from storage
			this->electromagnet.pickUp();
			
			//Wait to make sure it is picked up
			delay(magnetWaitTime);
			
			//Begin the process of RAISE
			this->dropOffState = RAISEd;
			
		case RAISEd:
			//While the intake is below the max height and the limit switch is not pressed
			if(this->intakeEncoder.getValue() < topHeight && this->highLimitSwitch.read() == LOW)
			{
				//Output >0 to go up
				this->intakeMotor.output(motorSpeed);
			}
			else
			{
				//Stop driving the motor upwards
				this->intakeMotor.output(stallSpeed);
				
				//We have reached the height, so time to drop off the coins.
				this->dropOffState = DROPd;
			}	
		case DROPd:
			
			if(this->electromagnet.hasCoin())
			{
				//Turn the turntable so the coins can pass through
				this->turnTable.setPosition(0);
				delay(turnTableWaitMax);
				
				//Once the turntable is ready, drop the magnet to drop height
				if(this->intakeEncoder.getValue() > dropHeight)
				{
					//Output <0 to go down
					this->intakeMotor.output((-1) * (motorSpeed));
				}
				else if(this->intakeEncoder.getValue() < dropHeight)
				{
					//Once the turntable is ready, drop the coin
					this->electromagnet.drop();
				
					//Wait for the coin to drop
					delay(magnetWaitTime);
				}
				
				this->turnTable.setPosition();
				delay(turnTableWaitMax);
			}
			//Check to see if the turntable is ready to let the intake drop
			else if(!this->electromagnet.hasCoin())
			{
				//Return the magnet to above the turntable
				if(this->intakeEncoder.getValue() < idleHeight)
				{
					//Output >0 to go up
					this->intakeMotor.output(motorSpeed);
				}
				else
				{
					//Stop driving the motor downwards
					this->intakeMotor.output(stallSpeed);
					
					//set the sequence to idle
					this->dropOffState = IDLEd;
					
					//Pickup complete!
					break;
				}
			}
			
		default:
			this->dropOffState = IDLEd;
	}
	
}

void Intake::raiseRackAndPinion(int newHeight)
{
	while (abs(this->intakeEncoder.getValue() - newHeight) > heightTreshold)
	{
		//If we are above the last height lower the motor
		if(lastHeight > newHeight)
		{
			this->intakeMotor.output((-1) * (motorSpeed));
		}
		//If we are below the last height raise the motor
		else if(lastHeight < newHeight)
		{
			this->intakeMotor.output((1) * (motorSpeed));
		}
	}
	if (abs(this->intakeEncoder.getValue() - newHeight) < heightTreshold)
	{
		lastHeight = newHeight;
	}
}

Motor Intake::getRackAndPinionMotor()
{
	return this->intakeMotor;
}

Encoder Intake::getRackAndPinionEncoder()
{
	Serial.print("this");
	return this->intakeEncoder;
}

void Intake::bottomLimit()
{
	this->intakeMotor.output(0);
	this->intakeEncoder.reset();
	currentMotorOutput = 0;
}

void Intake::topLimit()
{
	this->intakeMotor.output(0);
	currentMotorOutput = 0;
}

void Intake::reset()
{
	//Reset the encoders and the height of the rack and pinion!
	intakeMotor.output(resetSpeed);
	
	//This will cause the rack and pinion to drop and the interrupts above should make the motor not die
}

bool Intake::coinDetected()
{
	return this->metalDetector.read() == HIGH;
}

float Intake::coerce(float value, float high, float low)
{
	value = value > high ? high : value;
	value = value < low ? low : value;
	return value;
}