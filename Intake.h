#ifndef INTAKE_H
#define INTAKE_H

#include <Arduino.h>
#include <RobotLib.h>
#include "Electromagnet.h"
#include "IntakeConstants.h"
#include "Turntable.h"
#include <Servo.h>
#include "PIDController.h"

class Intake
{
	public:
		Intake();
		
		void begin(Motor& motor, Encoder& encoder, DigitalDevice& metalDetector, DigitalDevice& loLimitSwitch, DigitalDevice& hiLimitSwitch, Electromagnet& electromagnet, Turntable& turnTable, int colorServoPinNumber);
		
		int pickUpSequence(Color color, bool colorScanned);
				
		void dropOffSequence(Color color);
		
		bool coinDetected();
		
		Motor getRackAndPinionMotor();
		
		Encoder& getRackAndPinionEncoder();
		
		void raiseRackAndPinion(int newHeight);
		
		Turntable turnTable;
		
		String getStateString();
		void setColorServoIdle();
		
		void resetRack();
		
		void bottomLimit();
		void topLimit();
		void reset();
		
	private:
		enum PickUpState{IDLE, GRAB, SCAN, RAISE, STORE, DROP};
		enum DropOffState{IDLEd, GRABd, RAISEd, DROPd};
		
		int lastHeight;
		
		String state;
		
		float currentMotorOutput;
		
		float coerce(float value, float high, float low);
		
		PickUpState pickUpState;
		DropOffState dropOffState;
		PIDController intakePID;
		
		Motor intakeMotor;
		Encoder intakeEncoder;
		
		DigitalDevice metalDetector;
		DigitalDevice lowLimitSwitch;
		DigitalDevice highLimitSwitch;
		Electromagnet electromagnet;
		
		Servo colorServo;
				
		//Most recent read from color sensor
		Color lastColor;
		Color avgColor;
		
		//number of samples taken from color sensor
		int numColorSamples;
		int trashedSamples;
		
		Color randColor();
};

#endif