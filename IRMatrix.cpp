#include "IRMatrix.h"

/**
 * Pin 1 is VCC, Pin 7 is Ground
 * Format is [Eagle Pin]IR#:Arduino Ain
 *
 *				[2]1:A0
 *
 * [4]2:A1 	[3]3:A2	[5]4:A3
 *
 *				[6]5:A4
 */


IRMatrix::IRMatrix()  : values(5)
{
	this->binaryValue = 0;
}

IRMatrix::IRMatrix(int pin1, int pin2, int pin3, int pin4, int pin5)  : values(5)
{
	this->ir1.update(pin1, INPUT);
	this->ir2.update(pin2, INPUT);
	this->ir3.update(pin3, INPUT);
	this->ir4.update(pin4, INPUT);
	this->ir5.update(pin5, INPUT);
	
	this->binaryValue = 0;
	
	Serial.print(" -Matrix Construct- \n");
}

void IRMatrix::operator=(const IRMatrix& matrix)
{
	this->ir1 = matrix.ir1;
	this->ir2 = matrix.ir2;
	this->ir3 = matrix.ir3;
	this->ir4 = matrix.ir4;
	this->ir5 = matrix.ir5;
	Serial.print(" -Matrix Copy-Assign- \n");
	
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

unsigned int IRMatrix::readToBinary()
{
	this->binaryValue = 0;
	
	this->binaryValue = convertToBinary(this->ir1.read());
	this->binaryValue += convertToBinary(this->ir2.read()) * 2;
	this->binaryValue += convertToBinary(this->ir3.read()) * 4;
	this->binaryValue += convertToBinary(this->ir4.read()) * 8;
	this->binaryValue += convertToBinary(this->ir5.read()) * 16;
	
	return this->binaryValue;
}

void IRMatrix::printRawToSerial()
{
	Serial.print(this->ir1.read());
	Serial.print("\t");
	Serial.print(this->ir2.read());
	Serial.print("\t");
	Serial.print(this->ir3.read());
	Serial.print("\t");
	Serial.print(this->ir4.read());
	Serial.print("\t");
	Serial.print(this->ir5.read());
	Serial.print("\n");
}

/**
 * Private Functions Below
 */
 
 void IRMatrix::readAll()
 {
	 values.set(0, convertToBinary(this->ir1.read()) == HIGH);
	 values.set(1, convertToBinary(this->ir2.read()) == HIGH);
	 values.set(2, convertToBinary(this->ir3.read()) == HIGH);
	 values.set(3, convertToBinary(this->ir4.read()) == HIGH);
	 values.set(4, convertToBinary(this->ir5.read()) == HIGH);
 }
 
 int IRMatrix::convertToBinary(int raw)
 {
	 if(raw >= BW_THRESHOLD)
	 {
		 return 0;
	 }
	 return 1;
 }