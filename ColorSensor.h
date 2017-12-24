#ifndef COLORSENSOR_H_
#define COLORSENSOR_H__H

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_TCS34725.h"
#include "utility/imumaths.h"

class ColorSensor {
 
public:

	ColorSensor();
	
	void getColor();

};

#endif /* COLORSENSOR_H_ */
 