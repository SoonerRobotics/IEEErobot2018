#include <SoftwareSerial.h>
#define magPin 9

// SERIAL PORT VARS
char cString[50];  // array that will hold command string 
int iBufferIndex = 0; // index of buffer characters rec'd
int iBuffer = 0;

int icount = 0;
int magnetPin = 9;     // Control pin for servo motor

void setup() {
	Serial.begin(9600);         // connect to the serial port
	pinMode(magPin, OUTPUT);
	digitalWrite(magPin, LOW);
}

void loop() {
	readSerialString();  // read, wait for command
	processSerialString(); // process command, either pan or tilt
	//digitalWrite(magPin, HIGH);
}

void readSerialString () {
	
	while (Serial.available ())
		{
		iBufferIndex = 0;
		iBuffer = Serial.read();

		cString[iBufferIndex]  = iBuffer;
		iBufferIndex++;
		cString[iBufferIndex]= '\0';
		delay(10);  //never learned why this is necessary, but won't work without it
		}
   
}

void processSerialString() {
	if( iBufferIndex > 0) {
 
		if(cString[0] == '0' )
		{
			digitalWrite(magnetPin, LOW);
		
			Serial.println();
			Serial.print("Magnet is: ");
			Serial.print("OFF");
			
		}

		else if(cString[0] == '1' )
		{
			digitalWrite(magnetPin, HIGH);
		
			Serial.println();
			Serial.print("Magnet is: ");
			Serial.print("ON");
			
		}

	
	} // if string isn't 0
 
}