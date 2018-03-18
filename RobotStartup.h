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
	pinMode(45, OUTPUT);
	digitalWrite(45, LOW);
	
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
	Serial.println("Connecting to Gyro");
	
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
	Serial.println("y u do dis to me");
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
	stepperMot.begin(pinStepperMotStep, pinStepperMotDir);
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
		Serial.println(signal);
	}while(signal != 0);
	
	//Set the turntable to idle
//	intake.turntable.setPosition();
	//intake.setColorServoIdle();
	
	//Reset the intake to start
	//intake.reset();
}

#endif