#ifndef COLORSENSOR_H_
#define COLORSENSOR_H__H

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_Sensors/Adafruit_Sensor.h"
#include "Adafruit_Sensors/Adafruit_TCS34725.h"
#include "Adafruit_Sensors/utility/imumaths.h"

class ColorSensor {
 
public:

	ColorSensor();
	
	void getColor();

};

#endif /* COLORSENSOR_H_ */
 