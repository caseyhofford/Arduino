#include <Wire.h>

#define disk1 0x50 //i2c address of EEPROM

void setup() {
  Wire.begin();
  unsigned int address = 0;
  int numbers[10] = {100,101,102,103,104,105,106,107,108,109};
  for(int i=0;i<10;i++)
  {
    writeEEPROM(disk1, address, numbers[i]);
    address += sizeof(numbers[i]);
  }
}

void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data ) 
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();
 
  delay(5);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
