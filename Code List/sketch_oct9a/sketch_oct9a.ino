#include <Servo.h>
Servo myservo;

const int turnPin = 3;    // 회전
const int powerPin = 4;   // 바람 세기
const int motoraPin = 5;  // DC 모터 (A)
const int motorbPin = 6;  // DC 모터 (B)
const int sevorPin = 9;   // 서보모터

int motorSpeed = 0;
int turnState = LOW;
int lastturnState = LOW;
int powerState = LOW;
int lastpowerState = LOW;

bool motorControl = true;

void setup() 
{
  Serial.begin(9600);
  pinMode(turnPin, INPUT);
  pinMode(powerPin, INPUT);
  pinMode(motoraPin, OUTPUT);
  pinMode(motorbPin, OUTPUT);
  digitalWrite(motoraPin, LOW);
  digitalWrite(motorbPin, LOW);
  myservo.attach(sevorPin);
  myservo.attach(90);
}

void loop()
{
  turnState = digitalRead(turnPin);
  powerState = digitalRead(powerPin);

  if (turnState == HIGH)
  {
    motorControl = false;
  }
  else
  {
    motorControl = true;
  }

  if (motorControl)
  {
    if(powerState == HIGH && lastpowerState == LOW)
    {
      if(motorSpeed == 0)
      {
        motorSpeed = 100;
      }
      else if (motorSpeed == 100)
      {
        motorSpeed = 200;
      }
      else if (motorSpeed == 200)
      {
        motorSpeed =0;
      }

      if (motorSpeed == 0)
      {
        digitalWrite(motoraPin, LOW);
        digitalWrite(motorbPin, LOW);
      }
      else if (motorSpeed == 100)
      {
        digitalWrite(motoraPin, HIGH);
        digitalWrite(motorbPin, LOW);
      }
      else if (motorSpeed == 200)
      {
        digitalWrite(motoraPin, HIGH);
        digitalWrite(motoraPin, HIGH);
      }
    }
  }

  if (turnState == HIGH)
  {
    myservo..write(0);
    delay(100);
    myservo.write(90);
  }

  lastturnState = turnState;
  lastpowerState = powerState;
}






