#ifndef ROBOT_STARTUP_H
#define ROBOT_STARTUP_H

//Global variables
Drivetrain drivetrain;
Intake intake;

//Global variables that aren't supposed to be global, but have to be for setup to work
//--Drivetrain
Motor leftMot;
Motor rightMot;
DigitalDevice mDetector(pinMetDet, INPUT);
//IRMatrix mat(pinIRMatrix1, pinIRMatrix2, pinIRMatrix3, pinIRMatrix4, pinIRMatrix5, pinIRMatrix6, pinIRMatrix7, pinIRMatrix8, pinIRMatrixLED);
DigitalDevice goButton(pinGoButton,INPUT);

Encoder leftEnc(pinLeftEnc1, pinLeftEnc2);
Encoder rightEnc(pinRightEnc1, pinRightEnc2);
Encoder tEncoder(pinIntakeEnc1, pinIntakeEnc2);

//--Intake
DigitalDevice loSwitch(pinLowLimSwitch, INPUT);
DigitalDevice hiSwitch(pinHighLimSwitch, INPUT);
Electromagnet eMagnet(pinElecMag);
Motor iMotor;	
Turntable turntable;
StepperMotor stepperMot;

//Line follower
SensorBar lineFollower(0x3E);

//Encoder Masking
void encLeftInterrupt() 
{
	drivetrain.getLeftEncoder().process(); 
}
  
void encRightInterrupt() 
{  
	drivetrain.getRightEncoder().process(); 
}

void encIntakeInterrupt() 
{  
	intake.getRackAndPinionEncoder().process(); 
}

void intakeLowLimit()
{
	intake.bottomLimit();
}

//Setup Function
void robotSetup()
{
	//Virtual grounds and virtual power
	pinMode(45, OUTPUT);
	digitalWrite(45, LOW);
	
	pinMode(53, OUTPUT);
	digitalWrite(53, HIGH);
	
	Serial.begin(9600);
	Serial.print(" -Has Begun- \n");

	//Initialize Gyro and Color Sensor
	if(!gyro.begin())
	{
		Serial.println("Error Connecting to Gyro");
	}
	Serial.println("Connecting to Gyro");
	//Reset the gyro
	updateGyro();
	resetGyro();
	
	if(!colorSensor.begin())
	{
		Serial.println("Error Connecting to Color Sensor");
	}
	Serial.println("Connecting to Color Sensor");
	
	//Setup line follower
	lineFollower.setBarStrobe();
	lineFollower.clearInvertBits();
	if(!lineFollower.begin())
	{
		Serial.println("Sensor bar connection error");
	}
	Serial.println("Connecting to Sensor bar");
	
	
	
	//Set the encoder constants
	leftEnc.setConstant(leftEncoderConstant);
	rightEnc.setConstant(rightEncoderConstant);

	//Initialize motors and basic drivetrain functions
	leftMot.begin(pinLeftMot1, pinLeftMot2, pinLeftMotEnb);
	rightMot.begin(pinRightMot1, pinRightMot2, pinRightMotEnb);
	drivetrain.begin(leftMot, rightMot, leftEnc, rightEnc, mDetector);
	
	//Setup PID
	drivetrain.initializeTurnPID(TURN_KP, TURN_KI, TURN_KD, 0.3, -0.3);
	drivetrain.initializeDistancePID(DIST_KP, DIST_KI, DIST_KD, 0.4, -0.4);
	
	Serial.print(" -Drive Has Begun- \n");
	
	//Add pull down resistors to low and high limit switches
	loSwitch.pullUp();
	hiSwitch.pullUp();
	
	//--Intake
	tEncoder.setConstant(intakeEncoderConstant);
	
	//--Intake
	iMotor.begin(pinIntakeMot1, pinIntakeMot2, pinIntakeMotEnb);
	eMagnet.initialize(pinElecMag);
	stepperMot.begin(pinStepperMotStep, pinStepperMotDir, pinStepperEnable);
	stepperMot.setRPM(stepperMotRPM);
	turntable.begin(stepperMot);
	intake.begin(iMotor, tEncoder, mDetector, loSwitch, hiSwitch, eMagnet, turntable, colorServoPin);
  
	Serial.print(" -Intake Has Begun- \n");
	
	//--Interrupts
	attachInterrupt(0, encLeftInterrupt, CHANGE);
	attachInterrupt(1, encRightInterrupt, CHANGE);
	attachInterrupt(digitalPinToInterrupt(pinIntakeEnc2), encIntakeInterrupt, CHANGE);
	attachInterrupt(digitalPinToInterrupt(pinLowLimSwitch), intakeLowLimit, RISING);
	
	//Interrupt for Turntable Encoder needed + method
	Serial.print(" -Interrupts- \n");
	
	
	//Don't leave setup until GO button is pressed
	goButton.pullUp();
	int signal = goButton.read();
	do{
		signal = goButton.read();
	}while(signal != 0);
	
	//Set the turntable to idle
//	intake.turntable.setPosition();
	//intake.setColorServoIdle();
	
	//Reset the intake to start
	//intake.reset();
}

bool colorScanned = false;
void sitStillPickup() 
{
	
	//drive from metal detector to magnet
	drivetrain.drive(distMetalDetectToIntake, 0, yaw, true);
	
	while(intake.getIntakeReturn() != 2) 
	{
		updateColorSensor();
		
		intake.pickUpSequence(currentColor, colorScanned);
		
		if(intake.getIntakeReturn() == 2)
		{
			colorScanned = false;
		}
		else if (intake.getIntakeReturn() == 1)
		{
			colorScanned = true;
		}
		else
		{
			delay(50);
		}
	}
	
	//drive from magnet to metal detector (or to sensor bar)
	drivetrain.drive(distIntakeToMatrix, 0, yaw, true);
}

bool doneDrive = false;
void pickupDrive() 
{	
	//drive from metal detector to magnet
	drivetrain.drive(distMetalDetectToIntake, 0, yaw, true);

	while(intake.getIntakeReturn() !=2 && !doneDrive)
	{
		updateColorSensor();
		
		if(intake.getIntakeReturn() !=2)
		{
			intake.pickUpSequence(currentColor, colorScanned);
		}
		
		if(intake.getIntakeReturn() == 2)
		{
			colorScanned = false;
		}
		else if(intake.getIntakeReturn() == 1)
		{
			colorScanned=true;
		}
		else
		{
			delay(50);
		}
		
		if(!doneDrive && intake.getIntakeReturn() != 0)
		{
			//drive from magnet to metal detector (or to sensor bar)
			drivetrain.drive(distIntakeToMatrix, 0, yaw, true);
			doneDrive = drivetrain.followLineUntilCoin(lineFollower.getDensity(), lineFollower.getPosition(), yaw);
		}
	}
}

bool doneTurn = false;
void pickUpTurnDrive(int turnAngle) 
{
	//drive from metal detector to magnet
	drivetrain.drive(distMetalDetectToIntake, 0, yaw, true);
	
	while(!intake.coinDetected() && intake.getIntakeReturn() != 2)
	{
		updateColorSensor();
		
		if(intake.getIntakeReturn() !=2)
		{
			intake.pickUpSequence(currentColor, colorScanned);
		}
		
		if(intake.getIntakeReturn() == 2)
		{
			colorScanned = false;
		}
		else if(intake.getIntakeReturn() == 1)
		{
			colorScanned = true;
		}
		else
		{
			delay(50);
		}
		
		if(!doneTurn && intake.getIntakeReturn() != 0)
		{
			//drive from magnet to metal detector (or to sensor bar)
			drivetrain.drive(distIntakeToMatrix, 0, yaw, true);
			doneTurn = drivetrain.drive(0, turnAngle, yaw, true);
		}
		else if(doneTurn && !doneDrive && intake.getIntakeReturn() != 0)
		{
			doneDrive = drivetrain.followLineUntilCoin(lineFollower.getDensity(), lineFollower.getPosition(), yaw);
		}
		
	}
}

#endif