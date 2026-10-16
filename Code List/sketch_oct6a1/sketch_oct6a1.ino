#include <Servo.h> // Servo 라이브러리 추가

Servo myservo; //  Servo 클래스로 servo 객체 생성
int pos; //각도를 조절할 변수 선언
void setup()
{
  pinMode(9,OUTPUT);
  myservo.attach(9); // 모터의 신호선을 9번핀에 연결
  Serial.begin(9600);
}

void loop() 
{
  //연속 실행 ver
  for(pos = 5; pos<=65;pos++)
  {
    myservo.write(pos);
    delay(10);
  }
  for (pos = 65; pos >= 5; pos--)
  {
    myservo.write(pos);
    delay(10);
  }
}
