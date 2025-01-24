#include <Wire.h>
#include <Adafruit_MLX90614.h>
#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>
#include "WiFiEsp.h"
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros    


Adafruit_MLX90614 mlx = Adafruit_MLX90614();

char ssid[] = "3004c";   // your network SSID (name) 
char pass[] = "13956922663";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiEspClient  client;

const int LED=13;
const int GSR=A2;
int sensorValue;
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED_D = LED_BUILTIN;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.

PulseSensorPlayground pulseSensor; 
// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#define ESP_BAUDRATE  9600
#else
#define ESP_BAUDRATE  115200
#endif

unsigned long myChannelNumber = 2228162;
const char * myWriteAPIKey = "197I39ZS9N0J9B8S";

// Initialize our values
String myStatus = "";

void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  delay(1000);
  Serial.println("TEST START");  

  mlx.begin();  
    long sum=0;
  Serial.begin(9600);
  pinMode(LED_D,OUTPUT);
  digitalWrite(LED_D,LOW);
  delay(1000);
  
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold);   


  //Initialize serial and wait for port to open
  Serial.begin(9600);  // Initialize serial
 
  while(!Serial){
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  // initialize serial for ESP module  
  setEspBaudRate(ESP_BAUDRATE);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }

  Serial.print("Searching for ESP8266..."); 
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  Serial.println("found it!");
   
  ThingSpeak.begin(client);  // Initialize ThingSpeak

}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");   
  }

  // set the fields with the values
  int temperature=(mlx.readAmbientTempC());
  ThingSpeak.setField(1, temperature);
  int myBPM = pulseSensor.getBeatsPerMinute();  
  ThingSpeak.setField(2, myBPM);
  sensorValue=analogRead(GSR);
  ThingSpeak.setField(3, sensorValue);
  int resp=myBPM/4;
  ThingSpeak.setField(4, resp);

  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  

    Serial.print("Temp: ");
    Serial.print(mlx.readAmbientTempC());
    Serial.println(); 
    Serial.print("BPM: ");                       
    Serial.println(myBPM);                        
    Serial.print("SCL: ");
    Serial.println(sensorValue);
    Serial.print("RES: ");                       
    Serial.println(resp);
    Serial.println();           
  


  
  delay(500); // Wait 20 seconds to update the channel again
}

// This function attempts to set the ESP8266 baudrate. Boards with additional hardware serial ports
// can use 115200, otherwise software serial is limited to 19200.
void setEspBaudRate(unsigned long baudrate){
  long rates[6] = {115200,74880,57600,38400,19200,9600};

  Serial.print("Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");

  for(int i = 0; i < 6; i++){
    Serial1.begin(rates[i]);
    delay(100);
    Serial1.print("AT+UART_DEF=");
    Serial1.print(baudrate);
    Serial1.print(",8,1,0,0\r\n");
    delay(100);  
  }
    
  Serial1.begin(baudrate);
}
