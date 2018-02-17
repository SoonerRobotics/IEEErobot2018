#ifndef INTAKE_H
#define INTAKE_H

#include <Arduino.h>
#include <RobotLib.h>
#include "Electromagnet.h"
#include "IntakeConstants.h"
#include "Turntable.h"
#include <Servo.h>

class Intake
{
	public:
		Intake();
		
		void begin(Motor motor, Encoder encoder, DigitalDevice metalDetector, DigitalDevice limitSwitch, Electromagnet electromagnet, Turntable turnTable, int colorServoPin);
		
		bool pickUpSequence(Color color);
		
		void pickUpSequenceA(Color color);
		
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
		Turntable turnTable;
		
		//Most recent read from color sensor
		Color lastColor;
};

#endif