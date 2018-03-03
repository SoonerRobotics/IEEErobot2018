#ifndef SPECIAL_SENSORS_H
#define SPECIAL_SENSORS_H

Adafruit_BNO055 gyro;
Adafruit_TCS34725 colorSensor(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

Color currentColor;

float redRaw;
float greenRaw;
float blueRaw;

//Colors
Color blue("blue");
Color green("green");
Color red("red");	
Color cyan("cyan");
Color magenta("magenta");
Color yellow("yellow");
Color gray("gray");

float pitch = 0;
float roll = 0;
float yaw = 0;
float pitchOffset = 0, rollOffset = 0, yawOffset = 0;

void updateGyro()
{
	delay(0);  // takes 50ms to read 
	
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

	delay(0);  //takes 50ms to read 
	
	//Get the data from the color sensor
	colorSensor.getRawData(&red, &green, &blue, &clear);	
		
	//Do calculations on raw colors to convert to more readable values
	float r = (int)(((float)red/(float)clear) * 256);
	float rr = r / 255;
	float rrr = pow(rr, 2.5);
	float rrrr = (int)(rrr * 255);
	
	float g = (int)(((float)green/(float)clear) * 256);
	float gg = g / 255;
	float ggg = pow(gg, 2.5);
	float gggg = (int)(ggg * 255);
	
	float b = (int)(((float)blue/(float)clear) * 256);
	float bb = b / 255;
	float bbb = pow(bb, 2.5);
	float bbbb = (int)(bbb * 255);	
	
	//set the current color reference to color currently reading
	currentColor.setColor(rrrr, gggg, bbbb);
	//currentColor = Color(rrrr, gggg, bbbb);
}

#endif