// Test code for MMAX8452Q on ATTiny85
// Uses modified TinyWire library
// TinyWire library is set at 8 Mhz clock
// ATTiny85 is set at 8 MHz clock for softwareSerial

#include <TinyWireM.h>

// Define terms for the accelerometer
#define MMA8452_ADDRESS 0x1D // Accelerometer I2C address

//Define a few of the registers that we will be accessing on the MMA8452
#define OUT_X_MSB 0x01
#define XYZ_DATA_CFG  0x0E
#define WHO_AM_I   0x0D
#define CTRL_REG1  0x2A

#define GSCALE 2 // Sets full-scale range to +/-2, 4, or 8g. Used to calc real g values.

// No serial monitor on ATTiny85, so we will be using 3 LED pins instead for testing
#define LED1 4 // ATTiny Pin 3
#define LED2 1 // ATTiny Pin 6
#define LED3 3 // ATTiny Pin 2
#define ledThreshVal 0.7

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  
  TinyWireM.begin();
  initAccel();  
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int accelCount[3]; // Stores the 12-bit signed value
  readAccelData(accelCount);
  
  // Translate acceleration value into actual g's
  float accelG[3];
  for(int i = 0; i < 3; i++){
    accelG[i] = (float) accelCount[i] / ((1 << 12)/(2*GSCALE)); // get actual g value, this depends on scale being set
  }
  ledThresh(accelG[0], LED1);
  ledThresh(accelG[1], LED2);
  ledThresh(accelG[2], LED3);
  
  delay(10);
}
void ledThresh(float accelG, int pin){
  if(abs(accelG >= ledThreshVal)){
    digitalWrite(pin, HIGH);
  }
  else{
    digitalWrite(pin, LOW);
  }
  delay(10);
}

void readAccelData(int *destination){
  byte rawData[6]; // X, Y, Z accel register data stored here
  
  readRegisters(OUT_X_MSB, 6, rawData); // Read the six raw data registers into data array
  
  // Loop to calculate 12-bit ADC and g value for each axis
  for(int i = 0; i < 3; i++){
    int gCount = (rawData[i*2] << 8) | rawData[(i * 2) + 1]; //Combine the two 8 bit registers into one 12-bit number
    gCount >>= 4; //The registers are left align, here we right align the 12-bit integer
    
    // If the number is negative, we have to make it so manually (no 12-bit data type)
    if (rawData[i*2] > 0x7F){  
      gCount = ~gCount + 1;
      gCount *= -1;  // Transform into negative 2's complement #
    }

    destination[i] = gCount; //Record this gCount into the 3 int array
  }
}

void initAccel(){
   byte address = readRegister(WHO_AM_I); // Probe the accel address
   if(address == CTRL_REG1){ // Device is running and acquiring data
     working();
   }
   else{
     while(true){
       error();
     } 
   }
   
   // TODO: Need to cycle all three accelerometers and initialize them (set to standby and then active)
   // For this code we are looking at whether the accelerometer can communicate with ATTiny
   accelStandby(); // Sets the MMA8452 to standby mode. It must be in standby to change most register settings

   // Set up the full scale range to 2, 4, or 8g.
   byte range = GSCALE;
   if(range > 8){ // overflow check
     range = 8;
   }
   // range >>= 2 is the same as range = range >> 2;
   range >>= 2; // Neat trick, see page 22. 00 = 2G, 01 = 4A, 10 = 8G
   writeRegister(XYZ_DATA_CFG, range);
   
   //The default data rate is 800Hz and we don't modify it in this code
   
   accelActive();
}

void working(){
   // Blink all LEDs three times
   for(int counter = 0; counter < 3; counter++){
     digitalWrite(LED1, HIGH);
     digitalWrite(LED2, HIGH);
     digitalWrite(LED3, HIGH);
     delay(500);
     digitalWrite(LED1, LOW);
     digitalWrite(LED2, LOW);
     digitalWrite(LED3, LOW);
     delay(500);
   }
}
void error(){
  // Strobing LED
  for(int counter = 0; counter < 3; counter++){
    digitalWrite(LED1, HIGH);
    delay(500);
    digitalWrite(LED2, HIGH);
    delay(500);
    digitalWrite(LED3, HIGH);
    delay(500);
    
    digitalWrite(LED1, LOW);
    delay(500);
    digitalWrite(LED2, LOW);
    delay(500);
    digitalWrite(LED3, LOW);
    delay(500);    
  } 
}
// Sets the MMA8452 to standby mode. It must be in standby to change most register settings
void accelStandby(){
  byte address = readRegister(CTRL_REG1);
  writeRegister(CTRL_REG1, address & ~(0x01)); //Clear the active bit to go into standby
}

void accelActive(){
  byte address = readRegister(CTRL_REG1);
  writeRegister(CTRL_REG1, address | 0x01); //Set the active bit to begin detection
}

void readRegisters(byte addr2Read, int bytes2Read, byte * dest){
  TinyWireM.beginTransmission(MMA8452_ADDRESS);
  TinyWireM.send(addr2Read);
  TinyWireM.endTransmission();

  TinyWireM.requestFrom(MMA8452_ADDRESS, bytes2Read); //Ask for bytes, once done, bus is released by default  
  
  while(TinyWireM.available() < bytes2Read); //Hang out until we get the # of bytes we expect
  
  for(int x = 0; x < bytes2Read; x++){
    dest[x] = TinyWireM.receive();
  } 
}

byte readRegister(byte addr2Read){
  // byte accelData;
  
  TinyWireM.beginTransmission(MMA8452_ADDRESS);
  TinyWireM.send(addr2Read);
  TinyWireM.endTransmission(); // endTransmission doesn't take any arguement
  
  TinyWireM.requestFrom(MMA8452_ADDRESS, 1);
 
  while(!TinyWireM.available());
  // accelData = TinyWireM.receive();

  return TinyWireM.receive();
}

// Writes a single byte (dataToWrite) into addressToWrite
void writeRegister(byte addr2Write, byte data2Write){
  TinyWireM.beginTransmission(MMA8452_ADDRESS);
  TinyWireM.send(addr2Write);
  TinyWireM.send(data2Write);
  TinyWireM.endTransmission();
}
