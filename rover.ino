#include AFMotor.h
#include NewPing.h
#include Servo.h
#include TMRpcm.h
#include SPI.h
#include SD.h

#define HC05 Serial3
#define MAX_DISTANCE 25 // 25 centimeters
#define TRIGGER_PIN 30 // trigger pin = pin 50
#define ECHO_PIN 31 // echo pn = pin 51
#define MAX_DISTANCE 25 // 25 centimeters

Servo myservo; // create servo object to control a servo
Servo myservo2;

NewPing DistanceSensor(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); //create distance sensor object

AF_DCMotor FrontLeft(1);
AF_DCMotor FrontRight(4);
AF_DCMotor BackLeft(2);
AF_DCMotor BackRight(3);

char data;
char data1;
char data2;
String ne;
int place = 0;
int servo2Pos = 160;
int servoPos = 80;
int currentDistance = 999;
const int CS_PIN = 44;
boolean lightBlink = false;
TMRpcm sound;

void setup()
{
	Serial.begin(9600);
	HC05.begin(9600);
	pinMode(53, OUTPUT); // Car lights
	myservo.attach(10); // attaches the servo on pin 10 to the servo object
	myservo2.attach(9);
	SetSpeedMax();
	welcomeSpeech();
}

void loop()
{
	while (currentDistance > MAX_DISTANCE)
	{
		if (HC05.available() > 0) 
		{
			data = HC05.read();
			Serial.print("found ");
			Serial.print(data);
			switch (data)
			{
				case 'F': Forward();
				break;
				case 'B': Backward();
				break;
				case 'L': TurnLeft(); 
				break;
				case 'R': TurnRight();
				break;
				case 'S': SetSpeedMin();
				break;
				case 'e': SetSpeedMax();
				break;
				case 'i': servoUp();
				break;
				case 'k': servoDown();
				break;
				case 'j': servoLeft();
				break;
				case 'l': servoRight();
				break;
				case 'c': servoCenter();
				break;
				case 'W': digitalWrite(53, HIGH); 
				lightBlink = false;
				break;
				case 'w': digitalWrite(53, LOW);
				lightBlink = false;
				break;
				case 'b': lightBlink = true;
				break;
				case 'X': sound.play("horn.wav");
				break;
				case 'H': VServo();
				break;
				default: HC05.print("ERROR!!");
			}
		}
	
		currentDistance = DistanceSensor.ping_cm();
		if (currentDistance == 0)
		currentDistance = 999;
		if (lightBlink) 
		{
			digitalWrite(53, HIGH);
			delay(500);
			digitalWrite(53, LOW);
			delay(500);
		}
	}

	Serial.print("back");
	sound.play("back.wav");
	//backBeep();
	Backward();
	delay (1000);
	SetSpeedMin();
	sound.disable();
	currentDistance = DistanceSensor.ping_cm();
	Serial.print(currentDistance);
	if (currentDistance == 0)
	currentDistance = 999;
}

void backBeep()
{
	sound.play("back.wav");
}

void welcomeSpeech()
{
	sound.speakerPin = 46;
	Serial.println("Initializing Card");
	pinMode(CS_PIN, OUTPUT);
	if(!SD.begin(CS_PIN)) 
	{
		Serial.println("Card Failure");
		return;
	}
	Serial.println("Card Ready");
	sound.setVolume(5);
	sound.play("hello.wav");
	delay(3500);
}

void Forward ()
{
	SetSpeedMax();
	FrontLeft.run(FORWARD); 
	FrontRight.run(FORWARD);
	BackLeft.run(FORWARD);
	BackRight.run(FORWARD);
}

void Backward ()
{
	SetSpeedMed();
	//sound.play("back.wav");
	FrontLeft.run(BACKWARD); 
	FrontRight.run(BACKWARD);
	BackLeft.run(BACKWARD);
	BackRight.run(BACKWARD);
}

void TurnLeft()
{
	SetSpeedMax();
	FrontLeft.run(BACKWARD);
	BackLeft.run(BACKWARD);
	FrontRight.run(FORWARD);
	BackRight.run(FORWARD);
}

void TurnRight()
{
	SetSpeedMax();
	FrontLeft.run(FORWARD); 
	BackLeft.run(FORWARD);
	FrontRight.run(BACKWARD);
	BackRight.run(BACKWARD);
}

void SetSpeedMax()
	{
	FrontLeft.setSpeed(255);
	FrontRight.setSpeed(255);
	BackLeft.setSpeed(255);
	BackRight.setSpeed(255);
}

void SetSpeedMed()
{
	FrontLeft.setSpeed(155);
	FrontRight.setSpeed(155);
	BackLeft.setSpeed(155);
	BackRight.setSpeed(155);
}

void SetSpeedMin()
{
	FrontLeft.setSpeed(0);
	FrontRight.setSpeed(0);
	BackLeft.setSpeed(0);
	BackRight.setSpeed(0);
}

void Stop()
{
	FrontLeft.run(BRAKE);
	FrontRight.run(BRAKE);
	BackLeft.run(BRAKE);
	BackRight.run(BRAKE);
}

void servoUp()
{
	servo2Pos++;
	myservo2.write(servo2Pos);
}

void servoDown()
{
	Serial.write("in down");
	myservo2.write(180);
	//servo2Pos=servo2Pos-1;
}

void servoLeft()
{
	myservo.write(servoPos+1);
	servoPos=servoPos+1;
}

void servoRight()
{
	myservo.write(servoPos-1);
	servoPos=servoPos-1;
}

void servoCenter()
{
	myservo.write(80);
	myservo2.write(60);
}

void VServo()
{
	Serial.println("in V");
	if (HC05.available() > 2) 
	{
		char data1 = HC05.read();
		Serial.println(data1);
	}
	//String ne = data1 + data2;
	myservo.write(data1);
}

void HServo(int place)
{
	myservo.write(place);
}