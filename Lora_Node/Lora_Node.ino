// Lora Node
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>



#define i2c_Address 0x3c
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int counter = 0;

void setup() {
  Serial.begin(115200);
  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
 
  display.display();
  delay(2000);  
  while (!Serial);
  LoRa.begin(433E6);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  if (isSmokeDetected()) {
    LoRa.print("Smoke Detected");
    oled("Smoke Detected",1,10) ;        
  } 
  else {
    LoRa.print("Smoke Not Detected");
    oled("Smoke Not Detected",1,10) ;   
  }

  if (flameCount() >= 2) {
    LoRa.print(" || Flame Detected || ");
    oled("Flame Detected",1,20);
 
  }
  
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;

  delay(1000);
}
