
#include <ESP8266WiFi.h>



#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(D6,D7);
DFRobotDFPlayerMini myDFPlayer;
String Position = "NA";

 

// Set these to run example.

#define WIFI_SSID "DevoMechSolutions"
#define WIFI_PASSWORD "devomech1212"


void setup() {  
  Serial.begin(115200);
  mySoftwareSerial.begin(9600);  // Used for Communication With the SD CardReader
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected to the IP : ");
  Serial.println(WiFi.localIP());
  

  // RGB Leds Connection
//  pinMode(D1, OUTPUT);  // Blue
//  pinMode(D2, OUTPUT);  // Red
//  pinMode(D8, OUTPUT);  // Green
//
//  digitalWrite(D1,LOW);
//  digitalWrite(D2,LOW);
//  digitalWrite(D8,LOW);

 //SD card Player Initilization
  Serial.println();
  Serial.println("DFRobotPlayerMini Demo");
  Serial.println("Initiliazation DFPlayer ..... May take 2 to 3 minutes");
  if (!myDFPlayer.begin(mySoftwareSerial))
      {
        Serial.println("Unable to begin DFPlayer............... Please Check Connections.  or Insert SD card");
        Serial.println("2.Please insert the SD card!");
      }
 
   Serial.println("DFPlayer Online");
   myDFPlayer.setTimeOut(500);    // Serial Communication timeout 
    myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);
  
}


void loop() 
{

 //--------------------------------Device is full
 Serial.println("Device is full");
 delay(1000);
 myDFPlayer.play(4);
 delay(1000);
 Led_Signal(1,0,0);
  Serial.println("LED is Blinks");


 
 
 



}







 void Led_Signal(bool R, bool G, bool B)
  {
    Serial.println(R);
    Serial.println(G);
    Serial.println(B);

    digitalWrite(D2, R);
    digitalWrite(D1, G);
    digitalWrite(D8, B);
    delay(500);
    digitalWrite(D2, LOW);
    digitalWrite(D1, LOW);
    digitalWrite(D8, LOW);
    delay(500);

  }
