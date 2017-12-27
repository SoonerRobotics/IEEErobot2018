#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_Sensors/Adafruit_Sensor.h"
#include "Adafruit_Sensors/Adafruit_BNO055.h"
#include "Adafruit_Sensors/utility/imumaths.h"

class Gyro
{
	public:
		Gyro();
		
		void operator=(const Gyro& gyro);
		
		void updateData();
		float getPitch();
		float getRoll();
		float getYaw();
		
		void reset();
	
	private:
		Adafruit_BNO055 gyro;
		
		float pitch;
		float roll;
		float yaw;
		
};

#endif