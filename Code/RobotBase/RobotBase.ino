#include "RobotLib/Collection.h"
#include "RobotLib/BasicDrive.h"

DriveTrain drivetrain;

void setup()
{
	Collection leftM, leftE, rightM, rightE;
	
	//Left Encoder
	
	//Left Motor
	
	//Right Encoder
	
	//Right Motor
	
	drivetrain.begin(leftM, leftE, rightM, rightE);
}

void loop()
{
	drivetrain.setOutput(0.5, 0.5);
}