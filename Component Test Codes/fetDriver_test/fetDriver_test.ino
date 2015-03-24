#define fetPin 3
#define maxIntensity 255
void setup() {
  // put your setup code here, to run once:
  pinMode(fetPin, OUTPUT);
}

void loop() {
  int intensity = 0;
  // put your main code here, to run repeatedly:
  for(intensity; intensity <= maxIntensity; intensity++){
    analogWrite(fetPin, intensity);
    delay(75); 
  }
}
