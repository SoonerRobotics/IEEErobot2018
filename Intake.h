#ifndef INTAKE_H
#define INTAKE_H

#include <Arduino.h>

#include <DigitalDevice.h>
#include "Electromagnet.h"
#include "IntakeConstants.h"
#include <Motor.h>
#include <MotorController.h>
#include "Turntable.h"
#include "ColorSensor.h"
#include <Servo.h>

class Intake
{
	public:
		Intake();
		Intake(Motor motor, Encoder encoder, DigitalDevice metalDetector, DigitalDevice limitSwitch, Electromagnet electromagnet, Turntable turnTable, ColorSensor colorSensor, int colorServoPin);
		
		void pickUpSequence();
		
		bool coinDetected();
		
		void setConstants(IntakeConstants K);
		
	private:
		enum PickUpState{IDLE, GRAB, SCAN, RAISE, STORE};
		
		PickUpState pickUpState;
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