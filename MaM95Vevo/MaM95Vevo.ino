// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $
#include <VirtualWire.h>
#include <SoftwareServo.h>

SoftwareServo servo1; //fel, le
SoftwareServo servo2; //fel, le
SoftwareServo servo3; //előre, hátra
SoftwareServo servo4; //előre, hátra

int delayDef = 2;

void setup()
{
    servo1.attach(3); 
    servo2.attach(5);
    servo3.attach(6);
    servo4.attach(9);
    
  
    Serial.begin(9600); // Debugging only
    Serial.println("vevo");
    // Initialise the IO and ISR
    vw_set_ptt_inverted(true); // Required for DR3100
    vw_setup(2000);      // Bits per sec
    vw_rx_start();       // Start the receiver PLL running
}

//szervo pozíció beállítás adott helyzetre
void setPosition(int servDeg1, int servDeg2, int servDeg3, int servDeg4){
    for (int i=0; i <= 200; i++){
      //fel/le
      servo1.write(servDeg1);
      servo2.write(servDeg2);  
      //előre/hátra
      servo3.write(servDeg3);  
      servo4.write(servDeg4);
      SoftwareServo::refresh();
      delay(delayDef);
    }
}


//előre, és hátra függvény
void elore(bool isElore){

    int maxDeg = 180;
    int minDeg = 0;
    if (isElore == false){
      maxDeg = 0;
      minDeg = 180;
    }

    //lent, elől
    setPosition(minDeg, maxDeg, minDeg, maxDeg);

    //lent hátra
    setPosition(minDeg, maxDeg, maxDeg, minDeg);
    
    //fel, hátra
    setPosition(maxDeg, minDeg, maxDeg, minDeg);

    //fel, előre
    setPosition(maxDeg, minDeg, minDeg, maxDeg);
}

void fordulas(int iranyEro, bool isElore){

    int kanyar = 135;
    if (iranyEro == 1 || iranyEro == 3){
      kanyar = 45;
    }
    
    int maxDeg = 180;
    int minDeg = 0;    
    if (iranyEro == 3 || iranyEro == 4){
      maxDeg = 0;
      minDeg = 180;
    }

    if (isElore == false){
      //ha hátra menet van akkor megfordítjuk
      if (maxDeg == 0){
        maxDeg = 180;
        minDeg = 0;
      }
    }

    //lent elől
    setPosition(minDeg, maxDeg, minDeg, maxDeg);

    //lent hátra
    if (iranyEro == 1 || iranyEro == 2){
      setPosition(minDeg, maxDeg, minDeg, kanyar);
    }else{
      setPosition(minDeg, maxDeg, kanyar, maxDeg);
    }
    
    //fel hátul
    if (iranyEro == 1 || iranyEro == 2){
      setPosition(maxDeg, minDeg, minDeg, kanyar);
    }else{
      setPosition(maxDeg, minDeg, kanyar, maxDeg);
    }
        
    //fel előre
    setPosition(maxDeg, minDeg, minDeg, maxDeg);    
}

void hatraFordulKanyar(bool isBalra, bool isElore){

    int maxDeg = 180;
    int minDeg = 0;
    if (isElore == false){
      maxDeg = 0;
      minDeg = 180;
    }

    //fent, lent // elől
    if (isBalra){
      setPosition(minDeg, minDeg, minDeg, maxDeg);
    }else{
      setPosition(maxDeg, maxDeg, minDeg, maxDeg);
    }

    // fent, lent // elől, hátul 
    if (isBalra){
      setPosition(minDeg, minDeg, minDeg, minDeg);
    }else{
      setPosition(maxDeg, maxDeg, maxDeg, maxDeg);
    }

    // lent // elől, hátul 
    if (isBalra){
      setPosition(minDeg, maxDeg, minDeg, minDeg);
    }else{
      setPosition(minDeg, maxDeg, maxDeg, maxDeg);
    }

    // lent // hátul, elől 
    if (isBalra){
      setPosition(minDeg, maxDeg, maxDeg, maxDeg);
    }else{
      setPosition(minDeg, maxDeg, minDeg, minDeg);
    }

  
}


void loop()
{

  
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(buf, &buflen)) // Non-blocking
    {
        int i;
        digitalWrite(13, true); // Flash a light to show received good message
        // Message with a good checksum received, dump it.
        Serial.print("Got: ");
        
        for (i = 0; i < buflen; i++)
        {
            //Serial.print(buf[i], HEX);
            Serial.print(buf[i]);

            //mozgás meghatározása
            switch (buf[i]) {
              //előre
              case 65: //A
                hatraFordulKanyar(true, true);
                break;
              case 66: //B
                fordulas(2, true);
                break;
              case 67: //C
                fordulas(1, true);
                break;
              case 68: //D
                elore(true);
                break;
              case 69: //E
                fordulas(3, true);
                break;
              case 70: //F
                fordulas(4, true);
                break;
              case 71: //G
                hatraFordulKanyar(false, true);
                break;

              //hátramenet
              case 97: //a
                hatraFordulKanyar(true, false);
                break;
              case 98: //b
                fordulas(2, false);
                break;
              case 99: //c
                fordulas(1, false);
                break;
              case 100: //d
                elore(false);
                break;
              case 101: //e
                fordulas(3, false);
                break;
              case 102: //f
                fordulas(4, false);
                break;
              case 103: //g
                hatraFordulKanyar(false, false);
                break;
              break;
            }

            
            Serial.print(" ");
        }
        Serial.println("");
        digitalWrite(13, false);
    }
}

