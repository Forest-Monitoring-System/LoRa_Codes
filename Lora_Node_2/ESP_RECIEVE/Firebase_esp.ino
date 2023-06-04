#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// // Insert your network credentials
// #define WIFI_SSID "Redmi Note 9 Pro Max"
// #define WIFI_PASSWORD "12345678"

#define WIFI_SSID "Home wifi"
#define WIFI_PASSWORD "youwillneverknow"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDWjNUhJ8LaPVPsytvWwu0Q6nxXwJ4wIac"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://forest-monitoring-system-default-rtdb.asia-southeast1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
bool db_initialised = false;

void db_init(){
  // Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }

  // if ((WiFi.status() == WL_CONNECTED) && !db_initialised)
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void send_msg(int smoke, int flame, int count, float temp, float humid){
  bool data_sent = false;
  while (!data_sent){
    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
      sendDataPrevMillis = millis();
      // Writing the value of smoke
      if (Firebase.RTDB.setInt(&fbdo, "node_1/smoke_detected", smoke)){
        Serial.println("PASSED");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
        data_sent = true;
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        data_sent = false;
      }
      
      // Writing the value of flame
      if (Firebase.RTDB.setInt(&fbdo, "node_1/flame_detected", flame)){
        Serial.println("PASSED");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
        data_sent = true;
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        data_sent = false;
      }

      // Writing the value of count
      if (Firebase.RTDB.setInt(&fbdo, "node_1/count", count)){
        Serial.println("PASSED");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
        data_sent = true;
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        data_sent = false;
      }

      // Writing the value of temperature
      if (Firebase.RTDB.setInt(&fbdo, "node_1/temperature", temp)){
        Serial.println("PASSED");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
        data_sent = true;
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        data_sent = false;
      }

      // Writing the value of humidity
      if (Firebase.RTDB.setInt(&fbdo, "node_1/humidity", humid)){
        Serial.println("PASSED");
        // Serial.println("PATH: " + fbdo.dataPath());
        // Serial.println("TYPE: " + fbdo.dataType());
        data_sent = true;
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
        data_sent = false;
      }
    }
  }
  
}
