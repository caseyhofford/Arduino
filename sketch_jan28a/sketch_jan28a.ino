#include <DHT.h>
#include <DHT_U.h>
#include <EEPROM.h>

#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define fan 4

int eeAddress = 0;

struct ReadingObject {
  float temp;
  float humidity;
};

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(fan, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  float htot = 0;
  float ttot = 0;
  int i = 0;
  int samples = 1;
  while (i < samples) {
    //Serial.println("starting");
    delay(2000);
    float h = dht.readHumidity();
    // Read temperature as Celsius
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      i++;
    }
    ttot += t;
    htot += h;
  }
  float hav = htot/samples;
  float tav = ttot/samples;
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)

  // Check if any reads failed and exit early (to try again).

  ReadingObject reading = {
    tav,
    hav
  };

  EEPROM.put(eeAddress, reading);
  eeAddress += sizeof(reading);

  Serial.print("Humidity: ");
  Serial.print(hav);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(tav);
  Serial.println(" *C ");

  ReadingObject retrieve;
  boolean cont = true;
  int readframe = 0;
  int counter = 0;
  //while(readframe < 1016){
  //  counter++;
  //  EEPROM.get(readframe, retrieve);
  //  Serial.print(counter);
  //  Serial.print(": humidity: ");
    //Serial.print("First Reading:: t: ");
    //Serial.print(retrieve.temp);
    //Serial.print(" h: ");
  //  Serial.print(retrieve.humidity);
    //Serial.println(sizeof(retrieve));
  //  if (isnan(retrieve.temp))
  //  {
  //    cont = false;
  //  }
  //  readframe += sizeof(retrieve);
  //  Serial.print("frame: ");
  //  Serial.println(readframe);
  //}
  EEPROM.get(0, retrieve);
  Serial.print(retrieve.temp);
  Serial.print(" : ");
  Serial.print(retrieve.humidity);
  EEPROM.get(1024, retrieve);
  Serial.print(retrieve.temp);
  Serial.print(" : ");
  Serial.println(retrieve.humidity);
}

