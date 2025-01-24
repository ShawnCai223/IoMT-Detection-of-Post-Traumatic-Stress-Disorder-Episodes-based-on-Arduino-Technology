#include <SoftwareSerial.h>

const int sensorRXPin = 2; // Connect sensor TX to Arduino pin 2
const int sensorTXPin = 3; // Connect sensor RX to Arduino pin 3
SoftwareSerial sen (sensorRXPin, sensorTXPin);
int data[87];
int HR=0;
int RR=0;
int BPL=0;
int BPH=0;

void setup() {
  Serial.begin(38400); // Set the baud rate to 9600 for communication with the computer
  sen.begin(38400); // Set the sensor baud rate to 9600

  for(int i=0;i<500;i++)
  {
    for(int j=0;j<88;j++)
    {
      data[j]=analogRead(sensorRXPin);
    }
    delay(5);
  }

  int sum=0;
  for(int x=80; x<86; x++)
  {
    sum=data[x];
  }
  RR=sum/6;

}

void loop() {
  Serial.print("HeartRate = "); Serial.print(data[65]); 
  Serial.print("*bpm\tRespirationRate = "); Serial.print(RR); Serial.println("bpm");
  Serial.print("BPL = "); Serial.print(data[72]); 
  Serial.print("*F\tBPH = "); Serial.print(data[71]); Serial.println("*F");

  Serial.println();
  delay(500);

  
  }

