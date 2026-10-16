const int lefta = 6; // 왼쪽 모터
const int leftb = 5;
const int righta = 11; // 오른쪽 모터
const int rightb = 10;

const int irleft = A0; //IR 센서
const int irright = A1;

const int redled = 7; // 빨간색 LED
const int blueled = 8; // 파란색 LED

int Lspeed = 0; // 왼쪽 속
int Rspeed = 0; // 오른쪽 속도

const int sensorThreshold = 430; // 센서 임계값
const int sensorDifference = 200; // 센서값 차이

const int max_distance = 10; // 최대 거리
const int slow_distance = 20; // 느린 거리

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
} // setup 리스트(모터, 센서)

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
} // 초음파 센서 

void run_go()
{
  Lspeed = 185;
  Rspeed = 185;
} // 전진
void run_left()
{
  Lspeed = 50;
  Rspeed = 172;
} // 좌회전
void run_right()
{
  Lspeed = 172;
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
} // 슬로우

void tr()
{
  int valIRRIGHT = analogRead(irright);
  int valIRRIGHT = analogRead(irleft);
  if (valIRright > sensorThreshold && valIRleft > sensorThreshold)
  {
    Lspeed = 140;
    Rspeed = 140;
  }
  else if (valIRRIGHT - valIRLEFT > sensorDifference)
  {
    run_left();
  }
  else if (valIRLEFT - valIRRIGHT > sensorDifference)
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
  float distance = measureDistance();

  if (distance <= max_distance)
  {
    stop();
    digitalWrite(redled, HIGH);
    digitalWrite(blueled, LOW);
  }
  else if (distance > max_distance && distance <= slow_distnace)
  {
    slow();
    digitalWrite(redled, LOW);
    digitalWrite(blueled, HIGH);
  }
  else
  {
    tr();
    digitlaWrite(redled, LOW);
    digitlaWrite(blueled, HIGH);
  }

  analogWrite(lefta, Lspeed);
  analogWrite(leftb, 0);
  analogWrite(righta, /Rspeed);
  anslogWrite(rightb, 0);
  Serial.print("LEFT: ");
  Serial.print(analogRead(IRLEFT));
  Serial.print("RIGHT: ");
  Serial.println(analogRead(IRRIGHT));
  delay(10);
}

