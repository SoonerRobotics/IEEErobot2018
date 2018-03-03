#ifndef IR_MATRIX_H
#define IR_MATRIX_H

#include <Arduino.h>
#include <AnalogDevice.h>
#include "RobotGlobals.h"

class IRMatrix
{
	public:
		IRMatrix();
		IRMatrix(int pin1, int pin2, int pin3, int pin4, int pin5);
		void operator=(const IRMatrix& matrix);
		
		void begin(int pin1, int pin2, int pin3, int pin4, int pin5);
	
		unsigned int readToBinary();
		
		void printRawToSerial();
		
	private:
		AnalogDevice ir1;
		AnalogDevice ir2;
		AnalogDevice ir3;
		AnalogDevice ir4;
		AnalogDevice ir5;
		
		int binaryValue;
		
		void readAll();
		
		int convertToBinary(int raw);
};

#endif