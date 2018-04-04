#ifndef ROBOTGLOBALS_H
#define ROBOTGLOBALS_H

/**
 * The purpose of this header file is to be able to include all known pins 
 * with one include statement.
 */

 /*
 Regular motors are for the weak, use stepper motors instead
 
 //Left Drivetrain Motor
#define pinLeftMot1 16
#define pinLeftMot2 15
#define pinLeftMotEnb 11

//Right Drivetrain Motor
#define pinRightMot1 25
#define pinRightMot2 24
#define pinRightMotEnb 10
*/

//Left Drivetrain Motor pins
#define LEFT_IN_1 16
#define LEFT_IN_2 15
#define LEFT_IN_3 25
#define LEFT_IN_4 24

//Right Drivetrain Motor pins
#define RIGHT_IN_1 10
#define RIGHT_IN_2 11
#define RIGHT_IN_3 8
#define RIGHT_IN_4 7

//Drivetrain starting RPM
#define DRIVE_RPM 30
#define TURN_RPM 15

//Drivetrain distance ratio
#define DIST_RATIO 18/18

//Drivetrain turn ratio
#define TURN_RATIO 0.65

//Turning angles with turn ratio
//#define NINETY_DEG 2.875*3.141592653589*0.65
//#define ONE_THIRTY_FIVE_DEG 1.5*2.875*3.141592653589*0.65

#define NINETY_DEG 2.875*3.141592653589*0.61
#define ONE_THIRTY_FIVE_DEG 1.5*2.875*3.141592653589*0.60

//Move delay
#define MOVE_DELAY 500
#define PICK_DELAY 2000

//I2C addresses for Gyro and Color Sensor
#define pinColor1 0		//should have same pin on i2c chain
#define pinColor2 0
#define pinGyro1 0	 	//should have same pin on i2c chain
#define pinGyro2 0

//Left Drivetrain Encoder
#define pinLeftEnc1 2
#define pinLeftEnc2 5
#define leftEncoderConstant -0.0093986

//Right Drivetrain Encoder
#define pinRightEnc1 4
#define pinRightEnc2 3
#define rightEncoderConstant -0.0093986

//Intake Encoder
#define pinIntakeEnc1 28
#define pinIntakeEnc2 18
#define intakeEncoderConstant -0.002302351

//Metal Detector, Electromagnet, Limit Switch (Intake DIO)
#define pinMetDet 9 //make sure it's giving an output
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
#define pinIntakeMotEnb 12

//Stepper Motor Pins
#define pinStepperMotStep 49
#define pinStepperMotDir 50
#define pinStepperEnable 32
#define stepperMotRPM 60

//Turn PID while driving forward
#define TURN_KP .009 //.004
#define TURN_KI 0
#define TURN_KD 0

//Turn in place PID
#define IN_PLACE_KP 0.005
#define IN_PLACE_KI 0.0001
#define IN_PLACE_KD 0

//Distance PID
#define DIST_KP .03
#define DIST_KI .00001
#define DIST_KD 0

//Go Button
#define pinGoButton 39//Set Later

#define distMetalDetectToIntake 5.5
#define distIntakeToMatrix -3.75

#define METAL_DETECTOR_TO_INTAKE 5.5
#define IR_ARRAY_TO_INTAKE 3.75
#define SCOOT_FORWARD 2

//Distances
#define DIAGONAL_DISTANCE sqrt(2) * 6

#endif