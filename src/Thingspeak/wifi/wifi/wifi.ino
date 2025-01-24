#include <Arduino.h>
#include "WiFi.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

#define CHANNEL_ID 2228162
#define CHANNEL_API_KEY HN3T8FTN38HDMF5F

WiFiClient  client;
int counter=0;

#define WIFI_TIMEOUT_MS 20000
#define WIFI_NETWORK 3004
#define WIFI_PASSWORD 13956922663

void connectToWiFi(){
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

  unsigned long startAttemptTime = millis();

  while(WiFi.status()!=WL_CONNECTED && millis()-startAttemptTime<WIFI_TIMEOUT_MS){
    Serial.print(".");
    delay(100);
  }

  if(WiFi.status()!=WL_CONNECTED){
    Serial.println("Failed!");
  }else{
    Serial.print("Connected!");
    Serial.println(WiFi.localIP());
  }
}

void setup(){
  Serial.begin(9600);
  connectToWiFi();
  ThingSpeak.begin(client);
}

void loop(){
  counter++;

  ThingSpeak.setField(CHANNEL_ID, 1, counter, CHANNEL_API_KEY);

  delay(15000);
}

