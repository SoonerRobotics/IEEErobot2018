#include "Electromagnet.h"

Electromagnet::Electromagnet()
{
	this->status = false;
}

Electromagnet::Electromagnet(int pin) : DigitalDevice(pin, OUTPUT)
{
	this->status = false;
}

void Electromagnet::initialize(int pin)
{
	DigitalDevice::update(pin, OUTPUT);
}

void Electromagnet::pickUp()
{
	DigitalDevice::write(HIGH);
	this->status = true;
}

void Electromagnet::drop()
{
	DigitalDevice::write(LOW);
	this->status = false;
}

bool Electromagnet::hasCoin()
{
	return this->status;
}