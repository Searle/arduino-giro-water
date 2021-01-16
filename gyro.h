#ifndef gyro_h
#define gyro_h

#include <Arduino.h>

void gyro_setup();
void gyro_update();
int gyro_get_dx();
int gyro_get_dy();

#endif
