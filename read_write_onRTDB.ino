#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char *ssid =  "HaniHome-2.4GHz";      //WiFi SSID to which you want NodeMCU to connect
const char *pass =  "hani@2016";     //Password of your wifi network 

WiFiClient client;
FirebaseJson json;
FirebaseJson json2;
FirebaseJson lights;
                          
#define FIREBASE_HOST "homeautomation-6d999-default-rtdb.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "1rknUEZTjXVlUz6avz3mx9nnHlC9zxYu1PHjBiPy" //Your Firebase Database Secret goes here
             
    
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

 
  

// Declare the Firebase Data object in the global scope
FirebaseData firebaseData;

int val=0; 
String currentTime;
void setup() {
  // put your setup code here, to run once:
       Serial.begin(115200);
       delay(10);
       pinMode(LED_BUILTIN, OUTPUT);
               
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
       timeClient.begin();
       timeClient.setTimeOffset(10800);
  
                                   // Select the same baud rate if you want to see the datas on Serial Monitor
pinMode(D0,OUTPUT);
  Serial.println("Serial communication started\n\n");  
           
                                     //try to connect with wifi

  
  Serial.println();

                                              //print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase
  Serial.println("Connected to firebase");
  Firebase.reconnectWiFi(true);
//  delay(1000);



}

String get_time()
{
  timeClient.update();
  String timee="\"";
  int currentHour = timeClient.getHours();
  
  if(currentHour > 12)
  {
   currentHour-=12;
   
  }
  if(currentHour == 0)
  {
    currentHour = 12;
  }
//  Serial.println(currentHour);  
  timee+=String (currentHour);
  timee+=":";

  int currentMinute = timeClient.getMinutes();
  if(currentMinute<10){
    timee+="0";
  }
//  Serial.print("Minutes: ");
//  Serial.println(currentMinute);
  timee+=currentMinute;

  return timee;
}

String set_AM_PM(){
  timeClient.update();
  String am_pm="";  
  int currentHour = timeClient.getHours();
  if(currentHour>=12)
  {
    am_pm=" PM";
  }
  if(currentHour == 0)
  {
    am_pm=" AM";
  }
  else
    am_pm=" AM";

   return am_pm+"\"";
}

void loop() {

//-------------------set a value to RTDB-------------------------------
//Firebase.RTDB.setString(&firebaseData,"/s2/room2/light1","26265");
//---------------------------------------------------------------------

//----NTP----------
currentTime = get_time();
Serial.println(currentTime);

String timmme="\"";

timmme=get_time()+set_AM_PM();

Serial.println(timmme);

//-------------------get a value from RTDB-------------------------------
  if (Firebase.getInt(firebaseData, "/VMTnWxaqxrQwVEPeViX53pWjRBH3/led")) {                          

    if (firebaseData.dataType() == "int") {                           

      int val = firebaseData.intData();
       Serial.print("val = ");
       Serial.println(val);
        
//      val = firebaseData.intData();
//      Serial.println(val);
//      if(val == 1){
//        digitalWrite(D0,HIGH);
//        
//        }
//        else{
//          digitalWrite(D0,LOW);
//        }

      
      }

  }
  else{
    Serial.println("No light found");
  }
//-----------------------------------------------------------------------




//-----------------------------------append json to RTDB------------------------------
FirebaseJson updateData;
FirebaseJson json;
//json.set("/alias","hala's room");
//FirebaseJson lights;
//lights.set("/room1/light1_status",1);
//lights.set("/room1/alias","hala's rooom");
//updateData.set("room2/alias","hiba's room");
//updateData.set("room1", json);
//updateData.set("room1",lights);

//if (Firebase.updateNode(firebaseData, "/8HcAT87dasVTkdgBGc7qoUg8LY03/rooms", lights)) {
//
//  Serial.println(firebaseData.dataPath());
//
//  Serial.println(firebaseData.dataType());
//
//  Serial.println(firebaseData.jsonString()); 
//
//} else {
//  Serial.println(firebaseData.errorReason());
//}


//--------the following 9 lines is to read the light status of a hard coded path----------------

  if (Firebase.getString(firebaseData, "/VMTnWxaqxrQwVEPeViX53pWjRBH3/rooms/room1/power-on")) {                          

//    if (firebaseData.dataType() == "int") {                           

    
//      val = firebaseData.intData();
      String power_on = firebaseData.stringData();
       Serial.println("power_on:  ");
      Serial.println(power_on);
//      }

      if (power_on == timmme)
      {
                     
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(1000);                       // wait for a second
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        delay(1000);     
        Serial.println("EQUAL"); 

      }

  }

//-------------------------------------------------------

delay(1000);
//--------------------------------------------------------------------------------


}
