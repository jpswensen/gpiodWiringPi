#ifndef __WIRING_H__
#define __WIRING_H__

#define UNDEFINED -1
#define INPUT 0
#define OUTPUT 1

void pinMode(int pin, int dir);
int digitalRead(int pin);
void digitalWrite(int pin, int value);

#endif // __WIRING_H__
