/*
 * Color.cpp
 *
 *  Created on: Dec 4, 2017
 *      Author: Joseph Chang
 */

#include "Color.h"

Color::Color()
{
	
}

Color::Color(float r, float g, float b) {
		
	//BLUE
	if (isBetween(r,_BlueRMax,_BlueRMin) && isBetween(g,_BlueGMax,_BlueGMin) && isBetween(b,_BlueBMax,_BlueBMin)) {
		this->angle = _BlueAngle;
		this->colorName = "blue";
	}
	//GREEN
	else if (isBetween(r,_GreenRMax,_GreenRMin) && isBetween(g,_GreenGMax,_GreenGMin) && isBetween(b,_GreenBMax,_GreenBMin)) {
		this->angle = _GreenAngle;
		this->colorName = "green";
	}
	//RED
	else if (isBetween(r,_RedRMax,_RedRMin) && isBetween(g,_RedGMax,_RedGMin) && isBetween(b,_RedBMax,_RedBMin)) {
		this->angle = _RedAngle;
		this->colorName = "red";
	}
	//CYAN
	else if (isBetween(r,_CyanRMax,_CyanRMin) && isBetween(g,_CyanGMax,_CyanGMin) && isBetween(b,_CyanBMax,_CyanBMin)) {
		this->angle = _CyanAngle;
		this->colorName = "cyan";
	}
	//MAGENTA
	else if (isBetween(r,_MagentaRMax,_MagentaRMin) && isBetween(g,_MagentaGMax,_MagentaGMin) && isBetween(b,_MagentaBMax,_MagentaBMin)) {
		this->angle = _MagentaAngle;
		this->colorName = "magenta";
	}
	//YELLOW
	else if (isBetween(r,_YellowRMax,_YellowRMin) && isBetween(g,_YellowGMax,_YellowGMin) && isBetween(b,_YellowBMax,_YellowBMin)) {
		this->angle = _YellowAngle;
		this->colorName = "yellow";
	}
	//GRAY
	else if (isBetween(r,_GrayRMax,_GrayRMin) && isBetween(g,_GrayGMax,_GrayGMin) && isBetween(b,_GrayBMax,_GrayBMin)) {
		this->angle = _GrayAngle;
		this->colorName = "gray";
	}
	else
	{
		this->angle = _StartingAngle;
		this->colorName = "n/a";
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
		this->angle = _BlueAngle;
		this->colorName = "blue";
	}
	else if (color.equals("green"))
	{
		this->angle = _GreenAngle;
		this->colorName = "green";
	}
	else if (color.equals("red"))
	{
		this->angle = _RedAngle;
		this->colorName = "red";
	}
	else if (color.equals("cyan"))
	{
		this->angle = _CyanAngle;
		this->colorName = "cyan";
	}
	else if (color.equals("magenta"))
	{
		this->angle = _MagentaAngle;
		this->colorName = "magenta";
	}
	else if (color.equals("yellow"))
	{
		this->angle = _YellowAngle;
		this->colorName = "yellow";
	}
	else if (color.equals("gray"))
	{
		this->angle = _GrayAngle;
		this->colorName = "gray";
	}
	else
	{
		this->angle = 0;
	}
	
	//this->color = color;
}


void Color::operator=(const Color& c)
{
	this->angle = c.angle;
	this->colorName = c.colorName;
}

/**
 * setColor method
 * 
 * sets a new color for this object
 */
void Color::setColor(String color) {
	
	if (color.equals("blue"))
	{
		this->angle = _BlueAngle;
		this->colorName = "blue";
	}
	else if (color.equals("green"))
	{
		this->angle = _GreenAngle;
		this->colorName = "green";
	}
	else if (color.equals("red"))
	{
		this->angle = _RedAngle;
		this->colorName = "red";
	}
	else if (color.equals("cyan"))
	{
		this->angle = _CyanAngle;
		this->colorName = "cyan";
	}
	else if (color.equals("magenta"))
	{
		this->angle = _MagentaAngle;
		this->colorName = "magenta";
	}
	else if (color.equals("yellow"))
	{
		this->angle = _YellowAngle;
		this->colorName = "yellow";
	}
	else if (color.equals("gray"))
	{
		this->angle = _GrayAngle;
		this->colorName = "gray";
	}
	else
	{
		this->angle = 0;
	}
	
	//this->color = color;
	
}

void Color::setColor(float r, float g, float b) {
		
	//BLUE
	if (isBetween(r,_BlueRMax,_BlueRMin) && isBetween(g,_BlueGMax,_BlueGMin) && isBetween(b,_BlueBMax,_BlueBMin)) {
		this->angle = _BlueAngle;
		this->colorName = "blue";
	}
	//GREEN
	else if (isBetween(r,_GreenRMax,_GreenRMin) && isBetween(g,_GreenGMax,_GreenGMin) && isBetween(b,_GreenBMax,_GreenBMin)) {
		this->angle = _GreenAngle;
		this->colorName = "green";
	}
	//RED
	else if (isBetween(r,_RedRMax,_RedRMin) && isBetween(g,_RedGMax,_RedGMin) && isBetween(b,_RedBMax,_RedBMin)) {
		this->angle = _RedAngle;
		this->colorName = "red";
	}
	//CYAN
	else if (isBetween(r,_CyanRMax,_CyanRMin) && isBetween(g,_CyanGMax,_CyanGMin) && isBetween(b,_CyanBMax,_CyanBMin)) {
		this->angle = _CyanAngle;
		this->colorName = "cyan";
	}
	//MAGENTA
	else if (isBetween(r,_MagentaRMax,_MagentaRMin) && isBetween(g,_MagentaGMax,_MagentaGMin) && isBetween(b,_MagentaBMax,_MagentaBMin)) {
		this->angle = _MagentaAngle;
		this->colorName = "magenta";
	}
	//YELLOW
	else if (isBetween(r,_YellowRMax,_YellowRMin) && isBetween(g,_YellowGMax,_YellowGMin) && isBetween(b,_YellowBMax,_YellowBMin)) {
		this->angle = _YellowAngle;
		this->colorName = "yellow";
	}
	//GRAY
	else if (isBetween(r,_GrayRMax,_GrayRMin) && isBetween(g,_GrayGMax,_GrayGMin) && isBetween(b,_GrayBMax,_GrayBMin)) {
		this->angle = _GrayAngle;
		this->colorName = "gray";
	}
	else
	{
		this->angle = _StartingAngle;
		this->colorName = "n/a";
	}
}

bool Color::isBetween(float x,float high,float low) {
	if (x > low && x < high) {
		return true;
	} else {
		return false;
	}
}

String Color::getColorName() {
	return this->colorName;
}

/**
 * getAngle method
 *  
 * @returns angle : the angle associated with the color
 */ 
double Color::getAngle() {
	
	return angle;
}
