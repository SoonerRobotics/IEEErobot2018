#ifndef DRIVECONSTANTS_H
#define DRIVECONSTANTS_H

#define setpointTimeout 100 //How long the robot needs to be at the setpoint to complete the drive

#define stopSpeedThreshold 0.4 //Max speed for robot to be considered stopped at setpoint

#define distanceThreshold 1 //Window for robot to be in range of distance setpoint.

#define ANGLETHRESHOLD 2 //Window for robot to be in range of angle setpoint.

#define lineFollowSpeed .175//Speed to drive when line following

#define lineTurnSpeed .175 //Speed to turn when correcting for line following errors

#define lineFollowP 0.5

#define PATH_FOLLOW_SPEED 0.175
#define PATH_FOLLOW_TURN_MAX 0.4

#define RIGHT_MOTOR_POWER_OFFSET 0.05


#define driveTimeout 4

#endif