#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <Arduino.h>
#include <Collection.h>
#include <BasicDrive.h>
#include <PIDController.h>
#include <DigitalDevice.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <sensorbar.h>

#include "DriveConstants.h"
#include "RobotGlobals.h"

class Drivetrain : public BasicDrive
{
	public:
		Drivetrain();
		
		void begin(Motor& leftMot, Motor& rightMot, Encoder& leftEnc, Encoder& rightEnc, DigitalDevice& mDetector);
		void initializeTurnPID(float kp, float ki, float kd, float high = 1, float low = -1);
		void initializeDistancePID(float kp, float ki, float kd, float high = 1, float low = -1);
		
		bool drive(float targetDistance, float targetAngle, float inputYaw, bool reinitialize, long timeout = -1);
		
		void followLineRaw(int density, int raw);
		
		void followLine(int density, int position, float yaw);
		
		bool followLineUntilCoin(int density, int position, float yaw);
		
		bool searchForward(int density, float yaw);
		
		void followLineGyro();
		
		float getPositionSpark();
		float getTurnSpeed();
		
		void pathFollower(int density, int raw);
		
		void driveIndefinitely(float speed, float targetAngle, float inputYaw, bool reinitialize);

		
		float getLeftOutput();
		float getRightOutput();
		
	private:
		DigitalDevice metDetector;
		
		PIDController turnPID;
		PIDController distancePID;
		
		//PID turnPID_v1;
		//PID distancePID_v1;
		
		//PID turnPID_v1(gyroError, turnPIDOutput, targetDistance, TURN_KP, TURN_KI, TURN_KD, 0);
		//PID distancePID_v1(distance, distancePIDOutput, 0, DIST_KP, DIST_KI, DIST_KD, 0);
		
		void arcadeDrive(float Y, float X);
		void boostedArcadeDrive(float Y, float X);
		
		void makeDecision();
		
		float rawToIDAverage(int raw);
		
		//drive() specific private vars
		float Y;
		float X;
		float distance;
		float gyroError;
		
		float highT;
		float lowT;
		float highD;
		float lowD;
		
		float distancePIDOutput;
		float turnPIDOutput;
		
		float targetDistance;
		float targetAngle;	
		
		bool distanceInRange;
		bool angleInRange;
		bool driveComplete;
		bool turnComplete;
		
		long timer;
		long timeoutClock;
		long angleTimer;
		long distanceTimer;
		long angleTimerElapsed;
		long distanceTimerElapsed;
		
		bool movementComplete;
		
		//Line following vars
		float lastTurnSpeed;
		
		String step = "";
		
		enum Direction{LEFT, RIGHT, STRAIGHT, NONE};
		Direction lastDirection;
		float lastLeft;
		float lastRight;
	
		float globalLeft;
		float globalRight;
		
		float angleTicks;
		
		float lastRaw;
	
};


#endif