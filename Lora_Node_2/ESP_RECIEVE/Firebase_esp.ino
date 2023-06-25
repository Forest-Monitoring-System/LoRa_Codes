#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <time.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// // Insert your network credentials
// #define WIFI_SSID "Redmi Note 9 Pro Max"
// #define WIFI_PASSWORD "12345678"

// #define WIFI_SSID "Home wifi"
// #define WIFI_PASSWORD "youwillneverknow"

#define WIFI_SSID "eyantra_drone"
#define WIFI_PASSWORD "eyantra@123"

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

// void Wifi_disconnected(WiFiEvent_t event) {
//   wifi_connected = false;
//   check_wifi();
//   Serial.print(event);
//   // Wifi.reconnect();
// }

// For getting time
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800;
const int   daylightOffset_sec = 3600;


void wifi_init() {
  // WiFi.onEvent(Wifi_disconnected); //, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.onEvent(WiFiDisconnectEvent, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.onEvent(WiFiConnectEvent, WiFiEvent_t::SYSTEM_EVENT_STA_CONNECTED);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  
  Serial.print("Connecting to Wi-Fi");
  // int i = 0;
  while (WiFi.status() != WL_CONNECTED){
    // if (i == 0) {
    //   display.clearDisplay();
    //   display.setCursor(0, 30);
    //   display.print("Connecting to Wi-Fi");
    //   display.display();
    //   Serial.println("hi");
    // }
    // i++;
    // if (i == 5) i=0;

    // display.print(".");
    // display.display();
    Serial.print(".");
    delay(300);

  }
}

void db_init(){
  // Serial.begin(115200);
  wifi_connected = true;

  if ((WiFi.status() == WL_CONNECTED) && !db_initialised) {

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    strcpy(ip_addr,WiFi.localIP().toString().c_str());
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
    
    db_initialised = true;
  }
}

bool wifi_stat() {
  if (WiFi.status() == WL_CONNECTED)
    return true;
  else 
    return false;
}

void send_msg(int smoke, int flame, int count, float temp, float humid, int bat){
  bool data_sent = false;
  if (db_initialised) {
    // while (!data_sent){
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

        // Writing the value of battery
        if (Firebase.RTDB.setInt(&fbdo, "node_1/battper", bat)){
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


        //Data logging
        struct tm timeinfo;
        if(!getLocalTime(&timeinfo)){
          Serial.println("Failed to obtain time");
        } else {
          // sprintf(day_time, "%A, %B %d %Y %H:%M:%S", &timeinfo);
          int year = timeinfo.tm_year + 1900;
          int month = timeinfo.tm_mon + 1;
          int day = timeinfo.tm_mday;
          int hour = timeinfo.tm_hour;
          int minute = timeinfo.tm_min;
          int second = timeinfo.tm_sec;

          String date_time= String(year) + "-" + String(month) + "-" + String(day) + " " + String(hour) + ":" + String(minute) + ":" + String(second);

          Serial.println(date_time);

          /////////////////////////////////
          if (Firebase.RTDB.setInt(&fbdo, "node_1_log/"+date_time+"/smoke_detected", smoke)){
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
          if (Firebase.RTDB.setInt(&fbdo, "node_1_log/"+date_time+"/flame_detected", flame)){
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
          if (Firebase.RTDB.setInt(&fbdo, "node_1_log/"+date_time+"/count", count)){
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
          if (Firebase.RTDB.setInt(&fbdo, "node_1_log/"+date_time+"/temperature", temp)){
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
          if (Firebase.RTDB.setInt(&fbdo, "node_1_log/"+date_time+"/humidity", humid)){
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

          // Writing the value of battery
          if (Firebase.RTDB.setInt(&fbdo, "node_1_log/"+date_time+"/battper", bat)){
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
          /////////////////////////////////
        }
      }
    // }
  }
}
