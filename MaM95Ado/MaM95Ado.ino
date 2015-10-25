// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $
#include <VirtualWire.h>

int analogPin = 1;
int analogVal = 0;                
int gombPin = 2;
bool iranyElore = true;
bool iranyCheck = false;
int buttonState = 0;         
 
void setup()
{
   
   
    //Serial.begin(9600);   // Debugging only
    //Serial.println("setup");
    // Initialise the IO and ISR
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);      // Bits per sec

    //gomb nyomás
    pinMode(gombPin, INPUT);
}
    
void loop()
{

  //meg van-e nyomva
  buttonState = digitalRead(gombPin);

  if (buttonState == 0){
    if (iranyCheck == false){
      iranyCheck = true;
      if (iranyElore == true){
        iranyElore = false; 
      }else{
        iranyElore = true;
      }
    }
  }

  if (buttonState == 1){
    iranyCheck = false;
  }

  char *msg = "0";

  //megnézzük, hogy hol áll a szervó
  analogVal = analogRead(analogPin);  

  analogVal = analogVal/4;

  //bal
  if (analogVal >= 0 && analogVal < 21 && buttonState == 1){
    //Serial.println("balra hatra");
    if (iranyElore){
      msg = "A";
    }else{
      msg = "a";
    }
  }
  if (analogVal >= 21 && analogVal < 42 && buttonState == 1){
    //Serial.println("balra nagyon");
    if (iranyElore){
      msg = "B";
    }else{
      msg = "b";
    }
  }
  if (analogVal >= 42 && analogVal < 60 && buttonState == 1){
    //Serial.println("balra kicsit");
    if (iranyElore){
      msg = "C";
    }else{
      msg = "c";
    }
  }
  
  //közép
  if (analogVal >= 60 && analogVal < 65 && buttonState == 1){
    //Serial.println("elore");
    if (iranyElore){
      msg = "D";
    }else{
      msg = "d";
    }
  }

  //jobb
  if (analogVal >= 65 && analogVal < 84 && buttonState == 1){
    //Serial.println("jobbra kicsit");
    if (iranyElore){
      msg = "E";
    }else{
      msg = "e";
    }
  }
  if (analogVal >= 84 && analogVal < 105 && buttonState == 1){
    //Serial.println("jobbra nagyon");
    if (iranyElore){
      msg = "F";
    }else{
      msg = "f";
    }
  }
  if (analogVal >= 105 && analogVal <= 127 && buttonState == 1){
    //Serial.println("jobbra hatra");
    if (iranyElore){
      msg = "G";
    }else{
      msg = "g";
    }
  }

  //Serial.println(analogVal);             
  
  digitalWrite(13, true); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13, false);
  delay(200);
}

