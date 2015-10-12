#include <wiringPi.h>
#include <signal.h>
#include <stdlib.h>
#include <softPwm.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#include "signal433driver.h"

void exit_event(int signum);

int SOFT_PWM_PIN=0; //Software PWM Pin(GPIO0-11)

int main(void) {
  (void)signal (SIGINT,  exit_event);
  (void)signal (SIGQUIT, exit_event);
  int code = 0b110101011101;
  printf("Software based PWM test on LED\n");
  
  if(getuid()!=0) {
    printf("Error:wiringPi must be run as root.\n");
    return 1;
  }
  
  if(wiringPiSetup()==-1) {
    printf("Error:wiringPi setup failed.\n");
    return 1;
  }
  
  //signal433Create

  signal433Create(SOFT_PWM_PIN,code); //setup software pwm pin
  // int i = 0;
   
  
  while(1){
    /*
     for(i=0;i<100;i++){
      softPwmWrite(SOFT_PWM_PIN,i);
      delay(500);
    }
    */
  }
  return 0;
}

void exit_event(int signum) {
  printf("\b\bExiting...\n");
  digitalWrite(SOFT_PWM_PIN, LOW);
  delay(100); //wait a little for the pwm to finish write wiringPiSetup();
  pinMode(0, OUTPUT);
  exit(0);
}
