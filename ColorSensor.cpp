#include "ColorSensor.h"

Adafruit_TCS34725 thisColorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

ColorSensor::ColorSensor(void) {
	
	thisColorSensor.begin();

}

void ColorSensor::getColor() {
	
	uint16_t clear, red, green, blue;
	
	delay(60);  // takes 50ms to read 
	
	thisColorSensor.getRawData(&red, &green, &blue, &clear);

	Serial.print("C:\t"); 
	Serial.print(clear);
	Serial.print("\tR:\t"); 
	Serial.print(red);
	Serial.print("\tG:\t"); 
	Serial.print(green);
	Serial.print("\tB:\t"); 
	Serial.print(blue);
	
}

//method to return color based upon RGB values... need testing...
