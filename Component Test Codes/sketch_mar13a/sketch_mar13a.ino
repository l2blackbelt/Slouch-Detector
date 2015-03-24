int blinkPin = 0;
int pwmPin = 1;
void setup() {
  // put your setup code here, to run once:
  pinMode(blinkPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  for (int i=0; i<=255; i+=5)
{
  analogWrite(pwmPin, i);
  delay(5);
}
*/
   
  digitalWrite(blinkPin, HIGH);
  delay(200);
  digitalWrite(blinkPin, LOW);
  delay(200);
  
  /*
  digitalWrite(pwmPin, HIGH);
  delay(10);
  digitalWrite(pwmPin, LOW);
  delay(20);
  */
}
