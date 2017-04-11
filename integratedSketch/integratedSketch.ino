#include <SparkFunTSL2561.h>
#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>
#include <EEPROM.h>
#include <extEEPROM.h>
#include <avr/sleep.h>

#include "DHT.h"

#define disk1 0x50
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22

const int hygrometer = A0;

unsigned long msStart;
unsigned long msLast;

int duration = 24; //length to record in hours

uint16_t address = 0;
const uint32_t totalKBytes = 32;         //for read and write test functions
extEEPROM eep(kbits_256, 1, 64);         //device size, number of devices, page size

SFE_TSL2561 light;
boolean gain;
unsigned int ms;

struct Measurement {
  float temp;
  float humidity;
  float lux;
  float moisture;
  unsigned long time;
};

struct Sums {
  float temp;
  int temp_count;
  float humidity;
  int humidity_count;
  float lux;
  int lux_count;
  float moisture;
  int moisture_count;
};

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Program Started");
  //uint8_t *addressPtr = (uint8_t*)&address;
  //eep.read(0,addressPtr,2);//write to the address variable the first two bytes of eeprom, this will ensure data collection continues if power is interrupted
  dht.begin();
  msStart = millis();
  msLast = msStart;
  uint8_t eepStatus = eep.begin(twiClock400kHz);
  if (eepStatus) {
    Serial.print(F("extEEPROM.begin() failed, status = "));
    Serial.println(eepStatus);
    while (1);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  float temp;
  float humidity;
  double lux;//stores the lux value
  float moisture;
  /*Serial.print("Current time:   ");
  Serial.println(millis());
  Serial.print("Start:  ");
  Serial.println(msStart);
  Serial.print("Last:  ");
  Serial.println(msLast);
  Serial.print("Diff:  ");
  Serial.println(millis()-msLast);*/
  Sums sums{
    0,0,0,0,0,0,0,0,
  };
  while(millis()-msLast < ((duration*60*60)/1.6)) {
    if(Serial.available() > 0){
      sendData();
    }
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    Serial.println(t);
    int data0;//these strore individual values from the light sensor that are used by the light library for lux calculation.
    int data1;

    light.getLux(gain,ms,data0,data1,lux);

    moisture = readHygrometer();

    if (isnan(t) || isnan(h) || isnan(lux) || isnan(moisture)) {
      Serial.println("Error with a reading");
    }
    if(!isnan(t)) {
      sums.temp += t;
      sums.temp_count++;
    }
    if(!isnan(h)) {
      sums.humidity += h;
      sums.humidity_count++;
    }
    if(!isnan(lux)) {
      sums.lux += lux;
      sums.lux_count++;
    }
    if(!isnan(moisture)) {
      sums.moisture += moisture;
      sums.moisture_count++;
    }

//    Serial.print("Temperature: ");
//    Serial.println(t);
//    Serial.print("Humidity: ");
//    Serial.println(h);
//    Serial.print("Lux: ");
//    Serial.println(lux);
//    Serial.print("Soil moisture: ");
//    Serial.println(moisture);
    delay(5000);
  }
  msLast = millis();//records time at which the last reading was taken
  //unsigned long mseconds = msStart-msLast; //records the time from the start that the reading was taken
  Measurement reading = {
    sums.temp/sums.temp_count, sums.humidity/sums.humidity_count, sums.lux/sums.lux_count, sums.moisture/sums.moisture_count, millis()
  };

  /*Serial.print("Reading size: ");
  Serial.println(sizeof(reading));
  Serial.println("*************Into Memory**************");
  Serial.print(reading.temp);
  Serial.print(":::");
  Serial.print(reading.humidity);
  Serial.print(":::");
  Serial.print(reading.lux);
  Serial.print(":::");
  Serial.print(reading.moisture);
  Serial.print(":::");
  Serial.println(reading.time);*/

  uint8_t *ptr =  (uint8_t*)&reading;
  eep.write(address, ptr, sizeof(reading));

  Measurement stored;
  uint8_t *memPtr =  (uint8_t*)&stored;
  eep.read(address, memPtr, 20);
  /*Serial.println("********************From Memory*********************");
  Serial.print(stored.temp);
  Serial.print(":::");
  Serial.print(stored.humidity);
  Serial.print(":::");
  Serial.print(stored.lux);
  Serial.print(":::");
  Serial.print(stored.moisture);
  Serial.print(":::");
  Serial.println(stored.time);*/

  if(address <= 32747){
   address += sizeof(reading);
   //uint8_t *addressPtr = (uint8_t)&address;
   //eep.write(0,addressPtr,2);//the first 2 bytes of eeprom store the next available address, this allows you to distinguish old and new data in case of a power loss.
  }
  else{
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
  }
}

void sendData() {
  int send_address = 0;
  char request = Serial.read();
  Serial.println(request);
  if(request = 1) {
    while (send_address<32760){
      Measurement reading;
      uint8_t *ptr = (uint8_t*)&reading;
      uint8_t status = eep.read(send_address, ptr, 20);
      if (status){
        Serial.print("error reading EEPROM at ");
        Serial.println(send_address);
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
      Serial.println(send_address);
      send_address += sizeof(reading);
      delay(10);
    }
  }
}

int readHygrometer() {
  int value;
  value = analogRead(hygrometer);   //Read analog value
  value = constrain(value,400,1023);  //Keep the ranges!
  value = map(value,400,1023,10000,0);
  return value;
}

