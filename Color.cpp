/*
 * Color.cpp
 *
 *  Created on: Dec 4, 2017
 *      Author: Joseph Chang
 */

#include "Color.h"

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
//etc...

Color::Color(float r, float g, float b) {
		
	//BLUE
	if (isBetween(r,_BlueRMax,_BlueRMin) && isBetween(g,_BlueGMax,_BlueGMin) && isBetween(b,_BlueBMax,_BlueBMin)) {
		angle = (360/8)*(1);
		colorName = "blue";
	}
	//GREEN
	else if (isBetween(r,_GreenRMax,_GreenRMin) && isBetween(g,_GreenGMax,_GreenGMin) && isBetween(b,_GreenBMax,_GreenBMin)) {
		angle = (360/8)*(2);
		colorName = "green";
	}
	//RED
	else if (isBetween(r,_RedRMax,_RedRMin) && isBetween(g,_RedGMax,_RedGMin) && isBetween(b,_RedBMax,_RedBMin)) {
		angle = (360/8)*(3);
		colorName = "red";
	}
	//CYAN
	else if (isBetween(r,_CyanRMax,_CyanRMin) && isBetween(g,_CyanGMax,_CyanGMin) && isBetween(b,_CyanBMax,_CyanBMin)) {
		angle = (360/8)*(4);
		colorName = "cyan";
	}
	//MAGENTA
	else if (isBetween(r,_MagentaRMax,_MagentaRMin) && isBetween(g,_MagentaGMax,_MagentaGMin) && isBetween(b,_MagentaBMax,_MagentaBMin)) {
		angle = (360/8)*(5);
		colorName = "magenta";
	}
	//YELLOW
	else if (isBetween(r,_YellowRMax,_YellowRMin) && isBetween(g,_YellowGMax,_YellowGMin) && isBetween(b,_YellowBMax,_YellowBMin)) {
		angle = (360/8)*(6);
		colorName = "yellow";
	}
	//GRAY
	else if (isBetween(r,_GrayRMax,_GrayRMin) && isBetween(g,_GrayGMax,_GrayGMin) && isBetween(b,_GrayBMax,_GrayBMin)) {
		angle = (360/8)*(7);
		colorName = "gray";
	}
	else
	{
		angle = 0;
	}
}

Color::Color(String color) {
	
	if (color.equals("blue"))
	{
		//360/7 divides up 360 angles in circle for 7 color
		//multiplied by the order of the color (blue is the first color, but order can change)
		angle = (360/8)*(1);
	}
	else if (color.equals("green"))
	{
		angle = (360/8)*(2);
	}
	
	else if (color.equals("red"))
	{
		angle = (360/8)*(3);
	}
	
	else if (color.equals("cyan"))
	{
		angle = (360/8)*(4);
	}
	
	else if (color.equals("magenta"))
	{
		angle = (360/8)*(5);
	}
	
	else if (color.equals("yellow"))
	{
		angle = (360/8)*(6);
	}
	
	else if (color.equals("gray"))
	{
		angle = (360/8)*(7);
	}
	else
	{
		angle = 0;
	}
}

bool Color::isBetween(float x,float low,float high) {
	if (x > low && x < high) {
		return true;
	} else {
		return false;
	}
}

String Color::getColorName() {
	return colorName;
}

/**
 * getAngle method
 *  
 * @returns angle : the angle associated with the color
 */
 
 /*
 TODO: Fix this to match with header file
 
double Color::getAngle() {
	
	return angle;
}
*/