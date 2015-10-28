#ifndef SIGNAL433DRIVER_H
#define SIGNAL433DRIVER_H 1

#define	MAX_DATA_LENGHT	12
#define DEFAULT_REPEATS 3
#define DATA_433MHZ_PIN 0
//Software PWM Pin(GPIO0-11)

extern int signal433Create(int pin);
extern int  __attribute__ ((hot)) signal433Send(sendData_t toSend);
extern void signal433Stop();


#endif