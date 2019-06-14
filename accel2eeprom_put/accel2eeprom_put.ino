#include<Wire.h>
#include <EEPROM.h>

const int MPU=0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int addr = 0;

// variables for time stamp.
unsigned long timeInit;
unsigned long timeCurr;

// custom data structure for eeprom 'put'.
struct AccelData {
  unsigned long timestamp;
  int16_t acX;
  int16_t acY;
  int16_t acZ;
};


void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);

  // wait for serial port to connect. needed for native USB port only.
  while(!Serial) {
    ;
  }

  // record start time.
  timeInit = millis();
    
  
  // variable to store in EEPROM.
  float f = 123.456f; 
  // location we want the data to be put
  int eeAddress = 0;

  // commit a single float to memory, as a test.
  EEPROM.put(eeAddress, f);
  Serial.println("Written float data type!");

  // try out eeprom 'put' with custom data structures.
  AccelData oneInstance = {
    12345,
    1,
    2,
    3
  };

  // move address to the next byte after float 'f'.
  eeAddress += sizeof(float);

  EEPROM.put(eeAddress, oneInstance);
  Serial.print("Written custom data type!");
  
}
void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
  
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = "); Serial.println(AcZ); 
  
  Serial.print("Gyroscope: ");
  Serial.print("X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
  Serial.print(" | Z = "); Serial.println(GyZ);
  Serial.println(" ");
  delay(1000);
}
