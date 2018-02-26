#ifndef SPECIAL_SENSORS_H
#define SPECIAL_SENSORS_H

Adafruit_BNO055 gyro;
Adafruit_TCS34725 colorSensor(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

Color color;
float redRaw;
float greenRaw;
float blueRaw;

float pitch = 0;
float roll = 0;
float yaw = 0;
float pitchOffset = 0, rollOffset = 0, yawOffset = 0;

void updateGyro()
{
	delay(60);  // takes 50ms to read 
	
	//Get the data from the gyro
	imu::Vector<3> euler = gyro.getVector(Adafruit_BNO055::VECTOR_EULER);
	
	pitch = euler.y() - pitchOffset;
	roll = euler.z() - rollOffset;
	yaw = euler.x() - yawOffset;
	
	//Map yaw to (-180, 180]
	if(yaw > 180)
	{
		yaw = -(360 - yaw);
	}
}

void resetGyro()
{
	rollOffset = roll;
	pitchOffset = pitch;
	yawOffset = yaw;
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

int convertToUsable() {
	
}
#endif