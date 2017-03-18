#include <extEEPROM.h>

const uint32_t totalKBytes = 32;         //for read and write test functions
extEEPROM eep(kbits_256, 1, 64);         //device size, number of devices, page size

int address = 0;

struct Measurement {
  float temp;
  float humidity;
  float lux;
  float moisture;
  unsigned long time;
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  uint8_t eepStatus = eep.begin(twiClock400kHz);      //go fast!
  if (eepStatus) {
    Serial.print(F("extEEPROM.begin() failed, status = "));
    Serial.println(eepStatus);
    while (1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  while (address<32760){
    Measurement reading;
    uint8_t *ptr = (uint8_t*)&reading;
    uint8_t status = eep.read(address, ptr, 20);
    if (status){
      Serial.print("error reading EEPROM at ");
      Serial.println(address);
    }
    Serial.print(reading.temp);
    Serial.print(",");
    Serial.print(reading.humidity);
    Serial.print(",");
    Serial.print(reading.lux);
    Serial.print(",");
    Serial.print(reading.moisture);
    Serial.print(",");
    Serial.print(reading.time);
    Serial.print(",");
    Serial.println(address);
    address += sizeof(reading);
    delay(10);
  }
}

