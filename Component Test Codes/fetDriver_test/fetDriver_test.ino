// #define fetPin 3
// #define maxIntensity 255
const int fetPin = 1;
// const int maxIntensity = 255;
void setup() {
  // put your setup code here, to run once:
  pinMode(fetPin, OUTPUT);
}

void loop() {
  digitalWrite(fetPin, HIGH);
  delay(50);
  digitalWrite(fetPin, LOW);
  delay(25);
}
