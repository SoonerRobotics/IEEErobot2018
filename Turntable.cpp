/*
 * Turntable.cpp
 *
 *  Created on: Dec 4, 2017
 *      Author: Joseph Chang
 */

#include "Turntable.h"

Turntable::Turntable()
{
	//Do nothing
	this->pin = 0;
}

Turntable::Turntable(int pinServo) 
{
	servo.attach(pinServo);
	this->pin = pinServo;
}

void Turntable::operator=(const Turntable& turnTable)
{
	servo.attach(turnTable.pin);
	this->pin = turnTable.pin;
}

void Turntable::begin(int pinServo)
{
	servo.attach(pinServo);
	this->pin = pinServo;
}

void Turntable::setPosition(Color color)
{
	int angle = color.getAngle();
	
	servo.write(angle);
}

void Turntable::setPosition(int angle)
{
	servo.write(angle);
}

void Turntable::setPosition()
{
	servo.write(IDLE_ANGLE);
}