#include "Gyro.h"

Gyro::Gyro() 
{	
	this->gyro.begin();
}

void Gyro::operator=(const Adafruit_BNO055& gyro)
{
	this->gyro = gyro;
}

void Gyro::updateData()
{
	delay(60);  // takes 50ms to read 
	
	//Get the data from the gyro
	imu::Vector<3> euler = this->gyro.getVector(Adafruit_BNO055::VECTOR_EULER);
	
	this->pitch = euler.x();
	this->roll = euler.y();
	this->yaw = euler.z();
}

float Gyro::getPitch() 
{	
	//Return the pitch
	return this->pitch;
}

float Gyro::getRoll() 
{	
	//Return the roll
	return this->roll;
}

float Gyro::getYaw() 
{	
	//Return the yaw
	return this->yaw;
}