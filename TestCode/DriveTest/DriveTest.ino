//The sample code for driving one way motor encoder
const byte encoder0pinA = 18;//A pin -> the interrupt pin 0
const byte encoder0pinB = 19;//B pin -> the digital pin 4
byte encoder0PinALast;
int duration;//the number of the pulses
boolean Direction;//the rotation direction 

void setup() {
	pinMode(9, OUTPUT);
	pinMode(10, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
    Serial.begin(57600);//Initialize the serial port
	EncoderInit();//Initialize the module
}

void loop() {
  
	digitalWrite(10, LOW);

	//for(int i = 0; i < 250; i = i + 10){
    analogWrite(9, 100);
	//delay(200);}
	
	digitalWrite(5, LOW);

	//for(int i = 0; i < 250; i = i + 10){
    analogWrite(6, 100);
	//delay(200);}

	Serial.print("Pulse:");
	Serial.println(duration);
	duration = 0;
	delay(100);
}

void EncoderInit()
{
  Direction = true;//default -> Forward  
  pinMode(encoder0pinB,INPUT);  
  attachInterrupt(0, wheelSpeed, CHANGE);
}

void wheelSpeed()
{
  int Lstate = digitalRead(encoder0pinA);
  if((encoder0PinALast == LOW) && (Lstate==HIGH))
  {
    int val = digitalRead(encoder0pinB);
    if(val == LOW && Direction)
    {
      Direction = false; //Reverse
    }
    else if(val == HIGH && !Direction)
    {
      Direction = true;  //Forward
    }
  }
  encoder0PinALast = Lstate;
 
  if(!Direction)  duration++;
  else  duration--;
}
