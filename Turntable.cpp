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
	steps = 0;
	this->stepperMot = stepperMot;
}

void Turntable::setPosition(Color color)
{
	int steps = (color.getAngle()/360)*200;
	
	this->stepperMot.step(steps);
}

void Turntable::setPosition(int angle)
{
	int steps = (angle/360)*200;
	
	this->stepperMot.step(steps);
}

void Turntable::setPosition()
{
	this->stepperMot.step(-stepperMot.getCurrentSteps());
}