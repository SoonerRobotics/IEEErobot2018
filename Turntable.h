/*
 * Turntable.h
 *
 *  Created on: Dec 4, 2017
 *      Author: Joseph Chang
 */

#ifndef TURNTABLE_H_
#define TURNTABLE_H_

#include <Arduino.h>
#include <Servo.h>
#include "Color.h"

#define IDLE_ANGLE 0

class Turntable {

public:
	Turntable();
	Turntable(int pinServo);
	
	void begin(int pinServo);
	void operator=(const Turntable& turnTable);
	
	void setPosition(Color color);
	void setPosition(int angle);
	void setPosition();

private:
	int pin;
	Servo servo;
	 
};

#endif /* TURNTABLE_H_ */