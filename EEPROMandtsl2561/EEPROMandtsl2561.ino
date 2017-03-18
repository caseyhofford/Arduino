#include <SparkFunTSL2561.h>
#include <Wire.h>
#define disk1 0x50

SFE_TSL2561 light;

boolean gain;
unsigned int ms;

unsigned int location = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  light.begin();
  light.setTiming(0,2,ms); //sets the read timing for the light sensor
  Serial.println("Powering on");
  light.setPowerUp();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int data0, data1;
  
  if (light.getData(data0,data1))
  {
    // getData() returned true, communication was successful
    
    Serial.print("data0: ");
    Serial.print(data0);
    Serial.print(" data1: ");
    Serial.print(data1);

    double lux;    // Resulting lux value
    boolean good;  // True if neither sensor is saturated
    
    // Perform lux calculation:
    good = light.getLux(gain,ms,data0,data1,lux);
    Serial.print(" lux: ");
    Serial.print(lux);
    if (good) Serial.println(" (good)"); else Serial.println(" (BAD)");
    writeEEPROM(disk1,location,lux);
    location += sizeof(lux);
    delay(1);
  }
  else
  {
    Serial.println("error reading from sensor");
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
  }
  
 
  return rdata;
}
