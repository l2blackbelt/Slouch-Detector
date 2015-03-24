// For ATTiny85 only
// Vcc is set at 3.3V! NOT 5 V
const int s0 = 3; // #Define doesn't work, so use const instead to avoid porblems
const int s1 = 4;
const int muxOut = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(muxOut, OUTPUT);

}

void loop() {
  // Cycle through A0, A1, A2, on MUX
  digitalWrite(muxOut, HIGH);
  
  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  delay(100);
  
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);
  delay(100);
  
  digitalWrite(s0, LOW);
  digitalWrite(s1, HIGH);
  delay(100);
  
}
