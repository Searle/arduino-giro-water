#include "LedControl.h"
#include "Particles.h"
#include "gyro.h"

/*
 Now we need a LedControl to work with.
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
LedControl lc = LedControl(12, 10, 11, 1);

/* we always wait a bit between updates of the display */
unsigned long delaytime = 100;

#define WithPr 1

#if (WithPr)
#define SerialBegin() Serial.begin(57600)
#define Pr Serial.print
#define Prln Serial.println
#else
#define SerialBegin()
#define Pr(s)
#define Prln(s)
#endif

void init_parts();

int loop_count = 0;

void setup()
{
  /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 5);
  /* and clear the display */
  lc.clearDisplay(0);

  randomSeed(analogRead(0));

  SerialBegin();
  particlesInit();
  // gyro_setup();
}

void update_particles()
{
  particlesLoop(0, 5);
}

void show_particles()
{
  byte v[64];
  particlesTo8x8(v);

  for (int y = 0, index = 0; y < 8; ++y)
  {
    byte b = 0;
    for (int x = 0; x < 8; ++x, ++index)
    {
      if (v[index]) { // > (loop_count & 3)) {
        b |= B10000000 >> x;
      }
    }
    lc.setRow(0, y, b);
  }

  ++loop_count;
}

inline boolean e(float f) {
  return f < -0.01 || f > 0.01;
}

void loop()
{
  static int count= 0;
  static long t1= 0;
  static long t2= 0;


/*
  long m = millis();

  if(t1 == 0) {
    t1= t2= m;
    gyro_update();
  }

  if( m - t1 > 50) {
    gyro_update();
    t1 = m;
  }
  if( m - t2 > 150) {
    Serial.print(gyro_get_dx());
    Serial.print( "\t");
    Serial.println(gyro_get_dy());
    t2 = m;
  }
*/
  update_particles();
  show_particles();
  
// delay(50);
  return;
  
  update_particles();
  show_particles();

if(0) {
  count++;
  Particle *p0= getParticle(0);
  Particle *p1= getParticle(1);
  if ( e(p0->dx) || e(p0->dy) || e(p0->vx) || e(p0->vy) || e(p1->dx) || e(p1->dy) || e(p1->vx) || e(p1->vy) ) {
  Pr(count);
  // Prln("================");
  for (int i=0; i < PARTICLE_COUNT; i++) {
    printParticle(getParticle(i));
  }
    Prln("");
  }
}
  
  ++loop_count;
}
