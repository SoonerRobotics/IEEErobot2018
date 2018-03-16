#ifndef IR_MATRIX_H
#define IR_MATRIX_H

#include <Arduino.h>
#include <DigitalDevice.h>
#include "RobotGlobals.h"

class IRMatrix
{
	public:
		IRMatrix();
		IRMatrix(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7, int pin8, int pinLED);
		void operator=(const IRMatrix& matrix);
		
		void begin(int pin1, int pin2, int pin3, int pin4, int pin5, int pin6, int pin7, int pin8, int pinLED);
	
		unsigned int readToBinary();
		
		void printRawToSerial();
		
	private:
		DigitalDevice ir1;
		DigitalDevice ir2;
		DigitalDevice ir3;
		DigitalDevice ir4;
		DigitalDevice ir5;
		DigitalDevice ir6;
		DigitalDevice ir7;
		DigitalDevice ir8;
	
		DigitalDevice irLED;
		
		int binaryValue;
		
		void readAll();
		
		int convertToBinary(int raw);
};

#endif