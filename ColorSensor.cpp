#include "ColorSensor.h"

ColorSensor::ColorSensor() {
	
	this->colorSensor.begin();

}

void ColorSensor::operator=(const Adafruit_TCS34725& colorSensor) {
	this->colorSensor = colorSensor;
}

void ColorSensor::updateData()
{
	uint16_t clear, red, green, blue;

	delay(60);  // takes 50ms to read 
	
	//Get the data from the color sensor
	colorSensor.getRawData(&red, &green, &blue, &clear);	
	
	this->redRaw = red;
	this->greenRaw = green;
	this->blueRaw = blue;
}

float ColorSensor::getRed() {
	//returns raw red value of color
	return this->redRaw;
}

float ColorSensor::getGreen() {
	//returns raw green value of color
	return this->greenRaw;
}

float ColorSensor::getBlue() {
	//returns raw blue value of color
	return this->blueRaw;
}

Color ColorSensor::getColor() {
	
	float _R = redRaw;
	float _G = greenRaw;
	float _B = blueRaw;
	
	//Use RGB values to match color to each color 
	Color c(_R,_G,_B);
	return c;
}