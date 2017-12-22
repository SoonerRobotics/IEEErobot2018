#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Gyro
{
	public:
		Gyro();
		
		void operator=(const Gyro& gyro);
		
		float getAngle();
		
		void process();
		
		void reset();
	
	private:
		
		
};

#endif