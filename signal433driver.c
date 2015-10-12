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

// #include "433mhzT.h"

#define	DATA_LENGHT	12

static int sendCode = 0;
static int repeatCode = 3;
static int newpin = -1;

/*
 * 433Mhz klik aan klik uit
 *	Thread to do the actual output
 *********************************************************************************
 */

static PI_THREAD(signal433Thread) {
  int ontime = 0;
  int offtime = 0;
  int pin, r, c;

  piHiPri(50);

  pin = newpin;
  newpin = -1;

  for(r = 0; r <repeatCode; r++) {
    for(c = 0; c<25; c++) {
      ontime = 290;
      offtime = 900;
      if(c%2==1) {
        if((sendCode>>(11-c/2))&0x01) {
          ontime = 900;
          offtime = 300;
        }
      }
      digitalWrite(pin, HIGH);
      delayMicroseconds(ontime);
      digitalWrite(pin, LOW);
      delayMicroseconds(offtime);
    }
    delayMicroseconds(1200);
  }
  return NULL ;
}


/*
 * softPwmCreate:
 *	Create a new PWM thread.
 *********************************************************************************
 */
int signal433Create(int pin, char *toSend, int repeats) {
  int res;
  pinMode      (pin, OUTPUT) ;
  digitalWrite (pin, LOW) ;

  sendCode = toSend;
  newpin = pin;
  repeatCode = repeats;

  res = piThreadCreate(signal433Thread) ;

  while (newpin != -1)
    delay (1) ;

  return res ;
}

