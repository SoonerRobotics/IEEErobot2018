#ifndef GYRO_H
#define GYRO_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>

class Gyro
{
	public:
		Gyro();
		
		void operator=(const Adafruit_BNO055& gyro);
		
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