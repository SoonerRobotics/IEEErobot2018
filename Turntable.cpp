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
}

void Turntable::begin(StepperMotor stepperMot)
{
	this->stepperMot = stepperMot;
}

void Turntable::setPosition(Color color)
{
	
	this->stepperMot.step(color.getAngle());
}

void Turntable::setNegPosition(Color color)
{	
	this->stepperMot.step(color.getAngle() * -1);
}

void Turntable::setPosition(int steps)
{	
	this->stepperMot.step(steps);
}

void Turntable::setPosition()
{
	this->stepperMot.step(-stepperMot.getCurrentSteps());
}