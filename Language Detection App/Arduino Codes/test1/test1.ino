#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "remotesensingapp.firebaseio.com"
#define FIREBASE_AUTH "NMXI9EPJ28tjhbZL8gIg0s4SQV5SHm38KOruVYyw"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#define WIFI_SSID "Peter HotSpot"
#define WIFI_PASSWORD "Walquis1220"
SoftwareSerial mySoftwareSerial(D3, D4); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

// defines pins numbers
const int trigPin = D6;
const int echoPin = D7;

double duration;
float distance;
int device_status = 0;

void setup() {
//Green D1, Blue D2, Red D8
pinMode(D1 , OUTPUT);
pinMode(D2 , OUTPUT);
pinMode(D8 , OUTPUT);
digitalWrite(D1 , LOW);
digitalWrite(D2 , LOW);
digitalWrite(D8 , LOW);
 mySoftwareSerial.begin(9600);
 Serial.begin(9600);
// WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
//  Serial.print("connecting");
//  while (WiFi.status() != WL_CONNECTED) {
//    Serial.print(".");
//    delay(500);}
//    Serial.println();
//  Serial.print("connected: ");
//  Serial.println(WiFi.localIP());
// Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
 pinMode(echoPin, INPUT); // Sets the echoPin as an Input
// Serial.println("working"); 
 Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  delay(1000);
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    return;
  }
  Serial.println(F("DFPlayer Mini online."));
   myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(30);  //Set volume value (0~30).
   myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
   myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
   
/*The audio files are played in the same order they were copied in the SD card...
so the file copied first will be played by "1" in the play function and the one copied ninth by "9".*/

    Serial.println("Your Device is ON ");
    myDFPlayer.play(1);  //Play the ON message in English
    delay(2000);
    myDFPlayer.play(2);  //Play the ON message in French
    delay(2000);
    myDFPlayer.play(3);  //Play the ON message in Spanish
    delay(2000);
    LED_Signal(1, 1, 1);  //Generate signals for LEDs (Red, Green, Blue)

digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance in inches
distance= duration*0.0135/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" inches");

//Red D2, Green D8, Blue D1
//Full for <= 3, mid for between 6 and 8, empty after 10

if(distance <=3){
  if(device_status!= 1){
    //To emit Green light
//    digitalWrite(D2 , LOW);
//    digitalWrite(D8 , HIGH);
//    digitalWrite(D1 , LOW);
    Serial.println("Device full");
    device_status = 1;
    Firebase.setInt("sensor1" , 1);
    myDFPlayer.play(4);  //Play the empty message in English
    delay(2000);
    myDFPlayer.play(5);  //Play the empty message in French
    delay(2000);
    myDFPlayer.play(6);  //Play the empty message in Spanish
    LED_Signal(0, 1, 0);  //Generate signals for LEDs (Red, Green, Blue)
  }
}

if((distance >= 6) && (distance <= 8 )){
  if(device_status!= 2){
    //To emit Blue light
//    digitalWrite(D2 , LOW);
//    digitalWrite(D8 , LOW);
//    digitalWrite(D1 , HIGH);
    Serial.println("Device low");
    device_status = 2;
    Firebase.setInt("sensor1" , 2);
    myDFPlayer.play(7);  //Play the low message in English
    delay(2000);
    myDFPlayer.play(8);  //Play the low message in French
    delay(2000);
    myDFPlayer.play(9);  //Play the low message in Spanish
    LED_Signal(0, 0, 1);  //Generate signals for LEDs (Red, Green, Blue)  
  }
}

if(distance >= 10.5){
  if(device_status!= 3){
    //To emit Red light
//    digitalWrite(D2 , HIGH);
//    digitalWrite(D8 , LOW);
//    digitalWrite(D1 , LOW);
    Serial.println("Device is empty");
    device_status = 3;
    Firebase.setInt("sensor1" , 3);
    myDFPlayer.play(10);  //Play the empty message in English
    delay(2000);
    myDFPlayer.play(11);  //Play the empty message in French
    delay(2000);
    myDFPlayer.play(12);  //Play the empty message in Spanish
    LED_Signal(1, 0, 0);  //Generate signals for LEDs (Red, Green, Blue)
  }
}

Serial.println("deep sleep for 5 seconds");
ESP.deepSleep(5e6);
 
}
void loop() {

}

void LED_Signal(bool R,bool G,bool B){
  for(int count = 0; count < 3; count++){
    digitalWrite(D2 , R);
    digitalWrite(D8 , G);
    digitalWrite(D1 , B);
    delay(1000);  //Signal ON for 1 second
    digitalWrite(D2 , LOW);
    digitalWrite(D8 , LOW);
    digitalWrite(D1 , LOW);
    delay(500);  //Signal OFF for 0.5 second
  }
}
