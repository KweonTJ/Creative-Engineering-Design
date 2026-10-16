#include <Servo.h>

const int buttonPin = 3;         // 버튼 입력 핀
const int motorPin1 = 5;         // DC 모터 제어 핀 1
const int motorPin2 = 6;         // DC 모터 제어 핀 2
const int servoPin = 9;         // 서보모터 제어 핀

int motorSpeed = 0;              // DC 모터 속도 (0: 정지, 100: 100 속도, 230: 230 속도)

Servo myservo;                    // 서보모터 객체 생성

enum MotorState {
  STOPPED,
  SPEED_100,
  SPEED_230
};

MotorState motorState = STOPPED;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  digitalWrite(motorPin1, LOW);   // DC 모터 초기에 정지 상태로 설정
  digitalWrite(motorPin2, LOW);
  myservo.attach(servoPin);       // 서보모터 핀 초기화
  myservo.write(90);              // 서보모터를 초기 각도로 설정 (90도)
}

void loop() {
  if (digitalRead(buttonPin) == LOW) 
  {
    delay(50); // 디바운싱을 위한 짧은 지연
    if (digitalRead(buttonPin) == LOW) 
    {
      // 버튼이 눌렸을 때
      switch (motorState) 
      {
        case STOPPED:
          motorSpeed = 100;
          motorState = SPEED_100;
          break;
        case SPEED_100:
          motorSpeed = 230;
          motorState = SPEED_230;
          break;
        case SPEED_230:
          motorSpeed = 0;
          motorState = STOPPED;
          break;
      }
    }
  }

  // DC 모터 제어
  if (motorSpeed == 0) 
  {
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, 0);
  } 
  else if (motorSpeed == 100) 
  {
    analogWrite(motorPin1, 100);
    analogWrite(motorPin2, 0);
  } 
  else if (motorSpeed == 230) 
  {
    analogWrite(motorPin1, 230);
    analogWrite(motorPin2, 0);
  }
}