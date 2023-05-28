#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ArduinoJson.h>



#define i2c_Address 0x3c
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//define the pins used by the transceiver module
#define css 5
#define rst 14
#define dio0 2

void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  StaticJsonDocument<200> doc;  
  display.begin(i2c_Address, true); // Address 0x3C default
  
  display.display();
  //display.clearDisplay();   
  delay(2000);    
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(css, rst, dio0);
  while (!Serial);
  LoRa.begin(433E6);  
  //while (!LoRa.begin(433E6)) {
    //Serial.println(".");
    //delay(500);
  //}
   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  //Serial.println("LoRa Initializing OK!");

  db_init();
}

void loop() {
  // try to parse packet
  // send_msg(false);  
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    Serial.print("Packet Size");
    Serial.println(packetSize);

    // read packet
    while (LoRa.available()) {
      String LoRaData = LoRa.readString();
      Serial.println(LoRaData); 
            
      deserializeJson(doc, json);
      int smoke=  doc["msmoke"];
          
      int flame =doc["mflame"];

      int counter=doc["ccount"];
      // send_msg(false);      

    display.clearDisplay(); 
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(15,0);
    display.print("FOREST MONITOR");
    display.setCursor(12,10);
    display.print("_________________");    
    // delay(100);  
    display.display();
  
    
      oled(LoRaData,0,30);      
    }

    // print RSSI of packet
    //Serial.print("' with RSSI ");
    //Serial.println(LoRa.packetRssi());
  }
}