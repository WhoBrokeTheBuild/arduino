
int pin = 0;

void setup() {
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(A0, INPUT);
}

void loop() {
  float val = analogRead(A0);
  if (val < 2) {
    pin = 0;
  } 
  else if (val < 8) {
    pin = 1;
  }
  else {
    pin = 2;
  }
  
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  
  digitalWrite(pin + 5, HIGH);
  delay(20);
}
