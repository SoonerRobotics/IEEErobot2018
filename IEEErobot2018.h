#ifndef IEEErobot2018_H
#define IEEErobot2018_H

/**
 * The purpose of this header file is to be able to include the whole library 
 * with one include statement.
 */

 //Adafruit sensors that must be included in Arduino/libraries/ for this code to work
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_TCS34725.h>

//SparkFun Line following library
#include <sensorbar.h>

//2018 specific code
#include "Color.h"
#include "Electromagnet.h"
#include "Turntable.h"
#include "Drivetrain.h"
#include "DriveConstants.h"
#include "Intake.h"
#include "IRMatrix.h"
#include "RobotGlobals.h"
#include "SpecialSensors.h"
#include "RobotStartup.h"


#endif