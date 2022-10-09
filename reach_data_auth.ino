//#include <FirebaseESP8266.h>
#include <Arduino.h>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "HaniHome-2.4GHz"
#define WIFI_PASSWORD "hani@2016"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCdrQzBlbljlRRrOX4oL5lU8-dxcKpzgR0"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "lol@gmail.com"
#define USER_PASSWORD "halafarraj 1"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}


int val=0; 
void setup(){
  Serial.begin(115200);
  
  // Initialize WiFi
  initWiFi();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);
  Serial.println("Connected to firebase");
  Firebase.reconnectWiFi(true);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }

  if(Firebase.ready())
  {
    uid = auth.token.uid.c_str();
    Serial.print("User UID: ");
    Serial.print(uid);

    if(Firebase.get(fbdo,"/alias")){
      Serial.print(&fbdo.jsonObject());
    }
    
//    if (Firebase.RTDB.setInt(&fbdo, "test/int", 5)){
//      Serial.println("PASSED");
//      Serial.println("PATH: " + fbdo.dataPath());
//      Serial.println("TYPE: " + fbdo.dataType());
//    }
//    else {
//      Serial.println("FAILED");
//      Serial.println("REASON: " + fbdo.errorReason());
//    }
//if (Firebase.RTDB.getFloat(&fbdo, "/alias")) {
//      if (fbdo.dataType() == "String") {
//        floatValue = fbdo.floatData();
//        Serial.println(floatValue);
//      }
//    }
//    else {
//      Serial.println(fbdo.errorReason());
//    }
//  }
  // Print user UID
  

//  String path="/";
//  FirebaseObject object = Firebase.get(path);



}

void loop(){

}
