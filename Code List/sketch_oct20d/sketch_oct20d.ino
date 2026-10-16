const int RightMotorA = 6;
const int RightMotorB = 5;
const int LeftMotorA = 11;
const int LeftMotorB = 10;

const int IR_right = A0;  // 오른쪽 IR 센서
const int IR_left = A1;   // 왼쪽 IR 센서

const int LED_blue = 8;
const int LED_red = 7;

const int IR_THRESHOLD = 500;  // 조정이 필요할 수 있음

int motorSpeed = 180;  // 모터 속도 변수

void setup() {
  pinMode(RightMotorA, OUTPUT);
  pinMode(RightMotorB, OUTPUT);
  pinMode(LeftMotorA, OUTPUT);
  pinMode(LeftMotorB, OUTPUT);
  pinMode(IR_right, INPUT);
  pinMode(IR_left, INPUT);
  pinMode(LED_blue, OUTPUT);
  pinMode(LED_red, OUTPUT);

  analogWrite(RightMotorA, 0);  // 모터 초기 속도 설정
  analogWrite(LeftMotorA, 0);

  digitalWrite(LED_blue, HIGH);  // 파란색 LED를 초기에 켭니다.
  digitalWrite(LED_red, LOW);    // 빨간색 LED를 초기에 끕니다.
}

void loop() {
  int rightIR = analogRead(IR_right);
  int leftIR = analogRead(IR_left);

  if (rightIR > IR_THRESHOLD && leftIR > IR_THRESHOLD) {
    // 양쪽 센서가 흰색 라인을 감지할 때만 직진
    analogWrite(RightMotorA, motorSpeed);
    analogWrite(RightMotorB, 0);
    analogWrite(LeftMotorA, motorSpeed);
    analogWrite(LeftMotorB, 0);
  } else {
    // 양쪽 센서 중 하나라도 흰색 라인을 감지하지 않으면 정지
    analogWrite(RightMotorA, 0);
    analogWrite(RightMotorB, 0);
    analogWrite(LeftMotorA, 0);
    analogWrite(LeftMotorB, 0);
    
    // 레일을 벗어났을 때 보정 및 방향 전환 코드
    if (rightIR < IR_THRESHOLD && leftIR < IR_THRESHOLD) {
      analogWrite(RightMotorA, motorSpeed);  // 오른쪽 모터를 전진 방향으로
      analogWrite(RightMotorB, 0);
      analogWrite(LeftMotorA, motorSpeed);   // 왼쪽 모터를 전진 방향으로
      analogWrite(LeftMotorB, 0);
      delay(50);  // 50ms 동안 보정 후 직진
    } else {
      analogWrite(RightMotorA, 0);   // 오른쪽 모터를 정지
      analogWrite(RightMotorB, 0);
      analogWrite(LeftMotorA, motorSpeed);    // 왼쪽 모터를 전진 방향으로
      analogWrite(LeftMotorB, 0);
      delay(50);  // 50ms 동안 방향 전환
    }
  }
}

