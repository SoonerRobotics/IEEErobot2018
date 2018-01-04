#ifndef COLORSENSOR_H_
#define COLORSENSOR_H__H

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_Sensors/Adafruit_Sensor.h"
#include "Adafruit_Sensors/Adafruit_TCS34725.h"
#include "Adafruit_Sensors/utility/imumaths.h"
#include "Color.h"

class ColorSensor {
 
public:

	ColorSensor();
	
	void operator=(const ColorSensor& colorSensor);
	
	void updateData();
	
	float getRed();
	float getGreen();
	float getBlue();
	
	Color returnColorObject();
	
	//void reset();
	
private:

	Adafruit_TCS34725 colorSensor;
	
	Color c;
	
	float redRaw;
	float greenRaw;
	float blueRaw;
	
};

#endif /* COLORSENSOR_H_ */
 