const int trigPin = 3;
const int echoPin = 4;
const int moterbPin = 6;
const int moteraPin = 5;

float distanceCm = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(moteraPin, OUTPUT);
  pinMode(moterbPin, OUTPUT);
}

void loop()
{
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, LOW); 
   double duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.034 / 2;
  
  Serial.print("거리(cm):");
  Serial.println(distanceCm);
  delay(500);

  analogWrite(moteraPin, 155);
  analogWrite(moterbPin, 0);

  if (distanceCm <=5)
  {
    analogWrite(moteraPin, 0);
    analogWrite(moterbPin, 0);
    delay(500);
  }
}