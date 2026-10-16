const int lefta = 6; // 왼쪽 모터
const int leftb = 5;
const int righta = 11; // 오른쪽 모터
const int rightb = 10;

const int irleft = A0; // IR 센서
const int irright = A1;

const int redled = 7; // 빨간색 LED
const int blueled = 8; // 파란색 LED

int Lspeed = 180; // 왼쪽 속도
int Rspeed = 180; // 오른쪽 속도

const int sensorThreshold = 420; // 센서 임계값
const int sensorDifference = 370; // 센서값 차이

const int stop_distance = 10; // 정지 거리
const int slow_distance = 20; // 속도 감소 거리

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(lefta, OUTPUT);
  pinMode(leftb, OUTPUT);
  pinMode(righta, OUTPUT);
  pinMode(rightb, OUTPUT);
  pinMode(redled, OUTPUT);
  pinMode(blueled, OUTPUT);
  pinMode(irright, INPUT);
  pinMode(irleft, INPUT);
  Lspeed = 180;
  Rspeed = 195;
}

// 초음파 센서
float measureDistance() 
{
  digitalWrite(3, LOW);
  delayMicroseconds(5);
  digitalWrite(3, HIGH);
  delayMicroseconds(5);
  digitalWrite(3, LOW);

  float duration = pulseIn(2, HIGH);
  float distance = duration * 0.034 / 2;

  return distance;
}

void run_go()
{
  Lspeed = 180;
  Rspeed = 195;
} // 전진

void run_left()
{
  Lspeed = 70;
  Rspeed = 170;
} // 좌회전

void run_right()
{
  Lspeed = 180;
  Rspeed = 50;
} // 우회전

void stop()
{
  Lspeed = 0;
  Rspeed = 0;
} // 정지

void slow()
{
  Lspeed -= 90;
  Rspeed -= 90;
} // 속도 감소

void tr()
{
  int valIRright = analogRead(irright);
  int valIRleft = analogRead(irleft);
  if (valIRright > sensorThreshold && valIRleft > sensorThreshold)
  {
    Lspeed = 180;
    Rspeed = 195;
  }
  else if (valIRright - valIRleft > sensorDifference)
  {
    run_left();
  }
  else if (valIRleft - valIRright > sensorDifference)
  {
    run_right();
  }
  else
  {
    run_go();
  }
}

void loop()
{
  float distance = measureDistance(); // 초음파 센서 측정 거라값 받아오기

  if (distance <= stop_distance)
  {
    stop();
    digitalWrite(redled, HIGH);
    digitalWrite(blueled, LOW);
  }
  else if (distance > stop_distance && distance <= slow_distance)
  {
    slow();
    digitalWrite(redled, LOW);
    digitalWrite(blueled, HIGH);
  }
  else
  {
    tr();
    digitalWrite(redled, LOW);
    digitalWrite(blueled, HIGH);
  }

  analogWrite(lefta, Lspeed);
  analogWrite(leftb, 0);
  analogWrite(righta, Rspeed);
  analogWrite(rightb, 0);
  Serial.print("Left: ");
  Serial.print(analogRead(irleft));
  Serial.print("Right: ");
  Serial.println(analogRead(irright));
  delay(10);
}
