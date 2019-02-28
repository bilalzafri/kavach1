#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"

VR myVR(2,3);    // 2:RX 3:TX, you can choose your favourite pins.
uint8_t records[7]; // save record
uint8_t buf[64];
int motor = 13;
int buttonPin = 2; 
int buttonState = 0;
SoftwareSerial BTSerial(10, 11);

#define aRecord   (0)
#define bRecord   (1)
#define cRecord   (2)
#define dRecord   (3)
#define eRecord   (4)

void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}
void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
  myVR.begin(9600); 
  Serial.begin(115200);
  BTSerial.begin(38400);
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  pinMode(motor, OUTPUT);
  pinMode(buttonPin, INPUT);
    
  if(myVR.clear() == 0)
  {
    Serial.println("Recognizer cleared.");
  }
  
  else
  {
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
}

void loop()
{
  int ret,x;
  ret = myVR.recognize(buf, 50);
  if(ret>0)
  {
    switch(buf[1])
    {
      case aRecord:
      case bRecord:
      case cRecord:
      case dRecord:
      case eRecord:
         x = 1;
         break;
      default:
         x = 0;
         break;
    }
    /** voice recognized */
    printVR(buf);
  }

  if(x == 1)
  {
     buttonState = digitalRead(buttonPin);
     digitalWrite(motor, HIGH);
     delay(2000);
     digitalWrite(motor, LOW);
     if (buttonState == HIGH) 
      {
         delay(7000);
         if (buttonState == HIGH) 
         {
           BTSerial.write('A');
         }
      }
      
      else  
      {
         delay(7000);
         if (buttonState == LOW) 
         {
           BTSerial.write('A');
         }
      }
  }
}
