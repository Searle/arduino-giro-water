// From https://www.script-tutorials.com/particles-in-water/

#include "Particles.h"

#define GRID_CELL_SIZE 71
#define GRID_CELL_SIZE_HALF (GRID_CELL_SIZE / 2)
#define CANVAS_WIDTH 400
#define CANVAS_HEIGHT 400
#define GRID_WIDTH ((CANVAS_WIDTH + GRID_CELL_SIZE - 1) / GRID_CELL_SIZE)
#define GRID_HEIGHT ((CANVAS_HEIGHT + GRID_CELL_SIZE - 1) / GRID_CELL_SIZE)
#define MAX_NEIGHBORS 3
#define NEIGHBORS_SIZE (GRID_WIDTH * GRID_HEIGHT * MAX_NEIGHBORS)
#define DAMPENING 0.1

Particle particles[PARTICLE_COUNT];
byte neighbors[NEIGHBORS_SIZE];

void initParticle(Particle *particle, int id)
{
    particle->id = id;
    particle->x = CANVAS_WIDTH / 6 + random(CANVAS_WIDTH * 4 / 6);
    particle->y = CANVAS_HEIGHT / 6 + random(CANVAS_HEIGHT * 4 / 6);
    particle->vx = 0;
    particle->vy = 0;
    particle->dx = 0;
    particle->dy = 0;
};

Particle *getParticle(int index)
{
    return particles + index;
}

void printParticle(Particle *particle)
{
    Serial.print("[");
    Serial.print(particle->id);
    Serial.print(": ");
    Serial.print(particle->x);
    Serial.print(",");
    Serial.print(particle->y);
    Serial.print(" ");
    Serial.print(particle->vx);
    Serial.print(",");
    Serial.print(particle->vy);
    Serial.print(" ");
    Serial.print(particle->dx);
    Serial.print(",");
    Serial.print(particle->dy);
    Serial.print("]");
}

void moveParticle(Particle *particle)
{
    particle->x += particle->dx;
    particle->y += particle->dy;
    particle->vx += particle->dx;
    particle->vy += particle->dy;
    particle->dx = 0;
    particle->dy = 0;
};

void particlePhysics(Particle *particle, float gravity_x, float gravity_y)
{
    // gravity and acceleration
    particle->vx += gravity_x;
    particle->vy += gravity_y;
    particle->x += particle->vx;
    particle->y += particle->vy;

    // screens limits
    if (particle->x < GRID_CELL_SIZE_HALF)
    {
        particle->dx += GRID_CELL_SIZE_HALF - particle->x;
        particle->vx= 0;
    }
    else if (particle->x > CANVAS_WIDTH - GRID_CELL_SIZE_HALF)
    {
        particle->dx -= particle->x - CANVAS_WIDTH + GRID_CELL_SIZE_HALF;
        particle->vx= 0;
    }
    if (particle->y < GRID_CELL_SIZE_HALF)
    {
        particle->dy += GRID_CELL_SIZE_HALF - particle->y;
        particle->vy= 0;
    }
    else if (particle->y > CANVAS_HEIGHT - GRID_CELL_SIZE_HALF)
    {
        particle->dy -= particle->y - CANVAS_HEIGHT + GRID_CELL_SIZE_HALF;
        particle->vy= 0;
    }

    // grid coordinates
    int gx = round(particle->x / GRID_CELL_SIZE);
    int gy = round(particle->y / GRID_CELL_SIZE);

    // neighbors constraints
    for (int ix = gx - 1; ix <= gx + 1; ++ix)
    {
        for (int iy = gy - 1; iy <= gy + 1; ++iy)
        {
            if (ix >= 0 && ix < GRID_WIDTH && iy >= 0 && iy < GRID_HEIGHT)
            {
                int index = (iy * GRID_WIDTH + ix) * MAX_NEIGHBORS;
                for (int i = 0; i < MAX_NEIGHBORS; ++i, ++index)
                {
                    int n_index = neighbors[index];
                    if (n_index >= 0)
                    {
                        Particle *that = particles + n_index;
                        float dx = that->x - particle->x;
                        float dy = that->y - particle->y;
                        float d = sqrt(dx * dx + dy * dy);
                        if (d < GRID_CELL_SIZE && d > 0)
                        {
                            dx = (dx / d) * (GRID_CELL_SIZE - d) * DAMPENING;
                            dy = (dy / d) * (GRID_CELL_SIZE - d) * DAMPENING;
                            particle->dx -= dx;
                            particle->dy -= dy;
                            that->dx += dx;
                            that->dy += dy;
                        }
                    }
                }
            }
        }
    }

    // update neighbors array
    if (gy * GRID_WIDTH + gx < GRID_WIDTH * GRID_HEIGHT)
    {
        int index = (gy * GRID_WIDTH + gx) * MAX_NEIGHBORS;
        for (int i = MAX_NEIGHBORS - 1; i > 0; --i)
        {
            neighbors[index + i] = neighbors[index + i - 1];
        }
        neighbors[index] = particle->id;
    }
};

void particlesInit()
{
    for (int i = 0; i < PARTICLE_COUNT; i++)
        initParticle(particles + i, i);
}

void particlesLoop(float gravity_x, float gravity_y)
{
    for (int i = 0; i < NEIGHBORS_SIZE; ++i)
        neighbors[i] = -1;
    for (int i = 0; i < PARTICLE_COUNT; ++i)
        particlePhysics(particles + i, gravity_x, gravity_y);
    for (int i = 0; i < PARTICLE_COUNT; ++i)
        moveParticle(particles + i);
};

void particlesTo8x8(byte *v)
{
    for (int i = 0; i < 64; i++)
        v[i] = 0;
    for (int i = 0; i < PARTICLE_COUNT; i++)
    {
        Particle *particle = particles + i;
        if (particle->x >= 0 && particle->x < CANVAS_WIDTH && particle->y >= 0 && particle->y < CANVAS_HEIGHT)
        {
            int x = (particle->x * 8) / CANVAS_WIDTH;
            int y = (particle->y * 8) / CANVAS_HEIGHT;
            int index = y * 8 + x;
            v[index]++;
        }
    }
};
