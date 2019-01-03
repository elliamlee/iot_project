#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
 
#define HIGH 1
#define LOW 0
#define INPUT 1
#define OUTPUT 0

int outputPin = 28;

bool digitalPinMode(int pin, int dir){
  FILE * fd;
  char fName[128];
  
  // Exporting the pin to be used
  if(( fd = fopen("/sys/class/gpio/export", "w")) == NULL) {
    printf("Error: unable to export pin\n");
    return false;
  }
  fprintf(fd, "%d\n", pin);
  fclose(fd);

  // Setting direction of the pin
  sprintf(fName, "/sys/class/gpio/gpio%d/direction", pin);
  if((fd = fopen(fName, "w")) == NULL) {
    printf("Error: can't open pin direction\n");
    return false;
  }
  if(dir == OUTPUT) {
    fprintf(fd, "out\n");
  } else {
    fprintf(fd, "in\n");
  }
  fclose(fd);

  return true;
}

bool digitalWrite(int pin, int val) {
  FILE * fd;
  char fName[128];

  // Open pin value file
  sprintf(fName, "/sys/class/gpio/gpio%d/value", pin);
  if((fd = fopen(fName, "w")) == NULL) {
    printf("Error: can't open pin value\n");
    return false;
  }
  if(val == HIGH) {
    fprintf(fd, "1\n");
  } else {
    fprintf(fd, "0\n");
  }
  fclose(fd);

  return true;
}

int setup() {
   if (!digitalPinMode(outputPin, OUTPUT))
     return -1;
   return 0;
}

int main(void) {
  if (setup() == -1)
  {
    exit(1);
  }
	
  while(1){
    digitalWrite(outputPin, HIGH);
    sleep(1);
    digitalWrite(outputPin, LOW);
    sleep(1);
  }

  return 0;
}
