#include <Servo.h>

const int button1Pin = 3;        // 버튼 1 입력 핀
const int button2Pin = 4;        // 버튼 2 입력 핀
const int motorPin1 = 5;         // DC 모터 제어 핀 1
const int motorPin2 = 6;         // DC 모터 제어 핀 2
const int servoPin = 9;          // 서보모터 제어 핀

int motorSpeed = 0;              // DC 모터 속도 (0: 정지, 100: 100 속도, 230: 230 속도)
int servoAngle = 90;             // 서보모터 초기 각도 (90도)
int servoTargetAngle = 50;      // 목표 서보모터 각도 (예: 50도)

Servo myservo;                    // 서보모터 객체 생성

bool motorRunning = false;       // DC 모터 동작 여부를 나타내는 플래그
bool servoRotating = false;      // 서보모터 회전 여부를 나타내는 플래그
unsigned long lastServoMoveTime = 0; // 마지막 서보모터 회전 시간
unsigned long servoMoveInterval = 500; // 서보모터 회전 간격 (500ms)
bool button1Pressed = false;     // 버튼 1 눌림 여부를 나타내는 플래그
unsigned long servoMoveStartTime = 0; // 서보모터 회전 시작 시간
unsigned int servoMoveDuration = 3000; // 서보모터 회전 지속 시간 (밀리초)

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
  myservo.write(servoAngle);      // 서보모터를 초기 각도로 설정 (90도)
}

void loop() {
  // 버튼 1이 눌렸을 때 DC 모터 제어
  if (digitalRead(button1Pin) == LOW) 
  {
    if (!button1Pressed) 
    {
      button1Pressed = true;
      if (!motorRunning) 
      {
        motorSpeed = 100;
        motorRunning = true;
      }
      else if (motorSpeed == 100) 
      {
        motorSpeed = 230;
      }
      else if (motorSpeed == 230) 
      {
        motorSpeed = 0; // 정지
        motorRunning = false;
      }
    }
  } 
  else 
  {
    button1Pressed = false;
  }

  // DC 모터 제어
  if (motorRunning) 
  {
    analogWrite(motorPin1, motorSpeed);
    analogWrite(motorPin2, 0);
  } 
  else 
  {
    analogWrite(motorPin1, 0);
    analogWrite(motorPin2, 0);
  }

  // 버튼 2가 눌렸을 때 서보모터 제어
  if (digitalRead(button2Pin) == LOW) 
  {
    if (!servoRotating) 
    {
      servoMoveStartTime = millis();
      servoRotating = true;
    }
  }

  // 서보모터 회전 및 유지
  if (servoRotating) 
  {
    unsigned long currentTime = millis();
    if (currentTime - servoMoveStartTime <= servoMoveDuration) 
    {
      // 서보모터를 목표 각도로 점진적으로 회전시킵니다.
      float progress = float(currentTime - servoMoveStartTime) / servoMoveDuration;
      int currentAngle = servoAngle + int(progress * (servoTargetAngle - servoAngle));
      myservo.write(currentAngle);
    } 
    else 
    {
      // 지정된 시간이 경과하면 회전을 멈춥니다.
      myservo.write(servoTargetAngle);
      servoRotating = false;
    }
  }
}