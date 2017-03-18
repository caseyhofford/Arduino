#include <Wire.h>
#define disk1 0x50

int readingframe = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  int data = readEEPROM(disk1,readingframe);
  Serial.print("Lux: ");
  Serial.println(data);
  readingframe += sizeof(data);
}

byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
 
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress,1);
 
  if (Wire.available()) {
    rdata = Wire.read();
  }
  else  {
    Serial.print("error reading from eeprom");
    exit(0);
  }
  
 
  return rdata;
}


