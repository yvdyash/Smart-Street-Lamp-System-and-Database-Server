// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"

  const char text1[] = "auto";
  const char text2[] = "manual";
  char text3[10];

  
#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif




int auto_manual=4;
int SW1=5;
int SW2=6;
int i;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "ABCDE";




void setup () 
{


  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.printDetails();
  radio.stopListening();

  pinMode(auto_manual,INPUT);
  pinMode(SW1,INPUT);
  pinMode(SW2,INPUT);
  }
  



void loop () 
{
  
  if(digitalRead(auto_manual)==1)
  {
  for(i=0;i<6;i++)
  {
    text3[i]=text1[i];
  }
  i=0;
    
  radio.write(&text3, sizeof(text3));
  delay(1000);
  }
 
  else
  {

     for(i=0;i<6;i++)
  {
    text3[i]=text2[i];
  }
    
    text3[i]=',';
    i++;
    
    if(digitalRead(SW1)==1)
    {
      text3[i]='1';
      i++;
      //delay(1000);
      }
      
    else
    {
      text3[i]='0';
      i++;
    }
      
      text3[i]=',';
      i++;
      
      if(digitalRead(SW2)==1)
    {
      text3[i]='1';
      delay(1000);
    }
    else
      {
         text3[i]='0';
      }
  i=0;

  radio.write(&text3, sizeof(text3));
  delay(1000);

     
  }
   
}



