#include <Wire.h>
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "Adafruit_TCS34725.h"
#include "utility/imumaths.h"

Adafruit_BNO055 gyro = Adafruit_BNO055();
Adafruit_TCS34725 color = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {

	gyro.begin();
	color.begin();
  
	Serial.begin(9600);
  
}

void loop() {

	uint16_t clear, red, green, blue;
	
	delay(60);  // takes 50ms to read 
	
	color.getRawData(&red, &green, &blue, &clear);
	
	imu::Vector<3> euler = gyro.getVector(Adafruit_BNO055::VECTOR_EULER);

	Serial.print("X: ");
	Serial.print(euler.x());
	Serial.print(" Y: ");
	Serial.print(euler.y());
	Serial.print(" Z: ");
	Serial.print(euler.z());
	Serial.println("\t\t");
	
	Serial.print("C:\t"); 
	Serial.print(clear);
	Serial.print("\tR:\t"); 
	Serial.print(red);
	Serial.print("\tG:\t"); 
	Serial.print(green);
	Serial.print("\tB:\t"); 
	Serial.print(blue);
}
