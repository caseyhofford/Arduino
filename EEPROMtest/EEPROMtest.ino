#include <EEPROM.h>

int value = 333;
int circle;

void setup() {
  // put your setup code here, to run once:
  EEPROM.put(0, value);
  EEPROM.get(0, circle);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(value);
  Serial.print("here: ");
  Serial.println(circle);
}



