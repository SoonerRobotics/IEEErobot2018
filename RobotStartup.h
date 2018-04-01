#ifndef ROBOT_STARTUP_H
#define ROBOT_STARTUP_H

//Global variables
StepperMotorDrivetrain drivetrain;
Intake intake;

//Line follower
SensorBar lineFollower(0x3E);

//Drive Status Flag
bool driveComplete = false;
bool resetDrive = true;

//Distance to coin dynamic tracker
float distanceToCoin = 0;




//Global variables that aren't supposed to be global, but have to be for setup to work
//--Drivetrain
DigitalDevice metalDetector(pinMetDet, INPUT);
//IRMatrix mat(pinIRMatrix1, pinIRMatrix2, pinIRMatrix3, pinIRMatrix4, pinIRMatrix5, pinIRMatrix6, pinIRMatrix7, pinIRMatrix8, pinIRMatrixLED);
DigitalDevice goButton(pinGoButton,INPUT);

Encoder tEncoder(pinIntakeEnc1, pinIntakeEnc2);

//--Intake
DigitalDevice loSwitch(pinLowLimSwitch, INPUT);
DigitalDevice hiSwitch(pinHighLimSwitch, INPUT);
Electromagnet eMagnet(pinElecMag);
Motor iMotor;	
Turntable turntable;
StepperMotor stepperMot;

void encIntakeInterrupt() 
{  
	intake.getRackAndPinionEncoder().process(); 
}

void intakeLowLimit()
{
	intake.bottomLimit();
}

void commandTransition()
{
	delay(500);
}

//Setup Function
void robotSetup()
{
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

	//Initialize motors and basic drivetrain functions
	drivetrain.initLeft(LEFT_IN_1, LEFT_IN_2, LEFT_IN_3, LEFT_IN_4);
	drivetrain.initRight(RIGHT_IN_1, RIGHT_IN_2, RIGHT_IN_3, RIGHT_IN_4);
	drivetrain.setRPM(DRIVE_RPM);
	
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
	intake.begin(iMotor, tEncoder, metalDetector, loSwitch, hiSwitch, eMagnet, turntable, colorServoPin);
  
	Serial.print(" -Intake Has Begun- \n");
	
	attachInterrupt(digitalPinToInterrupt(pinIntakeEnc2), encIntakeInterrupt, CHANGE);
	attachInterrupt(digitalPinToInterrupt(pinLowLimSwitch), intakeLowLimit, RISING);
	Serial.print(" -Interrupts- \n");
	
	
	//Don't leave setup until GO button is pressed
	goButton.pullUp();
	int signal = goButton.read();
	do{
		signal = goButton.read();
	}while(signal != 0);
	
	//Delay a bit to allow people to move hand off button
	delay(2000);
	
	//Set the turntable and color sensor to idle
	//intake.turntable.setPosition();
	//intake.setColorServoIdle();
	
	//Reset the intake to start
	//intake.reset();
}

bool colorScanned = false;
bool doneDrive = false;
bool doneResetDrive1 = false;
bool doneResetDrive2 = false;
bool doneTurn = false;
bool finishedPickingUp = false;
bool resetDriveHere = true;

void sitStillPickup() 
{
	int status = 0;
	
	while(intake.getIntakeReturn() != 2) 
	{
		updateColorSensor();
		
		if(!finishedPickingUp && status != 2)
		{
			status = intake.pickUpSequence(currentColor, colorScanned);
		}
		
		if(intake.getIntakeReturn() == 2)
		{
			colorScanned = false;
			finishedPickingUp = true;
		}
		else if(intake.getIntakeReturn() == 1)
		{
			colorScanned = true;
		}
		else
		{
			delay(50);
		}
	}
}


#endif