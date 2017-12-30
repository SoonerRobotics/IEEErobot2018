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
	
	Color(String color);
	
	Color::Color(float r, float g, float b);
	
	//void setColor();
	
	//double getAngle(String color);
	
	bool isBetween(float x, float low, float high)
	
	String getColorName();
	
private:

	double angle;
	
	String colorName;

};

#endif /* COLOR_H_ */
 