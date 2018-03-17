#ifndef DRIVECONSTANTS_H
#define DRIVECONSTANTS_H

#define setpointTimeout 100 //How long the robot needs to be at the setpoint to complete the drive

#define stopSpeedThreshold 0.4 //Max speed for robot to be considered stopped at setpoint

#define distanceThreshold .5 //Window for robot to be in range of distance setpoint.

#define angleThreshold 3 //Window for robot to be in range of angle setpoint.

#define lineFollowSpeed .20//Speed to drive when line following

#define lineTurnSpeed .20 //Speed to turn when correcting for line following errors

#define lineFollowP 0.5

#endif