# include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11 
float temp;
float humid;

DHT dht(DHTPIN,DHTTYPE);
// void setup() {
//   Serial.begin(9600);
//   dht.begin();
//   delay(1000);
  
// }

// void loop() {
//   humid=dht.readHumidity();
//   temp=dht.readTemperature();
//   Serial.print( "Humidity =  ");
//   Serial.println(humid);
//   Serial.print("Temperetaure");  
//   Serial.println(temp);  
//   delay(1000);
  
// }
