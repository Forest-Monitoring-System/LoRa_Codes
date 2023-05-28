//Smoke Detection Code.....
#define SMOKE_THRESHOLD 120
#define SMOKE_PIN A0

bool isSmokeDetected() {
  bool smokeDetected = false;
  int smokeVal = analogRead(SMOKE_PIN);
  Serial.println(smokeVal);
  if (smokeVal > SMOKE_THRESHOLD) {
    smokeDetected = true;
  }
  return smokeDetected;
}
////////////////////////////////////////

//Flame Sensor Code.....
#define FLAME_THRESHOLD 900
int flameCount() {
  int count = 0;
  if (analogRead(A1) > FLAME_THRESHOLD) count++;
  if (analogRead(A2) > FLAME_THRESHOLD) count++;
  if (analogRead(A3) > FLAME_THRESHOLD) count++;
  if (analogRead(A4) > FLAME_THRESHOLD) count++;
  if (analogRead(A5) > FLAME_THRESHOLD) count++;
  return count;
}
//////////////////////////////////////////////

//DHT11 Sensor Code.....
#include "DHT.h"
#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void dht_init() {
  dht.begin();
  delay(100);
}

float get_humid() {
  if (dht.readHumidity() == NULL)
    return 0;
  else 
    return dht.readHumidity();
}

float get_temp() {
  // Serial.println(typeof(dht.readTemperature()));
  if (dht.readTemperature() == NULL)
    return 0;
  else 
    return dht.readTemperature();
}
//////////////////////////////////////////////
