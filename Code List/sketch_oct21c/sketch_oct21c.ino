#define RightMotorA_forward 6
#define RightMotorA_backward 5 
#define LeftMotorB_forward 11
#define LeftMotorB_backward 10

// IR(Infrared) 적외선 핀 설정
#define IR_right A1
#define IR_left A0

// LED 핀 설정
#define LED_right 2
#define LED_left 3

// 제어 입력
int speed = 165; // 모터 출력

// 로직 설정
int flag_right = 0;  // 1일 때 오른쪽으로 회전
int flag_left = 0;   // 1일 때 왼쪽으로 회전

// P 제어를 위한 변수 설정
double error_r = 0.0;
double kp = 0.0005;
float con_input_r = kp * error_r;
double forward_fitting = 0;

// IR 센서 값 저장을 위한 구조체 선언
struct IRsensor_msg {
  double value_l = 0;
  double value_r = 0;
  double time = -1;
};

// IR 변수 선언
IRsensor_msg Cur_ir_msgs;
IRsensor_msg Prev_ir_msgs;

void setup() {
  // 모터 핀 설정
  pinMode(RightMotorA_forward, OUTPUT);
  pinMode(RightMotorA_backward, OUTPUT);
  pinMode(LeftMotorB_forward, OUTPUT);
  pinMode(LeftMotorB_backward, OUTPUT);

  // 디버깅을 위한 LED 핀 설정
  pinMode(LED_right, OUTPUT);
  pinMode(LED_left, OUTPUT);

  // IR 핀 설정
  pinMode(IR_right, INPUT);
  pinMode(IR_left, INPUT);
  Serial.begin(9600);

  // 초기 LED 상태 설정
  digitalWrite(LED_left, LOW);
  digitalWrite(LED_right, LOW);
}

void error_compute() {
  // case 1:
  error_r = Cur_ir_msgs.value_r - Prev_ir_msgs.value_r;
  con_input_r = error_r * kp;

  // case 2:
  double x_des = 80;
  error_r = Cur_ir_msgs.value_r - x_des;
  if (error_r > 0)
    con_input_r = error_r * kp;
}

void sub_ir_msgs() {
  // IR 센서 값 읽기
  double Sub_msg_l = analogRead(IR_left);
  double Sub_msg_r = analogRead(IR_right); 

  // 초기값 설정
  if (Prev_ir_msgs.time == -1) {
    Prev_ir_msgs.time = millis();
    Prev_ir_msgs.value_l = Sub_msg_l;
    Prev_ir_msgs.value_r = Sub_msg_r;

    Cur_ir_msgs.time = millis();
    Cur_ir_msgs.value_l = Sub_msg_l;
    Cur_ir_msgs.value_r = Sub_msg_r;
    Serial.println("init done");
  }

  // 현재 값 업데이트 후, 과거 값 현재로 스왑
  if (Cur_ir_msgs.time < millis()) {
    Prev_ir_msgs.time = Cur_ir_msgs.time;
    Prev_ir_msgs.value_l = Cur_ir_msgs.value_l;
    Prev_ir_msgs.value_r = Cur_ir_msgs.value_r;

    Cur_ir_msgs.time = millis();
    Cur_ir_msgs.value_l = Sub_msg_l;
    Cur_ir_msgs.value_r = Sub_msg_r;
    // Serial.println("update done");
  } else {
    Serial.println("IR msgs not update");
  }
}

void motor_control() {
  if (Prev_ir_msgs.time - Cur_ir_msgs.time < 0) {
    // 정상으로 판단(경계선 사이 값)
    if (Cur_ir_msgs.value_r > 80 && Cur_ir_msgs.value_r < 400) {
      flag_right = 0;
      flag_left = 0;
    }
    // 검은선 판단(오른쪽으로 벗어남)
    else if (Cur_ir_msgs.value_r > 400) {
      flag_right = 0;
      flag_left = 1; // 왼쪽으로 이동
    }
    // 흰선 판단(왼쪽으로 벗어남)
    else if (Cur_ir_msgs.value_r < 80) {
      flag_right = 1; // 오른쪽으로 이동
      flag_left = 0;
    }

    Serial.print("sensor_right : ");
    Serial.println(Cur_ir_msgs.value_r);

    // 직선 주행
    if (flag_left == 0 && flag_right == 0) {
      analogWrite(RightMotorA_forward, speed);
      analogWrite(RightMotorA_backward, 0);
      analogWrite(LeftMotorB_forward, speed + forward_fitting);
      analogWrite(LeftMotorB_backward, 0);
      digitalWrite(LED_left, HIGH);
      digitalWrite(LED_right, HIGH);
    }
    // 좌측 주행
    else if (flag_left == 0 && flag_right == 1) {
      analogWrite(RightMotorA_forward, speed + con_input_r);
      analogWrite(RightMotorA_backward, 0);
      analogWrite(LeftMotorB_forward, 60 + forward_fitting);
      analogWrite(LeftMotorB_backward, 0);
      digitalWrite(LED_left, HIGH);
      digitalWrite(LED_right, LOW); 
    }
    // 우측 주행
    else if (flag_right == 0 && flag_left == 1) {
      analogWrite(RightMotorA_forward, 60);
      analogWrite(RightMotorA_backward, 0);
      analogWrite(LeftMotorB_forward, speed + con_input_r + forward_fitting);
      analogWrite(LeftMotorB_backward, 0);
      digitalWrite(LED_right, HIGH);
      digitalWrite(LED_left, LOW);
    }
  } 
}

// 모터 직선 주행 용
void forward() {
  analogWrite(RightMotorA_forward, speed);
  analogWrite(RightMotorA_backward, 0);
  analogWrite(LeftMotorB_forward, speed + forward_fitting);
  analogWrite(LeftMotorB_backward, 0);
}

void loop() {
  sub_ir_msgs();
  error_compute();
  motor_control();
  // forward(); // 주석 해제하려면 이 부분을 주석 해제하세요
  delay(50);
}


