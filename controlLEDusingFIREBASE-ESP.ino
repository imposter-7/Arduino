#include <ESP8266WiFi.h>                                               
#include <FirebaseArduino.h>

#define FIREBASE_HOST "homeautomation-6d999-default-rtdb.firebaseio.com"     
#define FIREBASE_AUTH "1rknUEZTjXVlUz6avz3mx9nnHlC9zxYu1PHjBiPy" 
#define WIFI_SSID "iPhone"                                          
#define WIFI_PASSWORD "hala1234" 

String fireStatus = "";                                                     // led status received from firebase
int led = 5; 


void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
  delay(1000);    
  pinMode(led, OUTPUT);                 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                               
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
//  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);                  // connect to firebase
//  Firebase.setString("LED_STATUS", "OFF"); 
}

void loop() {
  // put your main code here, to run repeatedly:
//fireStatus = Firebase.getString("LED_STATUS");                                      // get ld status input from firebase
//  if (fireStatus == "ON") 
//  {                                                          // compare the input of led status received from firebase
//    Serial.println("Led Turned ON");                                                        
//    digitalWrite(led, HIGH);                                                         // make external led ON
//  } 
//  else if (fireStatus == "OFF") 
//  {                                                  // compare the input of led status received from firebase
//    Serial.println("Led Turned OFF");
//    digitalWrite(led, LOW);                                                         // make external led OFF
//  }
//  else 
//  {
//    Serial.println("Command Error! Please send ON/OFF");
//  }
}
