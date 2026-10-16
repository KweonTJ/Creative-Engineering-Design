#define MOTOR_A_1A 5
#define MOTOR_A_1B 6

void setup() {
  pinMode(MOTOR_A_1A, OUTPUT);
  pinMode(MOTOR_A_1B, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // 모토 저속 제어
  analogWrite(MOTOR_A_1A, 100);
  analogWrite(MOTOR_A_1B, 0);
  delay(3000);

  //모터 정지
  analogWrite(MOTOR_A_1A, 0);
  analogWrite(MOTOR_A_1B, 0);
  delay(3000);

  //모터 고속 제어
  analogWrite(MOTOR_A_1A, 255);
  analogWrite(MOTOR_A_1B, 0);
  delay(3000);

  //모터 정지
  analogWrite(MOTOR_A_1A, 0);
  analogWrite(MOTOR_A_1B, 0);
  delay(3000);
}
