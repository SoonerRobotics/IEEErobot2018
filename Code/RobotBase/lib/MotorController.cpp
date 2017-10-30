#include "Arduino.h"
#include "MotorController.h"

MotorController::MotorController(int pin) {
	pinMode(pin, OUTPUT);
}



