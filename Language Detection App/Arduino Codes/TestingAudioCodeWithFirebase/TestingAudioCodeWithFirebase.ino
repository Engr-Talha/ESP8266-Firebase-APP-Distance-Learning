
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
FirebaseData firebaseData;

SoftwareSerial mySoftwareSerial(D3,D4);
DFRobotDFPlayerMini myDFPlayer;

String On = "On";;
String Low = "Low";;
String Full = "Full" ;   

// Set these to run example.
#define FIREBASE_HOST "remotesensingapp.firebaseio.com"
#define FIREBASE_AUTH "NMXI9EPJ28tjhbZL8gIg0s4SQV5SHm38KOruVYyw"
#define WIFI_SSID "DevoMechSolutions"
#define WIFI_PASSWORD "devomech1212"
String DEVICE_ID = "1";

void Device_On_AudioFile(int n);
void Device_Full_AudioFile(int n);
void Device_Low_AudioFile(int n);


void setup() {  
  Serial.begin(9600);
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
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  // Firebase Connection

  // RGB Leds Connection
  pinMode(D1, OUTPUT);  // Blue
  pinMode(D2, OUTPUT);  // Red
  pinMode(D8, OUTPUT);  // Green

  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW);
  digitalWrite(D3,LOW);

// SD card Player Initilization
//  Serial.println();
//  Serial.println(F"DFRobotPlayerMini Demo");
//  Serial.println(F"Initiliazation DFPlayer ..... May take 2 to 3 minutes");
//  delay(500);
//  if (!myDFPlayer.begin(mySoftwareSerial))
//      {
//        Serial.println("Unable to begin DFPlayer............... Please Check Connections.  or Insert SD card");
//        return;
//      }
//   Serial.println("DFPlayer Online");
//   myDFPlayer.setTimeout(500);    // Serial Communication timeout 
//   myDFPlayer.volume(30);         // Setting Volume
//   myDFPlayer.ED(DFPLAYER_EQ_NORMAL);
//   myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  
}
int n=0;
void loop() {
  while (true)
  {      
      Device_On_AudioFile(n);
      delay(1000);
      Device_Full_AudioFile(n);
      delay(1000);
      Device_Low_AudioFile(n);
      n++;
    } 

}
void Device_On_AudioFile(int n)
  {
    
    // Device on
//    Firebase.setString("/Device_On/message in French", "Appareil activé");
//    delay(100);
//     Firebase.setString("/Device_On/message in Spanish", "Dispositivo encendido");
//     delay(100);
//    Firebase.setString("/Device_On/message in English", "Device On");
//    delay(100);

    Firebase.setInt("/Device_On/counter", n);                   delay(100);
    Firebase.setString("/Device_On/message in French",  On);    delay(100);
    Firebase.setString("/Device_On/message in Spanish", On);    delay(100);
    Firebase.setString("/Device_On/message in English", On);    delay(100);
    Led_Signal(1,0,0);

    if (On == "On")
    {
      On="Off";
      }
      else
      {
        On="On";
      }
      
    
    // handle error
    if (Firebase.failed()) 
    {
        Serial.print("Sending message failed of Device On Audio File:");
        Serial.println(Firebase.error());  
        return;
    }
  }

void Device_Low_AudioFile(int n)
  {
    // Device Full
//    Firebase.setString("/Device_Low/message in French", "Appareil activé");
//    delay(100);
//     Firebase.setString("/Device_Low/message in Spanish", "Dispositivo encendido");
//     delay(100);
//    Firebase.setString("/Device_Low/message in English", "Device Full");
//    delay(100);
    Firebase.setInt("/Device_Low/counter", n); delay(100);
    Firebase.setString("/Device_Low/message in French", Low); delay(100);
    Firebase.setString("/Device_Low/message in Spanish", Low); delay(100);
    Firebase.setString("/Device_Low/message in English", Low); delay(100);
    Led_Signal(0,1,0);
    if (Low == "Low")
    {
      Low="High";
      }
      else
      {
        Low="Low";
      }
    // handle error
    if (Firebase.failed()) 
    {
        Serial.print("Sending message failed of Device On Audio File:");
        Serial.println(Firebase.error());  
        return;
    }
  }

void Device_Full_AudioFile(int n)
  {
    // Device Full
//    Firebase.setString("/Device_Full/message in French", "Appareil activé");
//    delay(100);
//     Firebase.setString("/Device_Full/message in Spanish", "Dispositivo encendido");
//     delay(100);
//    Firebase.setString("/Device_Full/message in English", "Device Full");
//    
    Firebase.setInt("/Device_Full/counter", n); delay(100);
    Firebase.setString("/Device_Full/message in French", Full);
    Firebase.setString("/Device_Full/message in Spanish", Full);
    Firebase.setString("/Device_Full/message in English", Full);


    Led_Signal(0,0,1);
    if (Full == "Full")
    {
      Full="Empty";
      }
      else
      {
        Full="Full";
      }

    
    
    // handle error
    if (Firebase.failed()) 
    {
        Serial.print("Sending message failed of Device On Audio File:");
        Serial.println(Firebase.error());  
        return;
    }
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

void sendMessage(String noti)
{      
     
     String msg_data = noti+"&"+getdateTime();
     String path = "/Users/"+ DEVICE_ID+"/latest";
     if(Firebase.set(firebaseData, path, msg_data)){
         Serial.println("Set int data success");
      }else{    
        Serial.print("Error in setInt, ");
        Serial.println(firebaseData.errorReason());
     }

     path = "/Users/"+ DEVICE_ID+"/messages";
     if(Firebase.push(firebaseData, path, msg_data)){
         Serial.println("Set int data success");
      }else{    
        Serial.print("Error in setInt, ");
        Serial.println(firebaseData.errorReason());
     }

   
}



  String getdateTime(){

  unsigned long epochTime = timeClient.getEpochTime();

  String date_time = "";
  date_time += (timeClient.getHours());
  date_time += ":";
  date_time += (timeClient.getMinutes());
  date_time += "  ";

  struct tm *ptm = gmtime ((time_t *)&epochTime); 
  date_time +=  ptm->tm_mday;
  date_time += "-";
  date_time +=  ptm->tm_mon+1;
  date_time += "-";
  date_time += ptm->tm_year+1900;

  if(date_time.endsWith("1970")){
    date_time = "Time Not Available";
  }

  return date_time;
}
