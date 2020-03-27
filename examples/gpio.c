#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "gpiodWiring.h"

#define PIN_OUT 2
#define PIN_IN 3

int main(int argc, char*argv[])
{
  pinMode(PIN_OUT,OUTPUT);
  pinMode(PIN_IN,INPUT);

  for(int i = 0 ; i < 5000 ; i++) {
    digitalWrite(PIN_OUT,1);
    usleep(1000);
    digitalWrite(PIN_OUT,0);
    usleep(1000);

    int val = digitalRead(PIN_IN);
    printf("Input pin value: %d\n", val);
  }
}
