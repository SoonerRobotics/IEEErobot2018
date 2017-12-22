#ifndef ELECTROMAGNET_H
#define ELECTROMAGNET_H

#include <Arduino.h>
#include <DigitalDevice.h>

class Electromagnet : public DigitalDevice
{
	public:
		Electromagnet();
		
		Electromagnet(int pin);
		
		void initialize(int pin);
		
		void pickUp();
		
		void drop();
		
		bool hasCoin();
		
	private:
		bool status;
};


#endif