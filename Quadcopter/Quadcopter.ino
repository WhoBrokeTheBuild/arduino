int pwmPin = 13;
float duty = 0.50;

void setup()
{
  pinMode(pwmPin, OUTPUT);
}

void loop()
{
  digitalWrite(pwmPin, HIGH);
  delayMicroseconds(1000 * duty); // Approximately 10% duty cycle @ 1KHz
  digitalWrite(pwmPin, LOW);
  delayMicroseconds(1000 - (1000 * duty));
}
