#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include <string.h>
#include <Wire.h>
#include "RTClib.h"

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
#define Serial SerialUSB
#endif

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int lightLevel,templol=1;
int lightLevel_temp = 0;
int RELAY = 2;


const char text1[] = "auto";
const char text2[] = "manual";
int state;

RF24 radio(7, 8);

const byte rxAddr[6] = "ABCDE";

void setup()
{
  pinMode(RELAY, OUTPUT);
  //RTC setup
  //Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    
  }
  else
  rtc.adjust(DateTime(2018 ,3, 22, 18 , 59, 0));
  while (!Serial);
  Serial.begin(9600);
  radio.begin();
  printf_begin();
  radio.printDetails();
  radio.openReadingPipe(0, rxAddr);

  radio.startListening();

  radio.printDetails();
}



void loop()
{



  DateTime now = rtc.now();
  /*Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  Serial.println();*/
  delay(1000);




  char text[32] ;
  if (radio.available())
  {
    radio.read(&text, sizeof(text));
   
    if(text[0]=='a')
    templol=1;
    else if (text[0]=='m')
    templol=2;
    if(text[0]!='0' and text[0]!='1')
    Serial.println(text);

  }

  
  /*if (strcmp(text, text1) == 0)
  {
      if (radio.available())
      {
        radio.read(&text, sizeof(text));
        Serial.println(text);

        if (text[0] == '1' or text[0] == '0')
        {
          //Serial.println("kahbf");
          lightLevel = 1000 * (text[0] - 48) + 100 * (text[1] - 48) + 10 * (text[2] - 48) + (text[3] - 48);
          automatic(lightLevel, now);
        }
      }
      

      //radio.write('1',1);
    
  }
  else if (strcmp(text, text2) == 0)
  {
    manual(text[7], text[9]);
    //radio.write('0',1);
  }*/
  if(text[0]=='1' or text[0]=='0')
  {
    if(templol==1)
    {
      lightLevel = 1000 * (text[0] - 48) + 100 * (text[1] - 48) + 10 * (text[2] - 48) + (text[3] - 48);
      automatic(lightLevel, now);
    }
    else if(templol==2)
    {
      manual(text[7], text[9]);
    }
    
  }
  else if(templol==2)
  manual(text[7], text[9]);


}

void automatic(int lightLevel, DateTime now)
{


  //lightLevel = analogRead(A2);
  Serial.println("Calling Auto, LDR value: ");
  Serial.println(lightLevel, DEC);
  delay(200);
  if (now.hour() >= 17 && now.hour() <= 19)
  {
    if (abs(lightLevel_temp - lightLevel) < 200)
    {
      if (lightLevel > 650)
      {

        digitalWrite(RELAY, HIGH);
        //delay(60000);
        //flag==1;

      }
      else if (lightLevel < 460)
        digitalWrite(RELAY, LOW);
    }
  }
  else if (now.hour() >= 5 && now.hour() < 7)
  {
    if (abs(lightLevel_temp - lightLevel) < 200)
    {
      if (lightLevel < 460)
      {
        digitalWrite(RELAY, LOW);
        //delay(60000);
      }
      else if (lightLevel > 640)
      {
        digitalWrite(RELAY, HIGH);
        //delay(60000);
      }
    }

  }
  else if (now.hour() > 19 || now.hour() <= 5)
  {
    digitalWrite(RELAY, HIGH);
    // delay(1000);
  }
  else
  {
    digitalWrite(RELAY, LOW);
    // delay(1000);
  }
  if (abs(lightLevel_temp - lightLevel) < 200)
    lightLevel_temp = lightLevel;
  delay(1000);


}


void manual(char a, char b)
{
  Serial.println("Calling Manual: ");
  if (a == '1')
  {
    digitalWrite(RELAY, HIGH);
  }
  else
  {
    digitalWrite(RELAY, LOW);
  }

  if (b == '1')
  {
    digitalWrite(RELAY, HIGH);
  }
  else
  {
    digitalWrite(RELAY, LOW);
  }


}
