#define SHOT 13
#define FOCUS 12

void setup() {
  pinMode(SHOT, OUTPUT);
  pinMode(FOCUS, OUTPUT);

  digitalWrite(FOCUS, LOW);
  digitalWrite(SHOT, LOW);

}

void loop() {
 digitalWrite(FOCUS, LOW);
 delay(2000);
 digitalWrite(FOCUS, HIGH);
 delay(2000);

}
