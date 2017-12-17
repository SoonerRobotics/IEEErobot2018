/*
 * Color.cpp
 *
 *  Created on: Dec 4, 2017
 *      Author: Joseph Chang
 */

#include "Color.h"

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
	
	else
	{
		angle = 0;
	}
}

/**
 * getAngle method
 *  
 * @returns angle : the angle associated with the color
 */
double Color::getAngle() {
	
	return angle;
}
