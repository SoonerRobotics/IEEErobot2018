#ifndef INTAKECONSTANTS_H
#define INTAKECONSTANTS_H
//Important heights in the pickup sequence
#define idleHeight 0.0
#define pickUpHeight 0.0
#define topHeight 3
#define scanHeight 2

//Important heights in the dropoff sequence
//Use idleHeight
#define getFromStorageHeight 0
//Use topHeight
#define dropHeight 3.0	
//

#define heightThreshold 1

//Angles for the color sensor
#define colorServoIdleAngle -5
#define colorServoDeployAngle 110

//Speeds for the motor to move at during the pickup sequence
#define motorSpeed 0.6
#define stallSpeed 0
#define resetSpeed -0.4

//TODO: Add more speeds to optimize the process
//Time delays in the pickup sequence
#define magnetWaitTime 200
#define turnTableWaitMax 150
#define colorServoDeployDelay 1500
#define colorServoRetractDelay 200

//PID for intake
#define intakeKp 0.4
#define intakeKi 0
#define intakeKd 0.002

//Amount of error allowed in height
#define RP_TOLERANCE 0.5

//Number of color samples to take to decide on a color
#define MIN_COLOR_SAMPLES 6

//Don't accept samples with an RGB component over this number
#define COLOR_MAX_VALUE 200

#endif