#ifndef GYRO_H_
#define GYRO_H__H

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_Sensors/Adafruit_Sensor.h"
#include "Adafruit_Sensors/Adafruit_BNO055.h"
#include "Adafruit_Sensors/utility/imumaths.h"

class Gyro {
 
public:

	Gyro();
	
	void getAngle();

};

#endif /* GYRO_H_ */
 