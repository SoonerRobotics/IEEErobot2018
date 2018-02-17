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
}


void Intake::begin(Motor motor, Encoder encoder, DigitalDevice metalDetector, DigitalDevice limitSwitch, Electromagnet electromagnet, Turntable turnTable, Adafruit_TCS34725 colorSensor, int colorServoPin)
//void Intake::begin(Motor motor, Encoder encoder, DigitalDevice metalDetector, DigitalDevice limitSwitch, Electromagnet electromagnet, Turntable turnTable, int colorServoPin)
{
	////this->intakeMotor = motor;
	this->intakeEncoder = encoder;
	this->metalDetector = metalDetector;
	this->limitSwitch = limitSwitch;
	this->electromagnet = electromagnet;
	this->turnTable = turnTable;
	//this->colorSensor = colorSensor;
	
	this->pickUpState = IDLE;
	this->dropOffState = IDLEd;
	
	this->lastColor = Color("none");
	
	this->colorServo.attach(colorServoPin);
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
				//this->intakeMotor.output((-1) * this->constants.motorSpeed);
			}
			else //Otherwise it is time to grab the coin
			{
				this->pickUpState = GRAB;
			}
			return false;
			
		case GRAB:
			//Stop the motor so we can do a pickup
			//this->intakeMotor.output(this->constants.stallSpeed);
			
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
				//this->intakeMotor.output(this->constants.motorSpeed);
			}
			else
			{
				//Stop the motor so we can scan the coin
				//this->intakeMotor.output(this->constants.stallSpeed);
				
				//Deploy the RGB color sensor! (servo)
				this->colorServo.write(this->constants.colorServoDeployAngle);
				delay(this->constants.colorServoDelay);
				
				//Detect and save the color
				//this->colorSensor.updateData();
				this->lastColor = Color("red"); //this->colorSensor.getColor();
				
				//Retract color sensor
				this->colorServo.write(this->constants.colorServoIdleAngle);
				delay(this->constants.colorServoDelay);
				
				//Move to the full raise state
				this->pickUpState = RAISE;
			}
			return false;
			
		case RAISE:
			//While the intake is below the max height and the limit switch is not pressed
			if(this->intakeEncoder.getValue() < this->constants.topHeight && this->limitSwitch.read() == LOW)
			{
				//Output >0 to go up
				//this->intakeMotor.output(this->constants.motorSpeed);
			}
			else
			{
				//Stop driving the motor upwards
				//this->intakeMotor.output(this->constants.stallSpeed);
				
				//We have reached the height, so time to store.
				this->pickUpState = STORE;
			}
			
			//Process unfinished
			return false;
			
		case STORE:
			
			if(this->electromagnet.hasCoin())
			{
				//Turn the turntable so it is ready to receive the coin
				this->turnTable.setPosition(this->lastColor);
				delay(this->constants.turnTableWaitMax);
				
				//Once the turntable is ready, drop the coin
				this->electromagnet.drop();
				
				//Wait for the coin to drop
				delay(this->constants.magnetWaitTime);
				
				//Tell the turntable to go back to its idle state
				this->turnTable.setPosition();
				delay(this->constants.turnTableWaitMax);
				
				//Process unfinished
				return false;
			}
			//Check to see if the turntable is ready to let the intake drop
			else if(!this->electromagnet.hasCoin())
			{
				//Drive the intake down to the idle height
				if(this->intakeEncoder.getValue() > this->constants.idleHeight)
				{
					//Output <0 to go down
					//this->intakeMotor.output((-1) * this->constants.motorSpeed);
					
					//Almost done, but not quite.
					return false;
				}
				else
				{
					//Stop driving the motor downwards
					//this->intakeMotor.output(this->constants.stallSpeed);
					
					//set the sequence to idle
					this->pickUpState = IDLE;
					
					//Pickup complete!
					return true;
				}
			}
			
		default:
			this->pickUpState = IDLE;
			return false;
	}
	
}

void Intake::pickUpSequenceA()
{	
	
	switch(this->pickUpState)
	{
		case IDLE:
			//If the intake is above the grab height, drop the magnet to that height
			if(this->intakeEncoder.getValue() > this->constants.pickUpHeight)
			{
				//Output <0 to go down
				//this->intakeMotor.output((-1) * this->constants.motorSpeed);
			}
			else //Otherwise it is time to grab the coin
			{
				this->pickUpState = GRAB;
			}
			
		case GRAB:
			//Stop the motor so we can do a pickup
			//this->intakeMotor.output(this->constants.stallSpeed);
			
			//Pick up the coin
			this->electromagnet.pickUp();
			
			//Wait to make sure it is picked up
			delay(this->constants.magnetWaitTime);
			
			//Begin the process of scanning the coin
			this->pickUpState = SCAN;
			
		case SCAN:
			//Raise the intake to scanning height
			if(this->intakeEncoder.getValue() < this->constants.scanHeight)
			{
				//Output >0 to go up
				//this->intakeMotor.output(this->constants.motorSpeed);
			}
			else
			{
				//Stop the motor so we can scan the coin
				//this->intakeMotor.output(this->constants.stallSpeed);
				
				//Deploy the RGB color sensor! (servo)
				this->colorServo.write(this->constants.colorServoDeployAngle);
				delay(this->constants.colorServoDelay);
				
				//Detect and save the color
				//this->colorSensor.updateData();
				this->lastColor = Color("red");//this->colorSensor.getColor();
				
				//Retract color sensor
				this->colorServo.write(this->constants.colorServoIdleAngle);
				delay(this->constants.colorServoDelay);
				
				//Move to the full raise state
				this->pickUpState = RAISE;
			}
			
		case RAISE:
			//While the intake is below the max height and the limit switch is not pressed
			if(this->intakeEncoder.getValue() < this->constants.topHeight && this->limitSwitch.read() == LOW)
			{
				//Output >0 to go up
				//this->intakeMotor.output(this->constants.motorSpeed);
			}
			else
			{
				//Stop driving the motor upwards
				//this->intakeMotor.output(this->constants.stallSpeed);
				
				//We have reached the height, so time to store.
				this->pickUpState = STORE;
			}
			
		case STORE:
			
			if(this->electromagnet.hasCoin())
			{
				//Turn the turntable so it is ready to receive the coin
				this->turnTable.setPosition(this->lastColor);
				delay(this->constants.turnTableWaitMax);
				
				//Once the turntable is ready, drop the coin
				this->electromagnet.drop();
				
				//Wait for the coin to drop
				delay(this->constants.magnetWaitTime);
				
				//Tell the turntable to go back to its idle state
				this->turnTable.setPosition();
				delay(this->constants.turnTableWaitMax);
				
			}
			//Check to see if the turntable is ready to let the intake drop
			else if(!this->electromagnet.hasCoin())
			{
				//Drive the intake down to the idle height
				if(this->intakeEncoder.getValue() > this->constants.idleHeight)
				{
					//Output <0 to go down
					//this->intakeMotor.output((-1) * this->constants.motorSpeed);
					
					//Almost done, but not quite.
					return false;
				}
				else
				{
					//Stop driving the motor downwards
					//this->intakeMotor.output(this->constants.stallSpeed);
					
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
			if(this->intakeEncoder.getValue() > this->constants.storageHeight)
			{
				//Output <0 to go down
				//this->intakeMotor.output((-1) * this->constants.motorSpeed);
			}
			else //Otherwise it is time to grab the coin from storage
			{
				this->dropOffState = GRABd;
			}
			
		case GRABd:
			//Turn the turntable to the coin storage of given color
			this->turnTable.setPosition(color);
			delay(this->constants.turnTableWaitMax);
			
			//Stop the motor so we can do a pickup
			//this->intakeMotor.output(this->constants.stallSpeed);
			
			//Pick up the coins from storage
			this->electromagnet.pickUp();
			
			//Wait to make sure it is picked up
			delay(this->constants.magnetWaitTime);
			
			//Begin the process of RAISE
			this->dropOffState = RAISEd;
			
		case RAISEd:
			//While the intake is below the max height and the limit switch is not pressed
			if(this->intakeEncoder.getValue() < this->constants.topHeight && this->limitSwitch.read() == LOW)
			{
				//Output >0 to go up
				//this->intakeMotor.output(this->constants.motorSpeed);
			}
			else
			{
				//Stop driving the motor upwards
				//this->intakeMotor.output(this->constants.stallSpeed);
				
				//We have reached the height, so time to drop off the coins.
				this->dropOffState = DROPd;
			}
			
		case DROPd:
			
			if(this->electromagnet.hasCoin())
			{
				//Turn the turntable so the coins can pass through
				this->turnTable.setPosition(0);
				delay(this->constants.turnTableWaitMax);
				
				//Once the turntable is ready, drop the magnet to drop height
				if(this->intakeEncoder.getValue() > this->constants.dropHeight)
				{
					//Output <0 to go down
					//this->intakeMotor.output((-1) * (this->constants.motorSpeed));
				}
				else if(this->intakeEncoder.getValue() < this->constants.dropHeight)
				{
					//Once the turntable is ready, drop the coin
					this->electromagnet.drop();
				
					//Wait for the coin to drop
					delay(this->constants.magnetWaitTime);
				}
				
				this->turnTable.setPosition();
				delay(this->constants.turnTableWaitMax);
			}
			//Check to see if the turntable is ready to let the intake drop
			else if(!this->electromagnet.hasCoin())
			{
				//Return the magnet to above the turntable
				if(this->intakeEncoder.getValue() < this->constants.idleHeight)
				{
					//Output >0 to go up
					//this->intakeMotor.output(this->constants.motorSpeed);
				}
				else
				{
					//Stop driving the motor downwards
					//this->intakeMotor.output(this->constants.stallSpeed);
					
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

bool Intake::coinDetected()
{
	return this->metalDetector.read() == HIGH;
}