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
	//this->servo.attach(pinServo);
	this->pin = pinServo;
}

void Turntable::operator=(const Turntable& turnTable)
{
	//this->servo = turnTable.servo;
	this->pin = turnTable.pin;
}

void Turntable::begin(int pinServo)
{
	//this->servo.attach(pinServo);
	this->pin = pinServo;
}

void Turntable::setPosition(Color color)
{
	int angle = color.getAngle();
	
	this->servo.writeMicroseconds(angle);
}

void Turntable::setPosition(int angle)
{
	this->servo.writeMicroseconds(angle);
}

void Turntable::setPosition()
{
	this->servo.writeMicroseconds(IDLE_ANGLE);
}