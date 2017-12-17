/*
 * Color.h
 *
 *  Created on: Dec 4, 2017
 *      Author: Joseph Chang
 */
 
#ifndef COLOR_H_
#define COLOR_H

#include <Arduino.h>

class Color {
 
public:

	Color();
	
	void setColor();
	
	double getAngle(String color);
	
private:

	double angle;

};

#endif /* COLOR_H_ */
 