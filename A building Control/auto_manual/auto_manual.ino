// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include<string.h>

const char text1[] = "auto";
const char text2[] = "manual";
char text3[10];
int k = 0, j = 0;


#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
#define Serial SerialUSB
#endif


int auto_manual = 4;
int SW1 = 5;
int SW2 = 6;
int i;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "ABCDE";




void setup ()
{

  Serial.begin(9600);
  printf_begin();
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.printDetails();
  radio.stopListening();

  pinMode(auto_manual, INPUT);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
}




void loop ()
{

  if (digitalRead(auto_manual) == 1)
  { //auto
    for (i = 0; i < 4; i++)
    {
      text3[i] = text1[i];
    }
    Serial.println(text3);

    radio.write(&text3, sizeof(text3));
    delay(1000);

    while (digitalRead(auto_manual) == 1);


  }



  else
  { //manual

    for (i = 0; i < 6; i++)
    {
      text3[i] = text2[i];
    }

    text3[i] = ',';
    i++;

    if (digitalRead(SW1) == 1)
    {
      text3[i] = '1';
      //delay(1000);
    }

    else
    {
      text3[i] = '0';

    }
    i++;

    text3[i] = ',';
    i++;

    if (digitalRead(SW2) == 1)
    {
      text3[i] = '1';
      // delay(1000);
    }
    else
    {
      text3[i] = '0';
    }
    //i=0;
    Serial.println(text3);

    //Serial.println(text3[7]);
    //Serial.println(text3[9]);

    do
    {
      //Serial.println(digitalRead(SW1));
      //Serial.println(text3[7]);

      if((digitalRead(SW1) == text3[7]))
      {}
      else
      break;
      if((digitalRead(SW2) == text3[9]))
      {}
      else
      break;

      
      /*((digitalRead(SW1) == text3[7])  (digitalRead(SW2) == text3[9]));
      delay(500);
      break;*/
    }
    while (digitalRead(auto_manual) == 0);


    radio.write(&text3, sizeof(text3));
    delay(1000);

    //for clearing the text3 string...............
    for(i=0;i<10;i++)
      {
      text3[i]=' ';
      }


  }

}




