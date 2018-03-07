#ifndef INTAKECONSTANTS_H
#define INTAKECONSTANTS_H
//Important heights in the pickup sequence
#define idleHeight 0
#define pickUpHeight 0
#define topHeight 0
#define scanHeight 0	

//Important heights in the dropoff sequence

//Use idleHeight
#define storageHeight 0

//Use topHeight
#define dropHeight 0	

#define heightTreshold 1

//Angles for the color sensor
#define colorServoIdleAngle 0
#define colorServoDeployAngle 0

//Speeds for the motor to move at during the pickup sequence
#define motorSpeed .35
#define stallSpeed 0

//TODO: Add more speeds to optimize the process
//Time delays in the pickup sequence
#define magnetWaitTime 0
#define turnTableWaitMax 0
#define colorServoDelay 0

#endif