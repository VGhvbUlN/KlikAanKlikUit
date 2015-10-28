/*
 *
 ***********************************************************************
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <wiringPi.h>
#include "signal433driver.h"

static int theSendCode;
static int lengthOfCode;
static int numberOfRepeats = -1; // DEFAULT_REPEATS;
static int pin = 0;
static int running = 0;

/*
 * actual output
 *********************************************************************************
 */
void  __attribute__ ((hot)) sendChip(int pin, int chip) {
    if(chip) { // chip 1 
        digitalWrite(pin, HIGH);
        delayMicroseconds(900);
        digitalWrite(pin, LOW);
        delayMicroseconds(300);
    } else { // chip 0
        digitalWrite(pin, HIGH);
        delayMicroseconds(290);
        digitalWrite(pin, LOW);
        delayMicroseconds(900);
    }
}

/*
 * 433Mhz klik aan klik uit
 *	Thread to do the actual output
 *********************************************************************************
 */ 
static  __attribute__ ((hot)) PI_THREAD(signal433Thread) {
    signed int sendBit; // must be sure it is signed for the for loop to stop.
    int localPin;
    int localSendCode;
    int localLengthOfCode = 0;
    
    localPin = pin;
    pin = -1;
    numberOfRepeats = -1
    
    piHiPri(150); // wiringPi HiPri
    
    while(running) {
        if(localNumberOfRepeats >= 0) { 
            /* Start Sending */
            sendChip(localPin, 0);
            for(sendBit = localLengthOfCode-1; sendBit >= 0; sendBit--) {
                if((theSendCode >> sendBit)&0x01) {
                    sendChip(localPin, 1);
                } else {
                    sendChip(localPin, 0);
                }
                sendChip(localPin, 0);
            }
            /* End sending */
            localNumberOfRepeats--;
        } else {
            if(numberOfRepeats>=0){
                if(lengthOfCode<MAX_DATA_LENGHT) {
                    localSendCode = theSendCode;
                    localLengthOfCode = lengthOfCode;
                    localNumberOfRepeats = numberOfRepeats;
                    numberOfRepeats = -1;
                } else {
                    numberOfRepeats = -2;
                }
            }
        }
        delayMicroseconds(1200);
    }
    digitalWrite(localPin, LOW);
    running = -1;
    return NULL;
}


/*
 *
 */
int signal433Create(int newpin){
  int res;
  if(running) {
    return -1;
  } 
  running = 1;
  pin = newpin;
  
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  
  res = piThreadCreate(signal433Thread);

  while(pin != -1)
    delay(1);
  return res ;
}

void signal433Stop() {
  running = 0;
  while(running != -1)
    delay(1);
}







int __attribute__ ((hot)) signal433Send(char *toSend, int length) {
    int countWildCard, countSendBit; // counters
    int numberOfSends; /* == square 2^numberOfWildcards == 1 << numberOfWildcards */
    int wildCardShift;
            
    if(length>MAX_DATA_LENGHT) {
        return -1;
    }
    if(running) {
        if(numberOfRepeats>=0) {
            return -2;
        }
        /* check the send code and find wildcards */
        numberOfSends = 1;
        for(countSendBit=0; countSendBit < length; countSendBit++){
            if((toSend[countSendBit]!='0')&&(toSend[countSendBit]!='1')&&(toSend[countSendBit]!='.')){
                return 0;
            }
            if(toSend[countSendBit]=='.'){
                numberOfSends = numberOfSends<<1;
            } 
        }
        
        for(countWildCard=0; countWildCard < numberOfSends; countWildCard++) {
            theSendCode = 0;
            wildCardShift = 0;
            for(countSendBit=0; countSendBit < length; countSendBit++){
                theSendCode = theSendCode << 1;
                if(toSend[countSendBit]=='1') {
                    theSendCode |= 1;
                } else if(toSend[countSendBit]=='.') {
                    theSendCode |= (countWildCard>>wildCardShift)&1;
                    wildCardShift++;
                }
            }
            // send!
            // theSendCode;
            lengthOfCode = length;
            numberOfRepeats = DEFAULT_REPEATS;
            // waith until sendcode accepted.
            while(numberOfRepeats>=0) {
                delay(6);
            }
        }
        
        return 1;
    }
    return -3;
}