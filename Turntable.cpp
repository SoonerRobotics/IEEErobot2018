/*
 * Turntable.cpp
 *
 *  Created on: Dec 4, 2017
 *      Author: Joseph Chang
 */

#include "Turntable.h"

Turntable::Turntable() {
	// TODO Auto-generated constructor stub

}

void Turntable::setPosition(Color color)
{
	servo.write(color.getAngle());
}

void Turntable::setPosition(int angle)
{
	servo.write(angle);
}

void Turntable::setPosition()
{
	servo.write(IDLE_ANGLE);
}