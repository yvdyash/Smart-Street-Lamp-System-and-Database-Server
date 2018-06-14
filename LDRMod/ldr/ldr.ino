// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "printf.h"
#include<string.h>

/*const char text1[] = "auto";
const char text2[] = "manual";
char text3[10];
int k = 0, j = 0;*/


#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
#define Serial SerialUSB
#endif

int lightLevel=A2;

/*int auto_manual = 4;
int SW1 = 5;
int SW2 = 6;
int i;*/

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "ABCDE";


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  printf_begin();
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.printDetails();
  radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:

  char LL[32]="00000000000000000000000000000000";
  int i=3;
  lightLevel = analogRead(A2);
  while(lightLevel)
  {
    LL[i]=(lightLevel%10)+48;
    lightLevel/=10;
    i--;
  }
  
  
  
  radio.write(&LL, sizeof(LL));
  Serial.println(LL);
    delay(1000);
}
