#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(D3,D4);
DFRobotDFPlayerMini myDFPlayer;
 

#define FIREBASE_HOST "remotesensingapp.firebaseio.com"
#define FIREBASE_AUTH "NMXI9EPJ28tjhbZL8gIg0s4SQV5SHm38KOruVYyw"
#define WIFI_SSID "Peter Hotspot"
#define WIFI_PASSWORD "Walquis1220"
String DEVICE_ID = "1";
String Position = "NA";




void setup() {  
  
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
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  // Firebase Connection

  // RGB Leds Connection
  pinMode(D2, OUTPUT);  // R
  pinMode(D8, OUTPUT);  // G//T
  pinMode(D1, OUTPUT);  // B//d1
  //pinMode(D8, OUTPUT);  //W


  digitalWrite(D2,LOW);
  digitalWrite(D8,LOW);
  digitalWrite(D8,LOW);
  //digitalWrite(D8, LOW);


 //SD card Player Initilization

  Serial.begin(115200);
  mySoftwareSerial.begin(9600);  // Used for Communication With the SD CardReader
  Serial.println();
  Serial.println("DFRobotPlayerMini Demo");
  Serial.println("Initiliazation DFPlayer ..... May take 2 to 3 minutes");
  delay(500);
  if (!myDFPlayer.begin(mySoftwareSerial))
      {
        Serial.println("Unable to begin DFPlayer............... Please Check Connections.  or Insert SD card");
        Serial.println("2.Please insert the SD card!");
      }
 
   Serial.println("DFPlayer Online");
   myDFPlayer.volume(30);         // Setting Volume
   
  
}

void loop() 
{
 String voice_arr[] = {"Device is full","Device is half","Device is Low","El dispositivo está lleno","El dispositivo está encendido","El dispositivo está vacío","L'appareil est plein","L'appareil est allumé","L'appareil est bas","Device Empty","Dispositivo vacío","Appareil vide"}; 
 //-----------------Device on
 Serial.println("Device is On");
 Position = voice_arr[1];
 delay(100);
 myDFPlayer.play(4);  
 delay(100);
 sendMessage(Position);

 Serial.println("El dispositivo está encendido---Spanish");
 Position = voice_arr[4];
 delay(100);
 myDFPlayer.play(5);  
 delay(100);
 sendMessage(Position);

 Serial.println("L'appareil est allumé-----French");
 Position = voice_arr[7];
 delay(100);
 myDFPlayer.play(6);  
 delay(100);
 sendMessage(Position);
 Led_Signal(1,1,1);
 
 //--------------------------------Device is full
 Serial.println("Device is full");
 Position = voice_arr[0];
 delay(100);
 myDFPlayer.play(1);
 delay(100);
 sendMessage(Position);

 Serial.println("El dispositivo está lleno --- Spanish");
 Position = voice_arr[3];
 delay(100);
 myDFPlayer.play(2);
 delay(100);
 sendMessage(Position);

 Serial.println("L'appareil est plein --- French");
 Position = voice_arr[6];
 delay(100);
 myDFPlayer.play(3);
 delay(100);
 sendMessage(Position);

 Led_Signal(0,1,0); 

 

 //-----------Device is Low

 Serial.println("Device is Low");
 Position = voice_arr[2];
 delay(100);
 myDFPlayer.play(7);  //Play the second mp3
 delay(100);
 sendMessage(Position);

Serial.println("El dispositivo es bajo------Spanish");
 Position = voice_arr[5];
 delay(100);
 myDFPlayer.play(8);  //Play the second mp3
 delay(100);
 sendMessage(Position);

 Serial.println("L'appareil est bas --- -- French");
 Position = voice_arr[8];
 delay(100);
 myDFPlayer.play(9);  //Play the second mp3
 delay(100);
 sendMessage(Position);
 Led_Signal(0,0,1);


//------------------------Device Empty
 Serial.println("Device Empty");
 Position = voice_arr[9];
 delay(100);
 myDFPlayer.play(10);  //Play the second mp3
 delay(100);
 sendMessage(Position);
 
 Serial.println("Dispositivo vacío -----Spanish");
 Position = voice_arr[10];
 delay(100);
 myDFPlayer.play(11);  //Play the second mp3
 delay(100);
 sendMessage(Position);

 Serial.println(" Appareil vide--- -- French");
 Position = voice_arr[11];
 delay(100);
 myDFPlayer.play(12);  //Play the second mp3
 delay(100);
 sendMessage(Position);
 
 Led_Signal(1,0,0);

 delay(120000);

}

 
 
void sendMessage(String noti)
{      
     
     String msg_data = noti;
     String path = "/Users/"+ DEVICE_ID+"/latest";
     Firebase.setString(path, msg_data);
     path = "/Users/"+ DEVICE_ID+"/messages";
     Firebase.pushString( path, msg_data);
     if (Firebase.failed()) 
    {
        Serial.print("Sending message failed of Device On Audio File:");
        Serial.println(Firebase.error());  
        return;
    }
     
    
   
}




 void Led_Signal(bool R, bool G, bool B)
  {
    


    digitalWrite(D2, R);
    digitalWrite(D8, G);
    digitalWrite(D1, B);
   // digitalWrite(D8, W);

    delay(5000);
    digitalWrite(D2, LOW);
    digitalWrite(D8, LOW);
    digitalWrite(D1, LOW);
   // digitalWrite(D1, LOW);

    delay(500);

  }
