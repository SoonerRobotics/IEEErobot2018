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

class Turntable {

public:
	
	Turntable();

private:
	
	Servo servo;
	 
};

#endif /* TURNTABLE_H_ */