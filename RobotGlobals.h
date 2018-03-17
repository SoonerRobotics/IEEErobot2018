#ifndef ROBOTGLOBALS_H
#define ROBOTGLOBALS_H

/**
 * The purpose of this header file is to be able to include all known pins 
 * with one include statement.
 */

 //Left Drivetrain Motor
#define pinLeftMot1 24
#define pinLeftMot2 25
#define pinLeftMotEnb 10

//Right Drivetrain Motor
#define pinRightMot1 15
#define pinRightMot2 16
#define pinRightMotEnb 11

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
#define pinMetDet 0 //make sure it's giving an output
#define pinElecMag 17 //make sure it's giving an output
#define pinLowLimSwitch 19
#define pinHighLimSwitch 29

//Black/White Threshold
#define BW_THRESHOLD 500

//Servos
#define turntableServoPin 7
#define colorServoPin 38

//Intake Motor Pins
#define pinIntakeMot1 22
#define pinIntakeMot2 23
#define pinIntakeMotEnb 13

//Turn PID
#define TURN_KP .004
#define TURN_KI 0
#define TURN_KD 0

//Distance PID
#define DIST_KP .027
#define DIST_KI .00001
#define DIST_KD 0

//Go Button
#define pinGoButton 39//Set Later

#define distMetalDetectToIntake 0
#define distIntakeToMatrix 0

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