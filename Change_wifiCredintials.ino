//#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
//#include <FirebaseESP8266.h>
#include <Arduino.h>
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"


//Variables
int i = 0;
int statusCode;
const char* ssid = "admin";
const char* passphrase = "admin";

//#define FIREBASE_HOST "homeautomation-6d999-default-rtdb.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
//#define FIREBASE_AUTH "1rknUEZTjXVlUz6avz3mx9nnHlC9zxYu1PHjBiPy" //Your Firebase Database Secret goes here

#define API_KEY "AIzaSyCdrQzBlbljlRRrOX4oL5lU8-dxcKpzgR0"

#define USER_EMAIL "admin@admin.com"
#define USER_PASSWORD "admin@admin.com"




int val=0; 

FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
String st;
String content;
String uid;


//Function Decalration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);

//Establishing Local server at port 80 whenever required
ESP8266WebServer server(80);

void setup()
{

  Serial.begin(115200); //Initialising if(DEBUG)Serial Monitor
//    for (int i = 0; i < 512; i++) {
//       EEPROM.write(i, 0);
//     }
//     EEPROM.commit();
  delay(10);
  
  Serial.println();
  Serial.println("Disconnecting current wifi connection");
  WiFi.disconnect();
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println();
  Serial.println();
  Serial.println("Startup");

  //---------------------------------------- Read eeprom for ssid and pass
  Serial.println("Reading EEPROM ssid");

  String esid;
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  Serial.println();
  Serial.print("SSID: ");
  Serial.println(esid);
  Serial.println("Reading EEPROM pass");

  String epass = "";
  for (int i = 32; i < 96; ++i)
  {
    epass += char(EEPROM.read(i));
  }
  Serial.print("PASS: ");
  Serial.println(epass);

//  ----- uemail: user's email that will be used to access his db in firebase-----

  String uemail= "";
  for (int i =96 ; i < 128 ; ++i)
  {
    uemail += char(EEPROM.read(i));
  }
  Serial.print("Email: ");
  Serial.println(uemail);

//  ----- upass: user's passowrd that will be used to access his db in firebase-----
  String upass = "";
  for (int i=128 ; i < 192 ; ++i)
  {
    upass+= char(EEPROM.read(i));
  }

   Serial.print("Password: ");
   Serial.println(upass);
   
  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi())
  {
    Serial.println("Succesfully Connected!!!");
    Serial.println(esid.c_str());
    Serial.println(epass.c_str());

     config.api_key = API_KEY;
     auth.user.email = uemail;
     auth.user.password = upass;
     Firebase.reconnectWiFi(true);
     firebaseData.setResponseSize(4096);
     config.token_status_callback = tokenStatusCallback;
     // Assign the maximum retry of token generation
      config.max_token_generation_retry = 5;
     Firebase.begin(&config, &auth);

     if((auth.token.uid) == "")
      {
        Serial.println("UID is null. Turning the HotSpot On");
        launchWeb();
        setupAP();// Setup HotSpot   
      }
        
//    return;
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    launchWeb();
    setupAP();// Setup HotSpot
  }

  config.api_key = API_KEY;
  auth.user.email = uemail;
  auth.user.password = upass;
   Firebase.reconnectWiFi(true);
  firebaseData.setResponseSize(4096);

   // Assign the callback function for the long running token generation task
  config.token_status_callback = tokenStatusCallback;
  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  Firebase.begin(&config, &auth);
  
  
  Serial.println();
  Serial.println("Waiting.");

  while ((WiFi.status() != WL_CONNECTED))
  {
    Serial.print(".");
    delay(100);
    server.handleClient();
  }

  pinMode(D0,OUTPUT);
//  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);   // connect to firebase
//  Serial.println("Connected to firebase");
 
   // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");

  if((auth.token.uid) == "")
  {
    Serial.println("UID is null. Turning the HotSpot On");
//    launchWeb();
//    setupAP();// Setup HotSpot   
    Firebase.reconnectWiFi(true);
    Firebase.begin(&config, &auth);
  }
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }

  if((auth.token.uid) != "")
  {
        // Print user UID
        uid = auth.token.uid.c_str();
        Serial.print("User UID: ");
        Serial.print(uid);
  }

}
void loop() {
  if ((WiFi.status() == WL_CONNECTED))
  {

    for (int i = 0; i < 3; i++)
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }

  }
//  else
//  {
//  }

//  if (Firebase.getInt(firebaseData, "LED_STATUS")) {                           // On successful Read operation, function returns 1  
//
//    if (firebaseData.dataType() == "int") {                            // print read data if it is integer
//
//      val = firebaseData.intData();
//      Serial.println(val);
//      Serial.println("\n Change value at firebase console to see changes here."); 
////      delay(1000);
//      if((int)val==1){
//        Serial.println("LED is ON");
//        digitalWrite(D0,HIGH);
////        delay(500);
//      }
//      else{
//        Serial.println("LED is OFF");
//        digitalWrite(D0,LOW);
////        delay(500);
//      }
//    }
//
////    delay(500);
// }

  

}


//----------------------------------------------- Fuctions used for WiFi credentials saving and connecting to it which you do not need to change
bool testWifi(void)
{
  int c = 0;
  Serial.println("Waiting for Wifi to connect");
  while ( c < 20 ) {
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  return false;
}

void launchWeb()
{
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  // Start the server
  server.begin();
  Serial.println("Server started");
}

void setupAP(void)
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);

    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.softAP("ElectronicsInnovation", "");
  Serial.println("Initializing_softap_for_wifi credentials_modification");
  launchWeb();
  Serial.println("over");
}

void createWebServer()
{
  {
    server.on("/", []() {

      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE HTML>\r\n<html>Welcome to Wifi Credentials Update page";
      content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
      content += ipStr;
      content += "<p>";
      content += st;
      content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64>";
      content += "<br> <br> <label> Email </label> <input name='email' length=32> <label> Password: </label> <input name='userPassword' length=64>";
      content +="<input type='submit'></form>";
      content += "</html>";
      server.send(200, "text/html", content);
    });
    server.on("/scan", []() {
      //setupAP();
      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

      content = "<!DOCTYPE HTML>\r\n<html>go back";
      server.send(200, "text/html", content);
    });

    server.on("/setting", []() {
      String qsid = server.arg("ssid");
      String qpass = server.arg("pass");
      String email = server.arg("email");
      String userpassword = server.arg("userPassword");
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 192; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
        Serial.println(email);
        Serial.println("");
        Serial.println(userpassword);

        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
         Serial.println("writing eeprom email:");
        for (int i = 0; i < email.length(); ++i)
        {
          EEPROM.write(96 + i, email[i]);
          Serial.print("Wrote: ");
          Serial.println(email[i]);
        }

         Serial.println("writing eeprom userPassword:");
        for (int i = 0; i < userpassword.length(); ++i)
        {
          EEPROM.write(128 + i, userpassword[i]);
          Serial.print("Wrote: ");
          Serial.println(userpassword[i]);
        }
        
        EEPROM.commit();

        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.reset();
      }
      
      else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);

    });
  }
}
