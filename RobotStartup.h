#ifndef ROBOT_STARTUP_H
#define ROBOT_STARTUP_H

//Global variables
Drivetrain drivetrain;
Intake intake;		

//Encoder Masking
void encLeftInterrupt() 
{
	drivetrain.getLeftEncoder().process(); 
}
  
void encRightInterrupt() 
{  
	drivetrain.getRightEncoder().process(); 
}

//Setup Function
void robotSetup()
{
	Serial.begin(9600);
	Serial.print(" -Has Begun- \n");
	
	Collection<float> turnK(3);
	Collection<float> distanceK(3);
	
	//Add Turn Constants
	turnK.add(TURN_KP);
	turnK.add(TURN_KI);
	turnK.add(TURN_KD);
	
	//Add Distance Constants
	distanceK.add(DIST_KP);
	distanceK.add(DIST_KI);
	distanceK.add(DIST_KD);
	
	//Initialize Gyro and Color Sensor
	if(!gyro.begin())
	{
		Serial.println("Error Connecting to Gyro!");
	}
	
	//Reset the gyro
	updateGyro();
	resetGyro();
	
	if(!colorSensor.begin())
	{
		Serial.println("Error Connecting to Color Sensor!");
	}
	
	//--Drivetrain
	Motor leftMot;
	Motor rightMot;
	Encoder leftEnc(pinLeftEnc1, pinLeftEnc2);
	Encoder rightEnc(pinRightEnc1, pinRightEnc2);
	DigitalDevice mDetector(pinMetDet, INPUT);
	IRMatrix mat(pinIRMatrix1, pinIRMatrix2, pinIRMatrix3, pinIRMatrix4, pinIRMatrix5);

	//Set the encoder constants
	leftEnc.setConstant(rightEncoderConstant);
	rightEnc.setConstant(rightEncoderConstant);

	//--Drivetrain
	leftMot.begin(pinLeftMot1, pinLeftMot2, pinLeftMotEnb);
	rightMot.begin(pinRightMot1, pinRightMot2, pinRightMotEnb);
	drivetrain.begin(leftMot, rightMot, leftEnc, rightEnc, mat, mDetector);
	
	//Setup PID
	drivetrain.initializeTurnPID(turnK, 0.3, -0.3);
	drivetrain.initializeDistancePID(distanceK, 0.3, -0.3);
	
	Serial.print(" -Drive Has Begun- \n");
	
	//--Intake
	Encoder tEncoder(pinIntakeEnc1, pinIntakeEnc2);
	DigitalDevice lSwitch(pinLimSwitch, INPUT);
	Electromagnet eMagnet(pinElecMag);
	Motor iMotor;	
	Turntable turntable(turntableServoPin);
	
	//--Intake
	iMotor.begin(pinIbtakeMot1, pinIbtakeMot2, pinIbtakeMotEnb);
	eMagnet.initialize(pinElecMag);
	intake.begin(iMotor, tEncoder, mDetector, lSwitch, eMagnet, turntable, colorServoPin);
  
	Serial.print(" -Intake Has Begun- \n");
	
	//--Interrupts
	attachInterrupt(0, encLeftInterrupt, CHANGE);
	attachInterrupt(1, encRightInterrupt, CHANGE);
	
	//Interrupt for Turntable Encoder needed + method
	Serial.print(" -Interrupts- \n");
}

#endif