 #include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <EEPROM.h>
#include "Arduino.h"
#include <SoftwareSerial.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <WiFiClientSecure.h>
#include "DFRobotDFPlayerMini.h"
#include "twilio.hpp"

String WIFI_SSID  ="Nayatel";
String WIFI_PASSWORD = "Apple123";
String DEVICE_ID = "";  


#define FIREBASE_HOST "projectpeter-2d64f.firebaseio.com/" 
#define FIREBASE_AUTH "eam8uTO7HA1E43rFOlrqHEmx2naI0l9JtLltADwI"
#define FIREBASE_FCM_SERVER_KEY "AAAAy8aTX7k:APA91bFl6vGDma8Ht9PLqRUFm698wFCoBbe9lcmHvGJPY4S0EfO2TEDp8OlYBVrHgGqlRh2zeYQpC41U3sfTkRkjOLCROIIMznYtDszrhFORcEXShOBgc0y1DTLQVCf7KTfT8A02dPiL"
 

const char* account_sid = "AC2e7454548a27b0de9832037ef3b8d419";
const char* auth_token = "a79f5b5c5eb9d8f5f77142b50d934abf";
const char fingerprint[] = "BC B0 1A 32 80 5D E6 E4 A2 29 66 2B 08 C8 E0 4C 45 29 3F D0";

String phone1="N", phone2="N", phone3="N";
String lang_e="English";

SoftwareSerial mySoftwareSerial(D3, D4); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);


FirebaseData firebaseData1,firebaseData;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600);
// defines pins numbers
const int trigPin = D6;
const int echoPin = D7;

long duration;
int distance;
int device_status = 0;

String token_r, ssid_r, pass_r, id_r;
String token_e, ssid_e, pass_e, id_e;
String Position = "NA";
String pending[100];
String Time[100];
int pending_index =0; 

String message_body    = "Hello from Twilio and the ESP8266!";
String to_number    = "+923176239744";
String from_number = "+14045667657";
String media_url = "";
String master_number    = "+14045667657";

void(* resetFunc) (void) = 0;

Twilio *twilio;
WiFiServer twilio_server(8000);

bool default_credentails = false;

void setup() {
//Green D1, Blue D2, Red D8
  pinMode(D1 , OUTPUT);
  pinMode(D2 , OUTPUT);
  pinMode(D8 , OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
 
  digitalWrite(D1 , LOW);
  digitalWrite(D2 , LOW);
  digitalWrite(D8 , LOW);
  
  mySoftwareSerial.begin(115200);
  Serial.begin(115200);
  EEPROM.begin(512);

  String read_ssid = Eeprom("",1,false);
  Serial.println(read_ssid);
  ssid_e = getValue(read_ssid, '%',1);
  ssid_e.replace("%","");
  Serial.println("&&&&&&&&&&&&&&&&&&&&&");
  Serial.println(ssid_e);
  pass_e = getValue(Eeprom("",2,false),'&',1);
  pass_e.replace("&","");
  id_e = (Eeprom("",3,false));
  
  int now_sec = millis();

  WiFi.begin(ssid_e, pass_e);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    default_credentails = false;
    if(millis()-now_sec >20000) break;
  }

  Serial.println("--------");
  Serial.print("Stored SSID: ");
  Serial.println(ssid_e);
  Serial.print("Stored PASS: ");
  Serial.println(pass_e);
  
  if(WiFi.status() != WL_CONNECTED){

    Serial.println("Wifi not available connecting to default network");
    default_credentails = true;
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  }

  DEVICE_ID  = WiFi.macAddress();
  Serial.println("##########################################");
  Serial.print("Device ID: ");
  Serial.println(DEVICE_ID);
  Serial.println("##########################################");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);


  firebaseData1.setBSSLBufferSize(1024, 1024);
  firebaseData1.setResponseSize(1024);
  firebaseData1.fcm.begin(FIREBASE_FCM_SERVER_KEY);
  firebaseData1.fcm.setPriority("high");
  firebaseData1.fcm.setTimeToLive(1000);
  
  twilio = new Twilio(account_sid, auth_token, fingerprint);
      twilio_server.begin();
  

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  delay(1000);


  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  }
  else{
  Serial.println(F("DFPlayer Mini online."));
     myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
     myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
     myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  }

  timeClient.begin();

  updateData(false);


}


unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

String concatData;
int increment=0;
void loop() {
  
  timeClient.update();

  String path = "/Users/"+ DEVICE_ID+"/change";

  if(Firebase.get(firebaseData, path)){
    if(firebaseData.boolData()){
      updateData(true);
       String path = "/Users/"+ DEVICE_ID+"/change";
       if(Firebase.set(firebaseData, path, false)){
           Serial.println("Set change data success");
        }else{    
          Serial.print("Error in set chnage, ");
          Serial.println(firebaseData.errorReason());
       }
    }
  }
  
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance= duration*0.0135/2;
    // Prints the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.println(distance);
    //Green D1, Blue D2, Red D8
    //full from =< 3, mid for between 6 and 8, empty after 10
    
    
    if(lang_e .equals("English")){
        increment =0;
    }
    else if(lang_e.equals("Spanish")){
        increment = 3;
    }
    else if(lang_e .equals("French")){
        increment = 6;
    }

   path = "/Users/"+ DEVICE_ID+"/Text";
  if(Firebase.get(firebaseData, path)){
     String var = firebaseData.stringData();
     if(var.equals("far")){
        distance = 10;
     }
     else if(var.equals("veryclose")){
        distance = 1;  
     }
     else if(var.equals("close")){
        distance = 8;
     }

     if(!var.equals("N/A")){
        Firebase.set(firebaseData, path,"N/A");
     }
  }
  
    String voice_arr[] = {"Device is full","Device is half","Device is empty","El dispositivo está lleno","El dispositivo es la mitad","El dispositivo está vacío","L'appareil est plein","L'appareil est à moitié","L'appareil est vide"}; 
    if(distance ==3 || distance <3){
      if(device_status!= 1){
        digitalWrite(D1 , LOW);
        digitalWrite(D2 , LOW);
        digitalWrite(D8 , HIGH);
      //  Serial.println("Device is full");
        device_status = 1;
        Position = voice_arr[0+increment];
        delay(100);
        myDFPlayer.play(2+increment);  //Play the first mp3
        delay(4000);
        sendMessage(Position);
    }
  }

    else if((distance > 6  ||distance == 6) && (distance == 8 || distance < 8 )){
      if(device_status!= 2){
        digitalWrite(D1 , HIGH);
        digitalWrite(D2 , LOW);
        digitalWrite(D8 , LOW);
        Position = voice_arr[1+increment];
        device_status = 2;
        delay(100);
        myDFPlayer.play(3+increment);  //Play the second mp3
        delay(4000);
        sendMessage(Position);
      }
    }
    
    else if(distance <=15 && distance > 8){
      if(device_status!= 3){
        digitalWrite(D2 , HIGH);
        digitalWrite(D1 , LOW);
        digitalWrite(D8 , LOW);
        Position = voice_arr[2+increment];
       // Serial.println("Device is empty");
        device_status = 3;
        delay(100);
        myDFPlayer.play(1+increment);  //Play the third mp3
        delay(4000);
        sendMessage(Position);
      }
    }
 delay(1000);
}



bool boot = true;
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

     firebaseData1.fcm.setNotifyMessage("Notification from your device", noti);
   
    if (Firebase.sendMessage(firebaseData1, 0))//send message to recipient index 0
    {

        Serial.println("PASSED");
        Serial.println(firebaseData1.fcm.getSendResult());
        Serial.println("------------------------------------");
        Serial.println();
    }
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData1.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
    
   

    pending[pending_index] = noti;
    Time[pending_index] = getdateTime();
    pending_index++;

    if(!boot){
      twilio = new Twilio(account_sid, auth_token, fingerprint);
      twilio_server.begin();
      String response;
      bool success = twilio->send_message(
              to_number,
              from_number,
              noti,
              response,
              media_url
      );    
      Serial.println(response);  
      
    }
    boot = false;

    

}

String Eeprom(String data, int cat, bool Write){
  int base_addr =0;
  String result ="";
  Serial.println(data);
  
  switch(cat){
    case 1 :
        base_addr = 0 ;
        break;
    case 2 :
        base_addr = 15;
        break;
    case 3:
        base_addr = 30;
        break;    
    case 4:
        base_addr = 45;
        break;        
    case 5:
        base_addr = 60;
        break;
    case 6:
        base_addr = 75;                  
        break;
    case 7:
        base_addr = 90;
        break;
    case 8:
        base_addr = 95;    
        break;
  }

  if(Write){
    for(int i=base_addr;i<base_addr+data.length();i++){
      EEPROM.write(0x0F+i, data[i-base_addr]); 
    }
    EEPROM.commit();
    Serial.print(data);
    Serial.print("\t");
    Serial.println("write success!");
    
    
  }
  else{
    for(int i=base_addr;i<base_addr+153;i++){
      char a = char(EEPROM.read(0x0F+i));
      if(a=='$'){
        break;
        Serial.println("Break");
        return result;
      }
      result = result+a;
    }
  }
  return result;
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


void updateData(bool credential){

  int offset=0;
  String parent_path = "Users/"+DEVICE_ID;
  String path = parent_path+"/offset";

  path = "/Users/"+ DEVICE_ID+"/offset";

  if(Firebase.get(firebaseData, path)){
    Serial.print("Get variant data success, type = ");
    Serial.println(firebaseData.dataType());
    offset = firebaseData.intData();
  }

  Serial.println(offset);

  timeClient.setTimeOffset(offset);


  int vol = 10;
  path = parent_path+"/volume";
  if(Firebase.get(firebaseData, path)){
    vol = firebaseData.intData();
    myDFPlayer.volume(vol);  //Set volume value (0~30).
  }

  path = parent_path+"/p1";
  if(Firebase.get(firebaseData, path)){
    phone1 = firebaseData.stringData();
  }

   path = parent_path+"/p2";
  if(Firebase.get(firebaseData, path)){
    phone2 = firebaseData.stringData();
  }

   path = parent_path+"/p3";
  if(Firebase.get(firebaseData, path)){
    phone3 = firebaseData.stringData();
  }

   path = parent_path+"/language";
  if(Firebase.get(firebaseData, path)){
    lang_e = firebaseData.stringData();
  }

  path = parent_path+"/token";
  if(Firebase.get(firebaseData, path)){
        String tok = firebaseData.stringData();
        Serial.println(tok);
        firebaseData1.fcm.addDeviceToken(tok);
  }
  Serial.println(default_credentails);
  if(!credential){
    bool rst = false;
     path = parent_path+"/ssid";
    if(Firebase.get(firebaseData, path)){
      String data = firebaseData.stringData();
  
      if(!ssid_e.equals(data)){
         String writeData = "%"+data+"%";
         Eeprom(writeData, 1, true);
         rst = true;
      }
    }
  
     path = parent_path+"/pass";
    if(Firebase.get(firebaseData, path)){
      String data = firebaseData.stringData();
      
      if(!pass_e.equals(data)){
         String writeData = "&"+data+"&";
         Eeprom(writeData, 2, true);
         rst = true;
      }
    }
    if(rst)
    resetFunc();
  }

  
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
