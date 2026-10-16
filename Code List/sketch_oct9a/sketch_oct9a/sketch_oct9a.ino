#include <Servo.h>
Servo myservo;

const int powerPin = 3;    // 회전
const int turnPin = 4;   // 바람 세기
const int moteraPin = 5;  // DC 모터 (A)
const int moterbPin = 6;  // DC 모터 (B)
const int sevorPin = 9;   // 서보모터

int pos;
int motorSpeed = 0;
int lastSwitchState = LOW;

void setup() 
{
  Serial.begin(9600);
  pinMode(powerPin, INPUT);
  pinMode(turnPin, INPUT);
  pinMode(moteraPin, OUTPUT);
  pinMode(moterbPin, OUTPUT);
  pinMode(sevorPin, OUTPTU);
}

void loop() 
{
  if (powerPin == HIGH)
  analogWrite()
}
