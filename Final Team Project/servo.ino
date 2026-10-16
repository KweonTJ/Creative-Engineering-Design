#include <Servo.h>
#include <U8glib.h> //U8glib 라이브러리 추가 
Servo myServo1;
Servo myServo2;

int scount1 = 1;
int scount2

int pos1 = 0;
int pos2;
unsigned long prevMillis1 = 0;
unsigned long prevMillis2 = 0;
const int interval1 = 10;  // 서보1의 움직임 간격 (밀리초)
const int interval2 = 50;  // 서보2의 움직임 간격 (밀리초)

char data;
unsigned long previousMillis = 0;
int up;
int sound;
int servop;

// 0.96인치 128x64 OLED
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);  
//SSD1306 128X64 I2C 규격 선택 

void move_servo()
{
  unsigned long currentMillis = millis();

  if (scount1 == 1) // scount1이 1인 경우에만 서보 제어
  {
    if (currentMillis - prevMillis1 >= interval1)
    {
      // 서보1의 위치 증가
      prevMillis1 = currentMillis;
      pos1++;
      myServo1.write(pos1);

      if (pos1 >= 130)
      {
        pos1 = 20;  // 다음 반복을 위해 위치 초기화
        scount1 = 0;     // scount1을 0으로 설정하여 20초 동안 서보 동작을 멈춤
      }
    }
  }
  else
  {
    // scount1이 0인 경우 20초 동안 서보를 20으로 유지
    if (currentMillis - prevMillis1 >= 20000)  // 20초 설정
    {

      // 서보2수정 발사하는 서보모터는 0-150까지
      // prevMillis2 = currentMillis;
      // pos2++;
      // myServo2.write(pos2);

      // if (pos2 >= 150)
      // {
      //   pos2 = 0;  // 다음 반복을 위해 위치 초기화
      //   scount2 = 0;     // scount1을 0으로 설정하여 20초 동안 서보 동작을 멈춤
      // }


      scount1 = 1;  // 20초가 경과하면 다시 scount1을 1로 설정하여 서보 동작을 시작
    }
  }

  // if (currentMillis - prevMillis2 >= interval2)
  // {
  //   // 서보2의 위치 증가
  //   prevMillis2 = currentMillis;
  //   pos2++;

  //   myServo2.write(pos2);

  //   if (pos2 >= 90)
  //   {
  //     pos2 = 0;  // 다음 반복을 위해 위치 초기화
  //   }
  // }
}

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  myServo1.attach(9);
  // myServo2.attach(10);
  Serial.begin(9600);
}

void loop() {
  // unsigned long currentMillis = millis();
  if(Serial.available())
  {
    data = Serial.read();
  }
  
  if(data == '1'){
    up = 1;
    sound = 1;
    servop = 1;
  }
  else if(data == '2'){
    up = 1;
    sound = 0;
    servop = 1;
  }
  else if(data == '3'){
    up = 1;
    sound = 1;
    servop = 0;
  }
  else if(data == '4'){
    up = 1;
    sound = 0;
    servop = 0;
  }
  else if(data == '5'){
    up = 0;
    sound = 0;
    servop = 1;
  }
  else if(data == '6'){
    up = 0;
    sound = 1;
    servop = 0;
  }
  else if(data == '7'){
    up = 0;
    sound = 1;
    servop = 1;
  }
  else if(data == '8'){
    up = 0;
    sound = 0;
    servop = 0;
  }
  if(data == '1' || data == '2' ||data == '5' ||data == '7'){
    move_servo();
  }
  else{
    //서보모터 멈춤
    myServo1.write(20);
    //myServo2.write(20);
  }
  
  u8g.firstPage(); //picture loop의 시작
  do {
    u8g.setFont(u8g_font_fub17); // 폰트지정
    u8g.setPrintPos(0, 10); //n데이터 출력 커서 설정
    u8g.print("po ser sou"); //데이터 출력
    u8g.setPrintPos(0, 60); //n데이터 출력 커서 설정
    u8g.print(up);
    u8g.print(servop);
    u8g.print(sound);
  } while(u8g.nextPage()); //picture loop의 끝

  // 여기에 다른 코드 추가
}
