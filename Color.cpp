/*
 * Color.cpp
 *
 *  Created on: Dec 4, 2017
 *      Author: Joseph Chang
 */

#include "Color.h"


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


/**
 * Color constructor
 * 
 * @param color : name of color 
 */
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

/**
 * setColor method
 * 
 * sets a new color for this object
 */
void Color::setColor(String color) {
	
	if (color.equals("blue"))
	{
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
double Color::getAngle() {
	
	return angle;
}
