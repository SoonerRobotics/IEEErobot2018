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
}

Intake::Intake(Motor motor, Encoder encoder, DigitalDevice metalDetector, DigitalDevice limitSwitch, Electromagnet electromagnet)
{
	this->intakeMotor = motor;
	this->intakeEncoder = encoder;
	this->metalDetector = metalDetector;
	this->limitSwitch = limitSwitch;
	this->electromagnet = electromagnet;
	
	this->pickUpState = IDLE;
}


bool Intake::pickUpSequence()
{
	switch(this->pickUpState)
	{
		case IDLE:
			//If the intake is above the grab height, drop the magnet to that height
			if(this->intakeEncoder.getValue() > this->constants.pickUpHeight)
			{
				//Output <0 to go down
				this->intakeMotor.output((-1) * this->constants.motorSpeed);
			}
			else //Otherwise it is time to grab the coin
			{
				this->pickUpState = GRAB;
			}
			return false;
			
		case GRAB:
			//Stop the motor so we can do a pickup
			this->intakeMotor.output(this->constants.stallSpeed);
			
			//Pick up the coin
			this->electromagnet.pickUp();
			
			//Wait to make sure it is picked up
			delay(this->constants.magnetWaitTime);
			
			//Begin the process of scanning the coin
			this->pickUpState = SCAN;
			
			//Process not complete yet
			return false;
			
		case SCAN:
			//Raise the intake to scanning height
			if(this->intakeEncoder.getValue() < this->constants.scanHeight)
			{
				//Output >0 to go up
				this->intakeMotor.output(this->constants.motorSpeed);
			}
			else
			{
				//Stop the motor so we can scan the coin
				this->intakeMotor.output(this->constants.stallSpeed);
				
				//TODO: Deploy the RGB color sensor!
				
				//TODO: Detect and save the color
				
				//Move to the full raise state
				this->pickUpState = RAISE;
			}
			return false;
			
		case RAISE:
			//While the intake is below the max height and the limit switch is not pressed
			if(this->intakeEncoder.getValue() < this->constants.topHeight && this->limitSwitch.read() == LOW)
			{
				//Output >0 to go up
				this->intakeMotor.output(this->constants.motorSpeed);
			}
			else
			{
				//Stop driving the motor upwards
				this->intakeMotor.output(this->constants.stallSpeed);
				
				//We have reached the height, so time to store.
				this->pickUpState = STORE;
			}
			
			//Process unfinished
			return false;
			
		case STORE:
			//Check to see if turntable is ready to receive the coin
			if(this->turnTable.inPosition() && this->electromagnet.hasCoin())
			{
				//Once the turntable is ready, drop the coin
				this->electromagnet.drop();
				
				//Wait for the coin to drop
				delay(this->constants.magnetWaitTime);
				
				//Tell the turntable to go back to its idle state
				
				//Process unfinished
				return false;
			}
			//Check to see if the turntable is ready to let the intake drop
			else if(this->turnTable.inPosition() && !this->electromagnet.hasCoin())
			{
				//Drive the intake down to the idle height
				if(this->intakeEncoder.getValue() > this->constants.idleHeight)
				{
					//Output <0 to go down
					this->intakeMotor.output((-1) * this->constants.motorSpeed);
					
					//Almost done, but not quite.
					return false;
				}
				else
				{
					//Stop driving the motor downwards
					this->intakeMotor.output(this->constants.stallSpeed);
					
					//Pickup complete!
					return true;
				}
			}
			else
			{
				//Keep waiting for whatever process to happen
				return false;
			}
			
		default:
			this->pickUpState = IDLE;
			return false;
	}
}

void Intake::setConstants(IntakeConstants K)
{
	this->constants = K;
}