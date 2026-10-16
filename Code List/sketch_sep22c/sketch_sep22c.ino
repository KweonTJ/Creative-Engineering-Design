const int RightA = 6;
const int RightB = 5;
const int LeftA = 11;
const int LeftB = 10;

const int IR_right = A0;
const int IR_left = A1;

const int LED_red = 7;
const int LED_blue = 8;

int speed = 160;
int moving = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RightA, OUTPUT);
  pinMode(RightB, OUTPUT);
  pinMode(LeftA, OUTPUT);
  pinMode(LeftB, OUTPUT);
  pinMode(LED_red, OUTPUT);
  pinMode(LED_blue, OUTPUT);
  pinMode(IR_right, INPUT);
  pinMode(IR_left, INPUT);
}

void motor_control() {
  int right_line = analogRead(IR_right);
  int left_line = analogRead(IR_left);

  if (right_line > 500 && left_line > 500) {
    // 양쪽 IR 센서가 흰색 라인을 감지하면 전진
    analogWrite(RightA, speed);
    analogWrite(RightB, 0);
    analogWrite(LeftA, speed);
    analogWrite(LeftB, 0);
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_blue, HIGH);
    moving = 1;
    Serial.println("Moving forward");
  } else if (right_line > 500) {
    // 오른쪽 IR 센서만 흰색 라인을 감지하면 오른쪽으로 방향 전환
    analogWrite(RightA, 0);
    analogWrite(RightB, speed);
    analogWrite(LeftA, speed);
    analogWrite(LeftB, 0);
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_blue, HIGH);
    moving = 1;
    Serial.println("Turning right");
  } else if (left_line > 500) {
    // 왼쪽 IR 센서만 흰색 라인을 감지하면 왼쪽으로 방향 전환
    analogWrite(RightA, speed);
    analogWrite(RightB, 0);
    analogWrite(LeftA, 0);
    analogWrite(LeftB, speed);
    digitalWrite(LED_red, LOW);
    digitalWrite(LED_blue, HIGH);
    moving = 1;
    Serial.println("Turning left");
  } else {
    // 어떤 IR 센서도 흰색 라인을 감지하지 않으면 정지
    analogWrite(RightA, 0);
    analogWrite(RightB, 0);
    analogWrite(LeftA, 0);
    analogWrite(LeftB, 0);
    digitalWrite(LED_red, HIGH);
    digitalWrite(LED_blue, LOW);
    moving = 0;
    Serial.println("Stopped");
  }
}

void loop() {
  motor_control();
  delay(50);
}

