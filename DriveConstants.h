#ifndef DRIVECONSTANTS_H
#define DRIVECONSTANTS_H

struct DriveConstants
{
	long timeout; //How long before we give up on the drive
	long setpointTimeout; //How long the robot needs to be at the setpoint to complete the drive
	
	float stopSpeedThreshold; //Max speed for robot to be considered stopped at setpoint
	
	float distanceThreshold; //Window for robot to be in range of distance setpoint.
	float angleThreshold; //Window for robot to be in range of angle setpoint.
	
	float lineFollowSpeed; //Speed to drive when line following
	float lineTurnSpeed; //Speed to turn when correcting for line following errors
};

#endif