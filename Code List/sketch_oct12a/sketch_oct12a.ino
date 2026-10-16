#include <Servo.h>

const int button1Pin = 1;        // 버튼 1 입력 핀
const int button2Pin = 2;        // 버튼 2 입력 핀
const int motorPin1 = 5;         // DC 모터 제어 핀 1
const int motorPin2 = 6;         // DC 모터 제어 핀 2
const int servoPin = 9;          // 서보모터 제어 핀

int motorSpeed = 0;              // DC 모터 속도 (0: 정지, 100: 100 속도, 200: 200 속도)

Servo myservo;                    // 서보모터 객체 생성

bool motorRunning = false;
bool servoRunning = false;

void setup() 
{
  Serial.begin(9600);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  analogWrite(motorPin1, 0);   // DC 모터 초기에 정지 상태로 설정
  analogWrite(motorPin2, 0);
  myservo.attach(servoPin);       // 서보모터 핀 초기화
  myservo.write(90);              // 서보모터를 초기 각도로 설정 (90도)
}

void loop() 
{
  // 버튼 1이 눌렸고 DC 모터가 동작 중이 아닌 경우
  if (digitalRead(button1Pin) == LOW && !motorRunning) 
  {
    motorSpeed = 100;
    motorRunning = true;
  }

  // 버튼 1이 눌렸고 DC 모터가 100 속도로 동작 중인 경우
  else if (digitalRead(button1Pin) == LOW && motorSpeed == 100) 
  {
    motorSpeed = 230;
  }

  // 버튼 1이 눌렸고 DC 모터가 200 속도로 동작 중인 경우
  else if (digitalRead(button1Pin) == LOW && motorSpeed == 230) 
  {
    motorSpeed = 0; // 정지
    motorRunning = false;
  }

  // DC 모터 제어
  if (motorRunning) 
  {
    analogWrite(motorPin1,motorSpeed);
    analogWrite(motorPin2, 0);
  } 
  else if (motorSpeed == 100) 
  {
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, 0);
  }

  // 버튼 2가 눌렸을 때 서보모터 회전
  if (digitalRead(button2Pin) == LOW) 
  {
    if (!servoRunning) 
    {
      myservo.write(50); // 원하는 각도로 설정 (예: 50도)
      servoRunning = true;
    }
  }
  else 
  {
    servoRunning = false;
  }
}