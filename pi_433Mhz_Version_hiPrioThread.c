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

int main(void) {
  (void)signal (SIGINT,  exit_event);
  (void)signal (SIGQUIT, exit_event);
  char code[13] = "110101011101";
  int ret;
  printf("Software based 433Mhz test\n");
  
  if(getuid()!=0) {
    printf("Error:wiringPi must be run as root.\n");
    return 1;
  }
  
  if(wiringPiSetup()==-1) {
    printf("Error:wiringPi setup failed.\n");
    return 1;
  }
  
  //signal433Create

  signal433Create(DATA_433MHZ_PIN); //,code,DEFAULT_REPEATS);
  // int i = 0;
   
  ret = 1;
  while(ret){
      ret = signal433Send(code,12);
      printf("Return %d\r\n",ret);
  }
  
  signal433Stop();
  return 0;
}

void exit_event(int signum) {
  printf("\b\bExiting...\n");
  signal433Stop();
  exit(0);
}
