# gpiodWiringPi
A quick and dirty replication of the Arduino "wiring" GPIO syntax for RPI

Not much more to say. It provides the following functions:

* void pinMode(int pin, int direction)
* void digitalWrite(int pin, int value)
* int digitalRead(int pin)

It uses the fairly recent gpiod and libgpiod functionality of linux under the hood. It should be as simple as including gpiodWiring.h and gpioWiring.c into your project.


## Examples

### gpio.c
This one just uses a simple for loop to toggle the pin high and low at 1 millisecond intervals

To compile:

  gcc -I.. gpio.c ../gpiodWiring.c -o gpio -lgpiod
  
To run:

  ./gpio
  
### rt_gpio.c
I created this one to use high priority threads (I am using a RT_PREEMPT kernel to get the best possible GPIO timing on an RPI4 for a stepper motor controller). My anecdotal testing with an oscilloscope shows that I can get about 25 microsecond accuracy with this technique (with the caveat that nothing else is running on the device).

To compile:
  
  gcc -I.. rt_gpio.c ../gpiodWiring.c -o rt_gpio -lgpiod -lpthread
  
To run (NOTE: this has to be run as sudo because I couldn't figure out how to get the high priority threads without it):

  sudo ./rt_gpio
