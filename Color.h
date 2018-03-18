/*
 * Color.h
 *
 *  Created on: Dec 4, 2017
 *      Author: Joseph Chang
 */
 
#ifndef COLOR_H
#define COLOR_H

#include <Arduino.h>


#define _BlueRMax 5
#define _BlueRMin 1
#define _BlueGMax 16
#define _BlueGMin 12
#define _BlueBMax 46
#define _BlueBMin 37

#define _GreenRMax 14
#define _GreenRMin 10
#define _GreenGMax 32
#define _GreenGMin 28
#define _GreenBMax 10
#define _GreenBMin 6

#define _RedRMax 100
#define _RedRMin 85
#define _RedGMax 5
#define _RedGMin 1
#define _RedBMax 5
#define _RedBMin 1

#define _CyanRMax 4
#define _CyanRMin 0
#define _CyanGMax 28
#define _CyanGMin 24
#define _CyanBMax 32
#define _CyanBMin 28

#define _MagentaRMax 20
#define _MagentaRMin 10
#define _MagentaGMax 20
#define _MagentaGMin 15
#define _MagentaBMax 15
#define _MagentaBMin 10

#define _YellowRMax 32
#define _YellowRMin 24
#define _YellowGMax 30
#define _YellowGMin 20
#define _YellowBMax 8
#define _YellowBMin 3

#define _GrayRMax 16
#define _GrayRMin 9
#define _GrayGMax 21
#define _GrayGMin 17
#define _GrayBMax 17
#define _GrayBMin 13

#define _StartingAngle 0
#define _RedAngle 25
#define _GreenAngle 50
#define _BlueAngle 75
#define _CyanAngle 100
#define _MagentaAngle -75
#define _YellowAngle -50
#define _GrayAngle -25


class Color {
 
	public:
		Color();
		
		Color(String color);
		
		Color(float r, float g, float b);
		
		void operator=(const Color& c);
		Color operator+(const Color& c);
		Color operator/(float divisor);
		
		String getColorName();
		
		void setColor(String color);
		
		void setColor(float r, float g, float b);
		
		double getAngle();
		
		float getRed();
		float getGreen();
		float getBlue();
		
		void reset();
	
	private:
	
		float R, G, B;
	
		double angle;
		
		String colorName;
		
		bool isBetween(float x, float low, float high);

};

#endif