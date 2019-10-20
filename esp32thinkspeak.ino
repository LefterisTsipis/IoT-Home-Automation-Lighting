//cosntants for the pins where sensors are plugged into.
#include "ThingSpeak.h"
#include "secrets.h"
#include <WiFi.h>

 int sensorPin = 32;
 int ledPin = 5;
  int ledPin2 = 14;
int lightInit;  // initial value
int lightVal;   // light reading

char ssid[] = SECRET_SSID;   //  network SSID (name) 
char pass[] = SECRET_PASS;   //  network password
int keyIndex = 0;            //  network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int number = 100;
long writingTimer = 20; 
long startTime = 0;
long waitTime = 0;

void setup()
{
  // We'll set up the LED pin to be an output.
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(sensorPin,INPUT);
  Serial.begin(115200);  //Initialize serial
   WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);
}
void loop()
{
   
wifiReconect();
Automated_Outdoor_Lighting();
ThingSpeakSendData(); 

  
}

void Automated_Outdoor_Lighting()
{  lightVal = analogRead(sensorPin); // read the current light levels
  Serial.println(lightVal); 
  //if lightVal is less than our initial reading withing a threshold then it is dark.
  //**********************************************************************************
  if(lightVal>=370 &&lightVal<=980)
  {
      digitalWrite (ledPin, HIGH); // turn on light
      digitalWrite (ledPin2, LOW);// turn off light
     }
  //otherwise, it is bright
  //*******************************************************************
 if(lightVal>= 1000)
  {
    digitalWrite (ledPin, LOW); // turn off light
    digitalWrite (ledPin2, LOW);// turn off light
  }
//***************************************************************
  if(lightVal<370)
  {
    digitalWrite (ledPin, HIGH); // turn on light
    digitalWrite (ledPin2, HIGH); // turn on light
  }
}

void wifiReconect()
{
// Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }  
}
void ThingSpeakSendData()
{
  if (waitTime > (writingTimer*1000)) //Send dat to ThingSpeak every 20 sec.
  {
     // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  int x = ThingSpeak.writeField(myChannelNumber, 1, lightVal, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
   startTime = millis();    
    delay(5000);  
  }
  waitTime = millis()-startTime;    
  
}
