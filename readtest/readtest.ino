#include <Wire.h>

#define disk1 0x50    //Address of 24LC256 eeprom chip

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  unsigned int address = 20;

  Serial.println(readEEPROM(disk1, address));
}

void loop() {
  // put your main code here, to run repeatedly:

}

byte readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
  Serial.println("beginning EEPROM read");
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
 
  Wire.requestFrom(deviceaddress,2);
 
  while (Wire.available()) {
    rdata = Wire.read();
  }
 
  return rdata;
}
