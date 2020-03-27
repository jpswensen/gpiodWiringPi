#include <stdio.h>
#include <gpiod.h>

#include "gpiodWiring.h"


#define NUM_PINS 27


// Global variable for keeping gpiod_chip object open for faster performance
struct gpiod_chip *chip = NULL;

// Global array for keeping gpiod_line objects open for faster performance
struct gpiod_line *lines[NUM_PINS] = {NULL};

// Global array for keeping track of whether a pin has been set up as output or input
int directions[NUM_PINS];

// Static global variable for initialization
static int initialized = 0;

// Defines for controlling which gpiod device gets opened and its consumer name
// NOTE: If you were trying to use this on a different gpiod compatible linux board,
//       you would want to change the GPIOD_CHIP_NAME to your chip from gpiodetect
#define GPIOD_CHIP_NAME "gpiochip0"
#define GPIOD_CONSUMER_NAME "gpiodWiringPi"


void initialize()
{
  chip = gpiod_chip_open_by_name(GPIOD_CHIP_NAME);
  if (!chip) {
    printf("Unable to open chip: %s\n", GPIOD_CHIP_NAME);
    exit(1);
  }

  for (int pin = 0 ; pin < NUM_PINS ; pin++)
    directions[pin] = UNDEFINED;
  
  initialized = 1;
}

void pinMode(int pin, int dir)
{
  if (!initialized)
    initialize();

  // If the line hasn't been reserved yet, then reserve it
  if (!lines[pin]) {
    lines[pin] = gpiod_chip_get_line(chip, pin);
    if (!lines[pin]) {
      printf("Unable to open line: %d\n", pin);
      return;
    }
  }

  
  if (dir == OUTPUT) {
    if (gpiod_line_request_output(lines[pin], GPIOD_CONSUMER_NAME, 0) != 0) {
      printf("Unable to set line %d to output\n", pin);
      return;
    }
    directions[pin] = OUTPUT; 
  }
  else {
    if (gpiod_line_request_input(lines[pin], GPIOD_CONSUMER_NAME) != 0) {
      printf("Unable to set line %d to input\n", pin);
      return;
    }
    directions[pin] = INPUT;
  }

}

void digitalWrite(int pin, int value)
{
  if (!initialized)
    initialize();
  
  if (directions[pin] != OUTPUT) {
    return;
  }

  if (gpiod_line_set_value(lines[pin], value) != 0) {
    printf("Unable to set line value for pin %d\n", pin);
    return;
  }
}

int digitalRead(int pin) {
  if (!initialized)
    initialize();
  
  if (directions[pin] != INPUT) {
    return -1;
  }

  int retval = gpiod_line_get_value(lines[pin]);
  if (retval == -1) {
    printf("Unable to read line value for pin %d\n",pin);
  }
  
  return retval;
}


