#ifndef IR_MATRIX_H
#define IR_MATRIX_H

#include <Arduino.h>
#include <DigitalDevice.h>

class IRMatrix
{
	public:
		IRMatrix();
		IRMatrix(int pin1, int pin2, int pin3, int pin4, int pin5);
		void operator=(const IRMatrix& matrix);
		
		void begin(int pin1, int pin2, int pin3, int pin4, int pin5);
		
		Collection<bool> getValues();
		unsigned int readToBinary();
		
	private:
		DigitalDevice ir1;
		DigitalDevice ir2;
		DigitalDevice ir3;
		DigitalDevice ir4;
		DigitalDevice ir5;
		
		Collection<bool> values(5);
		
		int binaryValue;
		
		void readAll();
};

#endif