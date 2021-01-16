#ifndef Particles_h
#define Particles_h

#include <Arduino.h>

void particlesInit();
void particlesLoop(float gravity_x, float gravity_y);
void particlesTo8x8(byte *v);

#endif
