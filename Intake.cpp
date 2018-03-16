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


void Intake::begin(Motor& motor, Encoder& encoder, DigitalDevice& metalDetector, DigitalDevice& loLimitSwitch, DigitalDevice& hiLimitSwitch, Electromagnet& electromagnet, Turntable& turnTable, int colorServoPinNumber)
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
	this->intakeEncoder.reset();
	
	/*while (this->lowLimitSwitch.read() != HIGH)
	{
		//Output <0 to go down
		this->intakeMotor.output(currentMotorOutput);
		this->intakeEncoder.reset();
	}*/
}


int Intake::pickUpSequence(Color color)
{	
	switch(this->pickUpState)
	{
		case IDLE:
			this->state = "IDLE";
			currentMotorOutput = resetSpeed;
		
			//If the intake is above the grab height, drop the magnet to that height
			if(this->lowLimitSwitch.read() != HIGH) // This MUST bottom out!
			{
				//Output <0 to go down
				this->intakeMotor.output(-motorSpeed);
			}
			else //Otherwise it is time to grab the coin
			{
				this->intakeEncoder.reset();
				this->intakeMotor.output(0);
				this->state = "GRAB";
				this->pickUpState = GRAB;
			}
			return 0;
			
		case GRAB:
			this->intakeEncoder.reset();
			this->state = "GRAB";
			//Stop the motor so we can do a pickup
			this->intakeMotor.output(stallSpeed);
			
			//Pick up the coin
			this->electromagnet.pickUp();
			
			//Wait to make sure it is picked up
			delay(magnetWaitTime);
			
			//Begin the process of scanning the coin
			this->state = "SCAN";
			this->pickUpState = SCAN;
			
			//Process not complete yet
			return 0;
			
		case SCAN:
			this->state = "SCAN";
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
				this->state = "RAISE";
				this->pickUpState = RAISE;
			}
			return 1;
			
		case RAISE:
			this->state = "RAISE";
			//currentMotorOutput = intakePID.getOutput2(topHeight, this->intakeEncoder.getValue());
			//currentMotorOutput = coerce(currentMotorOutput, motorSpeed, -motorSpeed);
			
			//While the intake is below the max height and the limit switch is not pressed
			if(this->intakeEncoder.getValue() < 3.0)//|| this->highLimitSwitch.read() != HIGH)
			{
				//Output >0 to go up
				this->intakeMotor.output(motorSpeed);
			}
			else
			{
				//Stop driving the motor upwards
				this->intakeMotor.output(stallSpeed);
				
				//We have reached the height, so time to store.
				this->state = "STORE";
				this->pickUpState = STORE;
			}
			
			//Process unfinished
			return 1;
			
		case STORE:
			this->state = "STORE";
			
			if(this->electromagnet.hasCoin())
			{
				//Turn the turntable so it is ready to receive the coin
				this->turnTable.setPosition(color);
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
				if(this->lowLimitSwitch.read() != HIGH)
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
					this->state = "IDLE";
					this->pickUpState = IDLE;
					
					//Pickup complete!
					return 2;
				}
			}
			
		default:
			this->state = "IDLE";
			this->pickUpState = IDLE;
			return 0;
	}
	
}

void Intake::dropOffSequence(Color color)
{
	
	switch(this->dropOffState)
	{
		case IDLEd:
		
			//If the magnet is below the grab height (from storage), raise the magnet to that height
			while (this->highLimitSwitch.read() != HIGH)
			{
				//Output >0 to go up
				this->intakeMotor.output((1) * motorSpeed);
			}
			
			//Otherwise it is time to grab the coin from storage
			this->dropOffState = GRABd;
			
		case GRABd:
		
			//Turn the turntable to the coin storage of given color
			this->turnTable.setPosition(color);
			delay(turnTableWaitMax);
			
			//Drop the magnet to the height to pick up coins from storage
			while (this->intakeEncoder.getValue() < getFromStorageHeight) 
			{
				//Drive the motor down to the getFromStorageHeight height
				//Output <0 to go down
				this->intakeMotor.output((-1) * motorSpeed);
			}
			
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
			while (this->highLimitSwitch.read() != HIGH)
			{
				//Output >0 to go up
				this->intakeMotor.output(motorSpeed);
			}

			//Stop driving the motor upwards
			this->intakeMotor.output(stallSpeed);
				
			//We have reached the height, so time to drop off the coins.
			this->dropOffState = DROPd;
				
		case DROPd:
			
			//Turn the turntable so the coins can pass through
			this->turnTable.setPosition();
			delay(turnTableWaitMax);
			
			//Once the turntable is ready, drop the magnet to drop height
			while(this->intakeEncoder.getValue() > dropHeight)
			{
				//Output <0 to go down
				this->intakeMotor.output((-1) * (motorSpeed));
			}
				
			//Stop driving the motor upwards
			this->intakeMotor.output(stallSpeed);
				
			//Once the turntable is ready, drop the coin
			this->electromagnet.drop();
			
			//Wait for the coin to drop
			delay(magnetWaitTime);
				

			
			//Return the magnet to above the turntable
			while (this->highLimitSwitch.read() != HIGH)
			{
				//Output >0 to go up
				this->intakeMotor.output(motorSpeed);
			}
			
			//Stop driving the motor upwards
			this->intakeMotor.output(stallSpeed);
				
			//set the sequence to idle
			this->dropOffState = IDLEd;
					
			//Pickup complete!
			break;
			
		default:
			this->dropOffState = IDLEd;
	}
	
}

void Intake::raiseRackAndPinion(int newHeight)
{
	while (abs(this->intakeEncoder.getValue() - newHeight) > heightThreshold)
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
	if (abs(this->intakeEncoder.getValue() - newHeight) < heightThreshold)
	{
		lastHeight = newHeight;
	}
}

Motor Intake::getRackAndPinionMotor()
{
	return this->intakeMotor;
}

Encoder& Intake::getRackAndPinionEncoder()
{
	//Serial.print(intakeEncoder.getA());
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

String Intake::getStateString()
{
	return this->state;
}

void Intake::resetRack()
{
	while(this->lowLimitSwitch.read() != HIGH) // This MUST bottom out!
	{
		//Output <0 to go down
		this->intakeMotor.output(currentMotorOutput);
	}
}