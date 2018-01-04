/*
 * Color.h
 *
 *  Created on: Dec 4, 2017
 *      Author: Joseph Chang
 */
 
#ifndef COLOR_H
#define COLOR_H

#include <Arduino.h>


#define _BlueRMax 0
#define _BlueRMin 0
#define _BlueGMax 0
#define _BlueGMin 0
#define _BlueBMax 0
#define _BlueBMin 0

#define _GreenRMax 0
#define _GreenRMin 0
#define _GreenGMax 0
#define _GreenGMin 0
#define _GreenBMax 0
#define _GreenBMin 0

#define _RedRMax 0
#define _RedRMin 0
#define _RedGMax 0
#define _RedGMin 0
#define _RedBMax 0
#define _RedBMin 0

#define _CyanRMax 0
#define _CyanRMin 0
#define _CyanGMax 0
#define _CyanGMin 0
#define _CyanBMax 0
#define _CyanBMin 0

#define _MagentaRMax 0
#define _MagentaRMin 0
#define _MagentaGMax 0
#define _MagentaGMin 0
#define _MagentaBMax 0
#define _MagentaBMin 0

#define _YellowRMax 0
#define _YellowRMin 0
#define _YellowGMax 0
#define _YellowGMin 0
#define _YellowBMax 0
#define _YellowBMin 0

#define _GrayRMax 0
#define _GrayRMin 0
#define _GrayGMax 0
#define _GrayGMin 0
#define _GrayBMax 0
#define _GrayBMin 0

#define _BlueAngle 45
#define _GreenAngle 90
#define _RedAngle 135
#define _CyanAngle 180
#define _MagentaAngle 225
#define _YellowAngle 270
#define _GrayAngle 315

class Color {
 
public:

	Color();
	
	Color(String color);
	
	Color(float r, float g, float b);
	
	bool isBetween(float x, float low, float high)
	
	String getColorName();
	
	void setColor();
	
	double getAngle();

	
private:

	double angle;
	
	String colorName;

};

#endif
 