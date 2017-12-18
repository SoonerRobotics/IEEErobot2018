#include "Electromagnet.h"

Electromagnet::Electromagnet()
{

}

Electromagnet::Electromagnet(int pin) : DigitalDevice(pin, OUTPUT)
{

}

void Electromagnet::initialize(int pin)
{
	DigitalDevice::update(pin, OUTPUT);
}

void Electromagnet::pickUp()
{
	DigitalDevice::write(HIGH);
}

void Electromagnet::drop()
{
	DigitalDevice::write(LOW);
}