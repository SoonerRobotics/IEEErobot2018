#ifndef ROBOTGLOBALS_H
#define ROBOTGLOBALS_H

/**
 * The purpose of this header file is to be able to include all known pins 
 * with one include statement.
 */

 //Left Drivetrain Motor
#define pinLeftMot1 15
#define pinLeftMot2 16
#define pinLeftMotEnb 11

//Right Drivetrain Motor
#define pinRightMot1 24
#define pinRightMot2 25
#define pinRightMotEnb 10

//I2C addresses for Gyro and Color Sensor
#define pinColor1 0		//should have same pin on i2c chain
#define pinColor2 0
#define pinGyro1 0	 	//should have same pin on i2c chain
#define pinGyro2 0

//Left Drivetrain Encoder
#define pinLeftEnc1 5
#define pinLeftEnc2 2
#define leftEncoderConstant 0.0093986//-0.0242797

//Right Drivetrain Encoder
#define pinRightEnc1 3
#define pinRightEnc2 4
#define rightEncoderConstant 0.0093986

//Intake Encoder
#define pinIntakeEnc1 28
#define pinIntakeEnc2 18
#define intakeEncoderConstant -0.002302351

//Metal Detector, Electromagnet, Limit Switch (Intake DIO)
#define pinMetDet 52 //make sure it's giving an output
#define pinElecMag 17 //make sure it's giving an output
#define pinLowLimSwitch 19
#define pinHighLimSwitch 29

//Black/White Threshold
#define BW_THRESHOLD 500

//Servos
#define colorServoPin 38

//Intake Motor Pins
#define pinIntakeMot1 22
#define pinIntakeMot2 23
#define pinIntakeMotEnb 13

//Stepper Motor Pins
#define pinStepperMotStep 49
#define pinStepperMotDir 50
#define pinStepperEnable 32
#define stepperMotRPM 50

//Turn PID while driving forward
#define TURN_KP .009 //.004
#define TURN_KI 0
#define TURN_KD 0

//Turn in place PID
#define IN_PLACE_KP .003
#define IN_PLACE_KI 0
#define IN_PLACE_KD 0

//Distance PID
#define DIST_KP .027
#define DIST_KI .00001
#define DIST_KD 0

//Go Button
#define pinGoButton 39//Set Later

#define distMetalDetectToIntake 5.5
#define distIntakeToMatrix 3.75

/*
//Colors
Color blue("blue");
Color green("green");
Color red("red");	
Color cyan("cyan");
Color magenta("magenta");
Color yellow("yellow");
Color gray("gray");
*/

#endif