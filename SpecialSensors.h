#ifndef SPECIAL_SENSORS_H
#define SPECIAL_SENSORS_H

Adafruit_BNO055 gyro;
Adafruit_TCS34725 colorSensor(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

Color color;
float redRaw;
float greenRaw;
float blueRaw;
float pitch;
float roll;
float yaw;

void updateGyro()
{
	delay(60);  // takes 50ms to read 
	
	//Get the data from the gyro
	imu::Vector<3> euler = gyro.getVector(Adafruit_BNO055::VECTOR_EULER);
	
	pitch = euler.x();
	roll = euler.y();
	yaw = euler.z();
}

void updateColorSensor()
{
	uint16_t clear, red, green, blue;

	delay(60);  //takes 50ms to read 
	
	//Get the data from the color sensor
	colorSensor.getRawData(&red, &green, &blue, &clear);	
	
	redRaw = red;
	greenRaw = green;
	blueRaw = blue;	
	
	color = Color(red, green, blue);
}

#endif