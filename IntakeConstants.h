#ifndef INTAKECONSTANTS_H
#define INTAKECONSTANTS_H
//Important heights in the pickup sequence
#define idleHeight 0.0
#define pickUpHeight 0.0
#define topHeight 3.0
#define scanHeight 2.5

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
#define motorSpeed 0.35
#define stallSpeed 0
#define resetSpeed -0.4

//TODO: Add more speeds to optimize the process
//Time delays in the pickup sequence
#define magnetWaitTime 0
#define turnTableWaitMax 0
#define colorServoDelay 0


//PID for intake
#define intakeKp 0.4
#define intakeKi 0
#define intakeKd 0.002

//Amount of error allowed in height
#define RP_TOLERANCE 0.5

#endif