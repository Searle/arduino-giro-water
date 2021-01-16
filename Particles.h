#ifndef Particles_h
#define Particles_h

#include <Arduino.h>

#define PARTICLE_COUNT 30

typedef struct
{
    int id;
    float x;
    float y;
    float vx;
    float vy;
    float dx;
    float dy;
} Particle;

void particlesInit();
void particlesLoop(float gravity_x, float gravity_y);
void particlesTo8x8(byte *v);
Particle *getParticle(int index);
void printParticle(Particle *particle);

#endif
