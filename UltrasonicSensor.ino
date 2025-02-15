const int trigPin = 9;
const int echoPin = 10;
long duration1,duration2;
int distance1,distance2;
float velocity;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration1 = pulseIn(echoPin, HIGH);
  long t1=millis();
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration2 = pulseIn(echoPin, HIGH);
  long t2=millis();
  distance1=duration1*0.017;
  distance2=duration2*0.017;
  velocity=100*(distance1-distance2)/(t1-t2);
  if (velocity<0) {
    velocity=-velocity;
  }
  Serial.print("Distance(in cm): ");
  Serial.print(distance2);
  Serial.print("\tVelocity(in cm/s): ");
  Serial.print(velocity);
  Serial.println();
  delay(500);
}
