#ifndef INTAKE_H
#define INTAKE_H

#include <Arduino.h>
#include <RobotLib.h>
#include "Electromagnet.h"
#include "IntakeConstants.h"
#include "Turntable.h"
#include "ColorSensor.h"
#include <Servo.h>

class Intake
{
	public:
		Intake();
		
		Intake(Motor motor, Encoder encoder, DigitalDevice metalDetector, DigitalDevice limitSwitch, Electromagnet electromagnet, Turntable turnTable, Adafruit_TCS34725 colorSensor, int colorServoPin);
		
		bool pickUpSequence();
		
		void pickUpSequenceA();
		
		void dropOffSequence(Color color);
		
		bool coinDetected();
		
	private:
		enum PickUpState{IDLE, GRAB, SCAN, RAISE, STORE, DROP};
		enum DropOffState{IDLEd, GRABd, RAISEd, DROPd};
		
		PickUpState pickUpState;
		DropOffState dropOffState;
		
		IntakeConstants constants;
		
		Motor intakeMotor;
		Encoder intakeEncoder;
		
		DigitalDevice metalDetector;
		DigitalDevice limitSwitch;
		Electromagnet electromagnet;
		
		Servo colorServo;
		ColorSensor colorSensor;
		Turntable turnTable;
		
		//Most recent read from color sensor
		Color lastColor;
};

#endif