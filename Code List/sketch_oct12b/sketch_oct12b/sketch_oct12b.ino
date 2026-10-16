const int motorDirectionPin = 5;  // DC 모터 방향 제어 핀
const int motorSpeedPin = 6;     // DC 모터 속력 제어 핀
const int button1Pin = 2;        // 버튼 1 핀
const int button2Pin = 3;        // 버튼 2 핀
const int servoPin = 9;          // 서보 모터 제어 핀

int dcMotorSpeed = 0; // DC 모터 속력
int dcMotorDirection = 1; // DC 모터 방향 (1: 정방향, -1: 역방향)
int servoState = 0; // 서보 모터 상태 (0: 정지, 1: 회전)

#include <Servo.h>
Servo servo; // Servo 라이브러리를 사용하여 서보 모터 제어

void setup() {
  pinMode(motorDirectionPin, OUTPUT);
  pinMode(motorSpeedPin, OUTPUT);
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);

  servo.attach(servoPin);
  digitalWrite(motorDirectionPin, dcMotorDirection == 1 ? HIGH : LOW);
  analogWrite(motorSpeedPin, dcMotorSpeed);
}

void loop() {
  // 버튼 1을 누르면 DC 모터 속력 변경
  if (digitalRead(button1Pin) == LOW) {
    dcMotorSpeed += 100;
    if (dcMotorSpeed > 230) {
      dcMotorSpeed = 0;
    }
    analogWrite(motorSpeedPin, dcMotorSpeed);
    delay(200); // 디바운싱을 위한 딜레이
  }

  // 버튼 2를 누르면 서보 모터 제어
  if (digitalRead(button2Pin) == LOW) {
    if (servoState == 0) {
      servo.write(90); // 90도 회전
      servoState = 1;
    } else {
      servo.write(0); // 0도 (정지)
      servoState = 0;
    }
    delay(200); // 디바운싱을 위한 딜레이
  }
}