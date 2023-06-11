// Lora Node
#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SH110X.h>
// #include <GyverOLED.h>
// #include <oled.h> 



// #define i2c_Address 0x3c
// #define SCREEN_WIDTH 128 // OLED display width, in pixels
// #define SCREEN_HEIGHT 64 // OLED display height, in pixels
// #define OLED_RESET -1   //   QT-PY / XIAO
// #define LOGO16_GLCD_HEIGHT 16
// #define LOGO16_GLCD_WIDTH  16

// Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// GyverOLED<SSH1106_128x64> display;
// OLED display(A4,A5,NO_RESET_PIN,OLED::W_128,OLED::H_64,OLED::CTRL_SH1106);   

int counter = 0;
StaticJsonDocument<200> doc;  

void setup() {
  Serial.begin(115200);
  delay(250); // wait for the OLED to power up
  Serial.println("Hi 1");
  pinMode(A5, INPUT);
  // display.begin(i2c_Address, true); // Address 0x3C default
  // display.init();
  // display.begin();

  Serial.println("Hi 2");

  // display.display();
  // display.clear();
  // display.update();
  delay(2000);  

  Serial.println("Hi 3");
  // while (!Serial);
  
  LoRa.begin(433E6);
  dht_init();
  

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

float batteryAvg[] = {3.7,3.7,3.7,3.7,3.7};
void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  Serial.print("Battery");
  batteryAvg[counter%5] = (float)analogRead(A5)*5/1023;
  float battVolt = (batteryAvg[0]+batteryAvg[1]+batteryAvg[2]+batteryAvg[3]+batteryAvg[4])/5;
  int battPercent = (battVolt-3.2)*100;
  Serial.println(battPercent);

  // send packet
  LoRa.beginPacket();
  // display.clear();
  int smoke =0; 
  int flcount=0 ;
  if (isSmokeDetected()) {
     smoke=1; 
    //LoRa.print("Smoke Detected");
    // oled("Smoke Detected",1,10) ;  
    // display.drawString(0,0,"Smoke Detected");       
  } 
  else {
    smoke =0;
    //LoRa.print("Smoke Not Detected");
    // oled("Smoke Not Detected",1,10) ; 
    // display.drawString(0,0,"Smoke not Detected");      
  }

  if (flameCount() >= 2) {
    flcount=1;    
    //LoRa.print("Flame Detected");
    // oled("Flame Detected",1,20);
    // display.drawString(0,10,"flame Detected");    
 
  } else {
    flcount=0;    
    // display.drawString(0,10,"flame not Detected"); 
  }

  doc["smoke"]   = smoke;
  doc["flame"]   = flcount;
  doc["count"]   = counter;
  doc["temp"]    = get_temp();
  doc["humid"]   = get_humid();
  doc["battper"] = battPercent;

  char serialized_data[100];
  serializeJson(doc, serialized_data); 


  Serial.println(serialized_data);
  LoRa.print(serialized_data);
  LoRa.endPacket();
  
  // display.clear();
  // display.home();   
  
    // печатай что угодно: числа, строки, float, как Serial!
  // display.display();
  // delay(2000);

  counter++;

  delay(1000);
}
