#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

const char *ssid =  "HaniHome-2.4GHz";      //WiFi SSID to which you want NodeMCU to connect
const char *pass =  "hani@2016";     //Password of your wifi network 

WiFiClient client;
 
                          
#define FIREBASE_HOST "homeautomation-6d999-default-rtdb.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "1rknUEZTjXVlUz6avz3mx9nnHlC9zxYu1PHjBiPy" //Your Firebase Database Secret goes here
             
                   
 
  

// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

int val=0; 

void setup() {
  // put your setup code here, to run once:
       Serial.begin(115200);
       delay(10);
               
       Serial.println("Connecting to ");
       Serial.println(ssid); 
 
       WiFi.begin(ssid, pass); 
       while (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
          }
      Serial.println("");
      Serial.println("WiFi connected to: ");
      Serial.println(ssid); 
      Serial.print("IP Address is : ");
      Serial.println(WiFi.localIP());
  
                                   // Select the same baud rate if you want to see the datas on Serial Monitor
  pinMode(D1,OUTPUT);
  Serial.println("Serial communication started\n\n");  
           
                                     //try to connect with wifi

  
  Serial.println();

                                              //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase
  Serial.println("Connected to firebase");
  Firebase.reconnectWiFi(true);
//  delay(1000);
}



void loop() {
  if (Firebase.getInt(firebaseData, "LED_STATUS")) {                           // On successful Read operation, function returns 1  

    if (firebaseData.dataType() == "int") {                            // print read data if it is integer

      val = firebaseData.intData();
      Serial.println(val);
      Serial.println("\n Change value at firebase console to see changes here."); 
//      delay(1000);
      if((int)val==1){
        Serial.println("LED is ON");
        digitalWrite(D1,HIGH);
//        delay(1000);
      }
      else{
        Serial.println("LED is OFF");
        digitalWrite(D1,LOW);
//        delay(1000);
      }
    }

//    delay(1000);
 }
}
