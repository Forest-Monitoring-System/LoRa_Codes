#include <SPI.h>
#include <LoRa.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ArduinoJson.h>

#include <WiFi.h>

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

StaticJsonDocument<200> doc; 
int smoke=0, flame=0, counter=0, bat=0;  
float temp=0.0, humid=0.0; 
bool wifi_connected = false;
char ip_addr[20];

void WiFiDisconnectEvent(WiFiEvent_t event){
  Serial.printf("[WiFi-event] event: %d\n", event);
  wifi_connected = false;
  check_wifi();
  Serial.print(event);
  update_oled();
}

void WiFiConnectEvent (WiFiEvent_t event) {
  wifi_connected = true;
  check_wifi();

  // strcpy(ip_addr, WiFi.localIP().toString().c_str());
  
  update_oled();
}

void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  display.begin(i2c_Address, true); // Address 0x3C default

  display_welcome();
  
  wifi_init();
  
  db_init();
  // while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(css, rst, dio0);
  // while (!Serial);
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

  update_oled();
}

void loop() {
  check_wifi();
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
            
      deserializeJson(doc, LoRaData);
      smoke   = doc["smoke"];          
      flame   = doc["flame"];
      counter = doc["count"];  
      temp  = doc["temp"];  
      humid = doc["humid"];
      bat = doc["battper"];

      send_msg(smoke, flame, counter, temp, humid, bat);

      update_oled();

      // oled(LoRaData,0,30);      
    }

    // print RSSI of packet
    //Serial.print("' with RSSI ");
    //Serial.println(LoRa.packetRssi());
  }
}