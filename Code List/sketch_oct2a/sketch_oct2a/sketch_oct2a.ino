const int pirPin = 7;
const int trigPin = 8;
const int echoPin = 9;
const int ledPin = 10;

boolean pirOn = false;
boolean pirOff = false;
long duration;
int distanceCm;

void setup() 
{
  Serial.begin(9600); 
  pinMode(pirPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  pirOn = digitalRead(pirPin) == HIGH;
  pirOff = digitalRead(pirPin) == LOW;

  if (pirOn)
  {
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(2); 
    digitalWrite(trigPin, LOW); 

    duration = pulseIn(echoPin, HIGH);
    distanceCm = duration * 0.034 / 2;
  
    Serial.print("거리(cm):");
    Serial.println(distanceCm);
    delay(2000);
  
    if (distanceCm <= 10)
    {
      digitalWrite(ledPin, HIGH);
      delay(2000);
      digitalWrite(ledPin, LOW);
      delay(2000);
    }
    else if (distanceCm > 10)
    {
      digitalWrite(ledPin, LOW);
    }
  }
  else if (pirOff)
  {
    digitalWrite(ledPin, LOW);
  }
  
}