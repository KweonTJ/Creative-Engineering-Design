int x;
int y;
int buzzer = 7;
int pause = 100;  // pause 변수 추가 및 초기화

void setup()
{
  Serial.begin(9600);
  pinMode(8, INPUT_PULLUP);
}

void loop()
{
  x = analogRead(A0);
  y = analogRead(A1);

  if (x == 0)
  {
    Serial.println("학교 종이 땡땡땡");
    tone(buzzer, 392, 250);
    delay(pause);
    tone(buzzer, 392, 250);
    delay(pause);
    tone(buzzer, 440, 250);
    delay(pause);
    tone(buzzer, 440, 250);
    delay(pause);
    tone(buzzer, 392, 250);
    delay(pause);
    tone(buzzer, 392, 250);
    delay(pause);
    tone(buzzer, 330, 550);
    delay(pause);
    tone(buzzer, 392, 250);
    delay(pause);
    tone(buzzer, 392, 250);
    delay(pause);
    tone(buzzer, 330, 250);
    delay(pause);
    tone(buzzer, 330, 250);
    delay(pause);
    tone(buzzer, 293, 750);
    delay(pause);
    noTone(buzzer);
    delay(pause);
    tone(buzzer, 392, 250);
    delay(pause);
    tone(buzzer, 392, 250);
    delay(pause);
    tone(buzzer, 440, 250);
    delay(pause);
    tone(buzzer, 440, 250);
    delay(pause);
    tone(buzzer, 392, 250);
    delay(pause);
    tone(buzzer, 392, 250);
    delay(pause);
    tone(buzzer, 329, 500);
    delay(pause);
    tone(buzzer, 392, 250);
    delay(pause);
    tone(buzzer, 329, 250);
    delay(pause);
    tone(buzzer, 293, 250);
    delay(pause);
    tone(buzzer, 329, 250);
    delay(pause);
    tone(buzzer, 261, 750);
    delay(pause);
  }
  if(x == 0)
  {
    
  }

  delay(100);
}
