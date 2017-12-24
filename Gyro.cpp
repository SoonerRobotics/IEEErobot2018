#include "Gyro.h"

Adafruit_BNO055 thisGyro = Adafruit_BNO055();

Gyro::Gyro() {
	
	thisGyro.begin();

}


void Gyro::getAngle() {
	
	delay(60);  // takes 50ms to read 
	
	imu::Vector<3> euler = thisGyro.getVector(Adafruit_BNO055::VECTOR_EULER);

	Serial.print("X: ");
	Serial.print(euler.x());
	Serial.print(" Y: ");
	Serial.print(euler.y());
	Serial.print(" Z: ");
	Serial.print(euler.z());
	Serial.println("\t\t");

}
