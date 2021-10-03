#include "ESP8266WiFi.h"

const char* ssid = "DevoMechSolutions"; //Enter SSID
const char* password = "devomech1212"; //Enter Password
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
 
// Set these to run example.
#define FIREBASE_HOST "ndtry-67d9e-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "on2bdtf57ONT1jPqE4VdMse1YYtmPMftBlMLUIdU"
#define WIFI_SSID "DevoMechSolutions" //provide ssid (wifi name)
#define WIFI_PASSWORD "devomech1212" //wifi passwordvoid setup()
void setup(void)
{ 
  Serial.begin(115200);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
     delay(500);
     Serial.print("*");
  }
  
  Serial.println("");
  Serial.println("WiFi connection Successful");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.println(WiFi.localIP());// Print the IP address


  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
   Serial.flush();
 if(Firebase.failed())
    {
    Serial.print(Firebase.error());
      Serial.print("Firebase not Connected");

    }
    else{
    
    Serial.print("Firebase Connected");
    Firebase.setString("/Value","0");
    }
 
 
 }


void loop() 
{

  for(int i = 0; i <= 100; i++)
 { 
    Firebase.setInt("/Value",i);
      Serial.println(i);

    delay(1000);
 }
 
}
