//부저, OLED, 온습도
#include "DHT.h"
#include <U8glib.h>
#include <U8g2lib.h>
#define DHTPIN 4 //온습도센
#define DHTTYPE DHT11
// 모터 핀 정의
#define MotorA1 10  // 모터 A: 앞으로 이동
#define MotorA2 11  // 모터 A: 뒤로 이동
#define MotorB1 5   // 모터 B: 앞으로 이동
#define MotorB2 6   // 모터 B: 뒤로 이동

//조이스틱
int x;
int y;
int value = 0;
int up = 0;
int down = 0;
int right = 0;
int left = 0;
int reset = 0;
int count = 0;

#include <Servo.h>
// Servo myservo;  // 서보 객체 생성
int pos = 40; //각도를 조절할 변수 선언
const int angleChangeSpeed = 1;        // 각도 변경 속도
const long interval1 = 5000;            // 각 구간의 지속 시간 (밀리초)

// 초음파 센서 핀 정의
#define TrigPin 3   // 초음파 센서 Trig 핀
#define EchoPin 2   // 초음파 센서 Echo 핀
#define TrigPin2 9   // 초음파 센서 Trig 핀
#define EchoPin2 8   // 초음파 센서 Echo 핀
#define TrigPin3 13   // 초음파 센서 Trig 핀
#define EchoPin3 12   // 초음파 센서 Echo 핀
DHT dht(DHTPIN, DHTTYPE);

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

//==============================================================
//노래
int songNum = 0;
int currentSong = 0;
typedef enum {
  NOTE_C, NOTE_Cs, NOTE_D, NOTE_Eb, NOTE_E, NOTE_F, NOTE_Fs, NOTE_G, NOTE_Gs, 
  NOTE_A, NOTE_Bb, NOTE_B, NOTE_MAX
} note_t;

// 화음별 피에조 부저 진동수 정의 및 옥타브에 따른 값 변경 함수
double ledcWriteNote(note_t note, uint8_t octave){
  const uint16_t noteFrequencyBase[12] = {
  //  1      2      3      4      5      6     7       8     9       10    11      12
  //  C      C#     D      Eb     E      F     F#      G     G#      A     Bb      B
     4186,  4435,  4699,  4978,  5274,  5588,  5920,  6272,  6645,  7040,  7459,  7902
  };
  if(octave > 8 || note >= NOTE_MAX){
    return 0;
  }
  double noteFreq =  (double)noteFrequencyBase[note] / (double)(1 << (8-octave));
  return noteFreq;
}

// ---------------------------------------------------------------------- 동요: 햇볕은 쨍쨍
uint8_t Tempo = 120;
float tempoRatio = float(Tempo)/60.0;
uint8_t KeySign = 0;
uint8_t octaveTemp = 4;
float beatTime = 2/4;

const note_t Mnote[] PROGMEM= {
NOTE_C,NOTE_E,NOTE_G,NOTE_G,NOTE_G,NOTE_E,NOTE_F,NOTE_G,NOTE_E,NOTE_C,NOTE_C,
NOTE_C,NOTE_B,NOTE_C,NOTE_A,NOTE_G,NOTE_A,NOTE_G,NOTE_E,NOTE_C,NOTE_B,NOTE_C,NOTE_A,NOTE_G,NOTE_A,NOTE_G,NOTE_E,
NOTE_A,NOTE_A,NOTE_G,NOTE_G,NOTE_E,NOTE_E,NOTE_G,NOTE_G,NOTE_D,NOTE_C,NOTE_D,NOTE_E,NOTE_C,NOTE_C,
};

const uint8_t Moct[] PROGMEM= {
4,4,4,4,4,4,4,4,4,4,4,
5,4,5,4,4,4,4,4,5,4,5,4,4,4,4,4,
4,4,4,4,4,4,4,4,4,4,4,4,4,4,
0,
};

const uint8_t localKey[] PROGMEM= {
0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,
};

const uint8_t Mdur[] PROGMEM= {
16,
3,1,4,4,4,2,2,2,2,4,4,
2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
2,2,2,2,2,2,2,2,3,1,2,2,4,4,
};
// ----------------------------------------------------------------------

uint8_t noteNum = 0;

note_t keySignature(note_t note) {
  if (pgm_read_byte(&localKey[noteNum]) == 0) {
    if (KeySign == 1) { 
      if (note == NOTE_B) note = NOTE_Bb;
    } else if (KeySign == 2) { 
      if (note == NOTE_B) note = NOTE_Bb;
      else if (note == NOTE_E) note = NOTE_Eb;
    } else if (KeySign == 3) {
      if (note == NOTE_B) note = NOTE_Bb;
      else if (note == NOTE_E) note = NOTE_Eb;
      else if (note == NOTE_A) note = NOTE_Gs;
    } else if (KeySign == 4) { 
      if (note == NOTE_B) note = NOTE_Bb;
      else if (note == NOTE_E) note = NOTE_Eb;
      else if (note == NOTE_A) note = NOTE_Gs;
      else if (note == NOTE_D) note = NOTE_Cs;
    } else if (KeySign == 5) { 
      if (note == NOTE_B) note = NOTE_Bb;
      else if (note == NOTE_E) note = NOTE_Eb;
      else if (note == NOTE_A) note = NOTE_Gs;
      else if (note == NOTE_D) note = NOTE_Cs;
      else if (note == NOTE_G) note = NOTE_Fs;
    } else if (KeySign == 6) {
      if (note == NOTE_B) note = NOTE_Bb;
      else if (note == NOTE_E) note = NOTE_Eb;
      else if (note == NOTE_A) note = NOTE_Gs;
      else if (note == NOTE_D) note = NOTE_Cs;
      else if (note == NOTE_G) note = NOTE_Fs;
      else if (note == NOTE_C) { note = NOTE_B; octaveTemp -= 1; }
    } else if (KeySign == 7) { 
      if (note == NOTE_B) note = NOTE_Bb;
      else if (note == NOTE_E) note = NOTE_Eb;
      else if (note == NOTE_A) note = NOTE_Gs;
      else if (note == NOTE_D) note = NOTE_Cs;
      else if (note == NOTE_G) note = NOTE_Fs;
      else if (note == NOTE_C) { note = NOTE_B; octaveTemp -= 1; }
      else if (note == NOTE_F) note = NOTE_E;
    } else if (KeySign == 8) { 
      if (note == NOTE_F) note = NOTE_Fs;
    } else if (KeySign == 9) {
      if (note == NOTE_F) note = NOTE_Fs;
      else if (note == NOTE_C) note = NOTE_Cs;
    } else if (KeySign == 10) {
      if (note == NOTE_F) note = NOTE_Fs;
      else if (note == NOTE_C) note = NOTE_Cs;
      else if (note == NOTE_G) note = NOTE_Gs;
    } else if (KeySign == 11) { 
      if (note == NOTE_F) note = NOTE_Fs;
      else if (note == NOTE_C) note = NOTE_Cs;
      else if (note == NOTE_G) note = NOTE_Gs;
      else if (note == NOTE_D) note = NOTE_Eb;
    } else if (KeySign == 12) {
      if (note == NOTE_F) note = NOTE_Fs;
      else if (note == NOTE_C) note = NOTE_Cs;
      else if (note == NOTE_G) note = NOTE_Gs;
      else if (note == NOTE_D) note = NOTE_Eb;
      else if (note == NOTE_A) note = NOTE_Bb;
    } else if (KeySign == 13) {
      if (note == NOTE_F) note = NOTE_Fs;
      else if (note == NOTE_C) note = NOTE_Cs;
      else if (note == NOTE_G) note = NOTE_Gs;
      else if (note == NOTE_D) note = NOTE_Eb;
      else if (note == NOTE_A) note = NOTE_Bb;
      else if (note == NOTE_E) note = NOTE_F;
    } else if (KeySign == 14) { 
      if (note == NOTE_F) note = NOTE_Fs;
      else if (note == NOTE_C) note = NOTE_Cs;
      else if (note == NOTE_G) note = NOTE_Gs;
      else if (note == NOTE_D) note = NOTE_Eb;
      else if (note == NOTE_A) note = NOTE_Bb;
      else if (note == NOTE_E) note = NOTE_F;
      else if (note == NOTE_B) { note = NOTE_C; octaveTemp += 1; }
    }
  }
  else { // 1. natural
    if (pgm_read_byte(&localKey[noteNum]) == 2) { // 2. b
      if (note == NOTE_C) { note = NOTE_B; octaveTemp -= 1; }
      else if (note == NOTE_D) note = NOTE_Cs;
      else if (note == NOTE_E) note = NOTE_Eb;
      else if (note == NOTE_F) note = NOTE_E;
      else if (note == NOTE_G) note = NOTE_Fs;
      else if (note == NOTE_A) note = NOTE_Gs;
      else if (note == NOTE_B) note = NOTE_Bb;
    }
    else if (pgm_read_byte(&localKey[noteNum]) == 3) { // 2. #
      if (note == NOTE_C) note = NOTE_Cs;
      else if (note == NOTE_D) note = NOTE_Eb;
      else if (note == NOTE_E) note = NOTE_F;
      else if (note == NOTE_F) note = NOTE_Fs;
      else if (note == NOTE_G) note = NOTE_Gs;
      else if (note == NOTE_A) note = NOTE_Bb;
      else if (note == NOTE_B) { note = NOTE_C; octaveTemp += 1; }
    }
    else if (pgm_read_byte(&localKey[noteNum]) == 4) { // 4. bb
      if (note == NOTE_C) { note = NOTE_Bb; octaveTemp -= 1; }
      else if (note == NOTE_D) note = NOTE_C;
      else if (note == NOTE_E) note = NOTE_D;
      else if (note == NOTE_F) note = NOTE_Eb;
      else if (note == NOTE_G) note = NOTE_F;
      else if (note == NOTE_A) note = NOTE_G;
      else if (note == NOTE_B) note = NOTE_A;
    }
    else if (pgm_read_byte(&localKey[noteNum]) == 5) { // 5. ##
      if (note == NOTE_C) note = NOTE_D;
      else if (note == NOTE_D) note = NOTE_E;
      else if (note == NOTE_E) note = NOTE_Fs;
      else if (note == NOTE_F) note = NOTE_G;
      else if (note == NOTE_G) note = NOTE_A;
      else if (note == NOTE_A) note = NOTE_B;
      else if (note == NOTE_B) { note = NOTE_Cs; octaveTemp += 1; }
    }
  }
  return note;
}
// ----------------------------------------------------------------------

int beepPin = 7;

bool beepMelody = false;
unsigned long int beepTime = 0;

//===========================================================================
double length, length2, length3;
unsigned long previousMillis = 0;  // 이전에 모터 동작을 시작한 시간
const long interval = 2000;        // 모터 동작 시간 (2초)

//오른쪽이 좀 더 빠름.
void moveForward(int speed){
  analogWrite(MotorA1, speed);
  analogWrite(MotorA2, 0);
  analogWrite(MotorB1, speed+60);
  analogWrite(MotorB2, 0);

  if(length < 30 || length2 < 30 || length3 < 30){
    analogWrite(MotorA1, 0);
    analogWrite(MotorA2, 0);
    analogWrite(MotorB1, 0);
    analogWrite(MotorB2, 0);
    delay(500);

    analogWrite(MotorA1, speed);
    analogWrite(MotorA2, 0);
    analogWrite(MotorB1, 0);
    analogWrite(MotorB2, speed);
    delay(1000);
  }
}
//================================================
void playSun(){
  if (millis() - beepTime >= pgm_read_byte(&Mdur[noteNum])*(1000/(4*tempoRatio))) {
    beepTime = millis();
    noTone(beepPin);
    octaveTemp = pgm_read_byte(&Moct[noteNum]);
    note_t KeyNote = keySignature(pgm_read_word(&Mnote[noteNum]));
    tone(beepPin, ledcWriteNote(KeyNote, octaveTemp));  // ledcWriteNote(uint8_t channel, note_t note, uint8_t octaveTemp);
    noteNum++;
    if (noteNum == sizeof(Mdur)) { // 초기화
      noTone(beepPin);  
      noteNum = 0; 
      beepMelody = false;
    }
  }
}

//=================================================
void turnLeft(int speed) {
  analogWrite(MotorA1, speed);
  analogWrite(MotorA2, 0);
  analogWrite(MotorB1, (speed+60) - (speed / 2));
  analogWrite(MotorB2, 0);

  if(length < 30 || length2 < 30 || length3 < 30){
    analogWrite(MotorA1, 0);
    analogWrite(MotorA2, 0);
    analogWrite(MotorB1, 0);
    analogWrite(MotorB2, 0);
    delay(500);

    analogWrite(MotorA1, 0);
    analogWrite(MotorA2, speed);
    analogWrite(MotorB1, speed);
    analogWrite(MotorB2, 0);
    delay(1000);
  }
}

void turnRight(int speed) {
  analogWrite(MotorA1, speed - (speed / 2));
  analogWrite(MotorA2, 0);
  analogWrite(MotorB1, speed+60);
  analogWrite(MotorB2, 0);

  if(length < 30 || length2 < 30 || length3 < 30){
    analogWrite(MotorA1, 0);
    analogWrite(MotorA2, 0);
    analogWrite(MotorB1, 0);
    analogWrite(MotorB2, 0);
    delay(500);

    analogWrite(MotorA1, speed);
    analogWrite(MotorA2, 0);
    analogWrite(MotorB1, 0);
    analogWrite(MotorB2, speed);
    delay(1000);
  }
}

void stopMoving() {
  analogWrite(MotorA1, 0);
  analogWrite(MotorA2, 0);
  analogWrite(MotorB1, 0);
  analogWrite(MotorB2, 0);
}


void setup(){
  dht.begin();

  pinMode(MotorA1, OUTPUT);
  pinMode(MotorA2, OUTPUT);
  pinMode(MotorB1, OUTPUT);
  pinMode(MotorB2, OUTPUT);

  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);

  pinMode(TrigPin2, OUTPUT);
  pinMode(EchoPin2, INPUT);

  pinMode(TrigPin3, OUTPUT);
  pinMode(EchoPin3, INPUT);


  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();
  static int randomDirection = -1;
  x = analogRead(A0);
  y = analogRead(A1);
  
  // 초음파 1
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  double duration = pulseIn(EchoPin, HIGH);
  length = duration * 340 / 10000 / 2;

  // 초음파 2
  digitalWrite(TrigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin2, LOW);
  double duration2 = pulseIn(EchoPin2, HIGH);
  length2 = duration2 * 340 / 10000 / 2;

  // 초음파 3
  digitalWrite(TrigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin3, LOW);
  double duration3 = pulseIn(EchoPin3, HIGH);
  length3 = duration3 * 340 / 10000 / 2;


  if(x <= 600 && x >= 400&& y <= 600 && y >= 400){
    reset = 0;
  }

  if(x > 600 && reset == 0){ //오른쪽
    right += 1;
    reset = 1;
    
    if(right == 2){
      right = 0;
    }
  }
  else if(x < 400 && reset == 0){ //왼쪽
    left += 1;
    reset = 1;
    
    if(left == 2){
      left = 0;
    }
  }

  else if(y > 600 && reset == 0){ //모터 작동 (위)
    up += 1;
    reset = 1;
    
    if(up == 2){
      up = 1;
    }
  }
  else if(y < 400 && reset == 0){ //모터 끔(아래)
    up -= 1;
    reset = 1;
    if(up == -1){
      up = 0;
    }
  }

  if(up == 1 && right == 1 && left == 1){
    // Serial.println("moter on");
    Serial.write('1');
    digitalWrite(7, HIGH);
    playSun();
    if(currentMillis - previousMillis < interval){
      // 이전 동작 유지
      switch (randomDirection){
        case 0:
          moveForward(150);
          break;
        case 1:
          turnRight(150);
          break;
        case 2:
          turnLeft(150);
          break;
      }
    } else {
      // 새로운 무작위 방향으로 이동
      randomDirection = random(3);
      previousMillis = currentMillis;
    }
  }
  else if(up == 1 && right == 1 && left == 0){
    // Serial.write("moter off");
    digitalWrite(7, LOW);
    Serial.write('2');
    if(currentMillis - previousMillis < interval){
      // 이전 동작 유지
      switch (randomDirection){
        case 0:
          moveForward(150);
          break;
        case 1:
          turnRight(150);
          break;
        case 2:
          turnLeft(150);
          break;
      }
    } else {
      // 새로운 무작위 방향으로 이동
      randomDirection = random(3);
      previousMillis = currentMillis;
    }
  }
  else if(up == 1 && right == 0 && left == 1){
    Serial.write('3');
    digitalWrite(7, HIGH);
    playSun();
    if(currentMillis - previousMillis < interval){
      // 이전 동작 유지
      switch (randomDirection){
        case 0:
          moveForward(150);
          break;
        case 1:
          turnRight(150);
          break;
        case 2:
          turnLeft(150);
          break;
      }
    } else {
      // 새로운 무작위 방향으로 이동
      randomDirection = random(3);
      previousMillis = currentMillis;
    }
  }
  else if(up == 1 && right == 0 && left == 0){
    digitalWrite(7, LOW);
    Serial.write('4');
    if(currentMillis - previousMillis < interval){
      // 이전 동작 유지
      switch (randomDirection){
        case 0:
          moveForward(150);
          break;
        case 1:
          turnRight(150);
          break;
        case 2:
          turnLeft(150);
          break;
      }
    } else {
      // 새로운 무작위 방향으로 이동
      randomDirection = random(3);
      previousMillis = currentMillis;
    }
  }
  
  else if(up == 0 && right == 1 && left == 0){
    digitalWrite(7, LOW);
    Serial.write('5');
    stopMoving();
  }
  else if(up == 0 && right == 0 && left == 1){
    digitalWrite(7, HIGH);
    Serial.write('6');
    stopMoving();
    playSun();
  }
  else if(up == 0 && right == 1 && left == 1){
    digitalWrite(7, HIGH);
    Serial.write('7');
    stopMoving();
    playSun();
  }
  else if(up == 0 && right == 0 && left == 0){
    digitalWrite(7, LOW);
    Serial.write('8');
    stopMoving();
  }

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  u8g.firstPage();
  do{
    u8g.setFont(u8g_font_fub17);
    u8g.setPrintPos(0, 55);
    u8g.print(humidity,1);
    u8g.print("%");

    u8g.setFont(u8g_font_fub17);
    u8g.setPrintPos(70, 55);
    u8g.print(temperature,1);
    u8g.print("C");
  }while(u8g.nextPage());
  //==================================================
  
}