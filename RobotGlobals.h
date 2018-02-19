#ifndef ROBOTPINS_H
#define ROBOTPINS_H

/**
 * The purpose of this header file is to be able to include all known pins 
 * with one include statement.
 */

 //Left Drivetrain Motor
#define pinLeftMot1 24
#define pinLeftMot2 25
#define pinLeftMotEnb 10

//Right Drivetrain Motor
#define pinRightMot1 26
#define pinRightMot2 23
#define pinRightMotEnb 11

//I2C addresses for Gyro and Color Sensor
#define pinColor1 0		//should have same pin on i2c chain
#define pinColor2 0
#define pinGyro1 0	 	//should have same pin on i2c chain
#define pinGyro2 0

//Left Drivetrain Encoder
#define pinLeftEnc1 1
#define pinLeftEnc2 2

//Right Drivetrain Encoder
#define pinRightEnc1 3
#define pinRightEnc2 4

//Intake Encoder
#define pinIntakeEnc1 0
#define pinIntakeEnc2 0

//Metal Detector, Electromagnet, Limit Switch (Intake DIO)
#define pinMetDet 0 //make sure it's giving an output
#define pinElecMag 0 //make sure it's giving an output
#define pinLimSwitch 0

//Pins for the IR Matrix
#define pinIRMatrix1 A0
#define pinIRMatrix2 A1
#define pinIRMatrix3 A2
#define pinIRMatrix4 A3
#define pinIRMatrix5 A4

//Servos
#define turntableServoPin 0
#define colorServoPin 0

//Intake Motor Pins
#define pinIbtakeMot1 0
#define pinIbtakeMot2 0
#define pinIbtakeMotEnb 0


#endif