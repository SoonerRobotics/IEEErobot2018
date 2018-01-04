#include "IRMatrix.h"

/**
 * Pin 1 is VCC, Pin 7 is Ground
 * Format is [Eagle Pin]IR#
 *
 *			[2]1
 *
 * [4]2 	[3]3	[5]4
 *
 *			[6]5
 */


IRMatrix::IRMatrix()
{
	this->binaryValue = 0;
}

IRMatrix::IRMatrix(int pin1, int pin2, int pin3, int pin4, int pin5)
{
	this->ir1.update(pin1, INPUT);
	this->ir2.update(pin2, INPUT);
	this->ir3.update(pin3, INPUT);
	this->ir4.update(pin4, INPUT);
	this->ir5.update(pin5, INPUT);
	
	this->binaryValue = 0;
}

void IRMatrix::operator=(const IRMatrix& matrix)
{
	this->ir1 = matrix.ir1;
	this->ir2 = matrix.ir2;
	this->ir3 = matrix.ir3;
	this->ir4 = matrix.ir4;
	this->ir5 = matrix.ir5;
	
	this->values = matrix.values;
	this->binaryValue = matrix.binaryValue;
}

void IRMatrix::begin(int pin1, int pin2, int pin3, int pin4, int pin5)
{
	this->ir1.update(pin1, INPUT);
	this->ir2.update(pin2, INPUT);
	this->ir3.update(pin3, INPUT);
	this->ir4.update(pin4, INPUT);
	this->ir5.update(pin5, INPUT);
}

Collection<bool> IRMatrix::getValues()
{
	readAll();
	return this->values;
}

int IRMatrix::readToBinary()
{
	this->binaryValue = 0;
	
	this->binaryValue = this->ir1.read();
	this->binaryValue += this->ir2.read() * 2;
	this->binaryValue += this->ir3.read() * 4;
	this->binaryValue += this->ir4.read() * 8;
	this->binaryValue += this->ir5.read() * 16;
	
	return this->binaryValue;
}

/**
 * Private Functions Below
 */
 
 void IRMatrix::readAll()
 {
	 values.set(0, this->ir1.read() == HIGH);
	 values.set(1, this->ir2.read() == HIGH);
	 values.set(2, this->ir3.read() == HIGH);
	 values.set(3, this->ir4.read() == HIGH);
	 values.set(4, this->ir5.read() == HIGH);
 }