# gpiodWiringPi
A quick and dirty replication of the Arduino "wiring" GPIO syntax for RPI

Not much more to say. It provides the following functions:
  void pinMode(int pin, int direction)
  void digitalWrite(int pin, int value)
  int digitalRead(int pin)

It uses the fairly recent gpiod and libgpiod functionality of linux under the hood. It should be as simple as including gpiodWiring.h and gpioWiring.c into your project.
