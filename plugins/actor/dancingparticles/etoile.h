#include <cmath>
#include <GL/glu.h>
#include <cstring>
#include <iostream>

#include "actor_dancingparticles.h"

#include "fastmath.h"
#include "floatpoint.h"

typedef FloatPoint FloatPoint;

#include "matrix.h"

/* Globals */

struct parameters
{
  char *name;
  char key;
  int mode;
  myfloat k;      // cst du ressort
  myfloat d0min;  // longueur min
  myfloat d1;     // longueur du cercle du centre
  myfloat ag;     // antigravité
  int antigorder;
  int agnumparts;
  myfloat maxantig;
  int noagexplosion;
  myfloat dancingpart; // in %
  int dancingpartk;
  myfloat velocity;
  int     numfrq;
  myfloat visc;
  myfloat rotspeed1;
  myfloat rotspeed2;
  int strombo;    // 1 = no strombo

  int numpart;
  myfloat size;
  myfloat sizeloudness;

  int chance;
  int duration_f; 
  int duration_b;
};

extern parameters p;


extern FloatPoint *pts;
extern FloatPoint *speed;
extern FloatPoint *Centers;
extern int         *newline;
extern FloatPoint *Centers2; // used to build the Centers  TODO need to be a better data struct for Centers
extern int         *newline2;
extern int numCenters;
extern int numCenters2;

extern FloatPoint Center;
extern int ptsNum, ptsNumMax;

extern int beat;
extern int beatcnt;
extern int thick, quiet;

extern parameters *ps;
extern int nump;
extern int frames;

/* These define the resolution and tonal scale of the spectrum analyzer */
#define NUM_BANDS       255
extern GLfloat heights[NUM_BANDS];


/* Prototypes */

void etoileLoop(void);

void	DrawPt(int i);	
void DrawCenter();
void BeginDrawing();
void EndDrawing();
Boolean eTestEnd();
FloatPoint eSetCenter(long time);
void Tourne(short i);
void Elastantig(short i,FloatPoint &c);

// parameters
void etoileinit();
void  init_parameters();
void changep();
void allocParts();

void loadepic(char *file);

class beatdetector
{
public:
  uint32_t filter[NUM_BANDS];
  uint32_t filter2[NUM_BANDS];
  uint32_t filterpower;      // sum of all the bands

  int lastbeats[10];
  int freq; // in CLK_TCK
  uint16_t variance;
  uint32_t loudness;
  int beat;

  uint32_t	avloudness;
  uint32_t	uplimit;
  uint32_t	downlimit;
  int           state;

  int	beatbase;
  int	beatquiet;	/* force "quiet" situation? */
  
  beatdetector();
  void calc_loudness(uint16_t data[2][256]);
  void detect_beat();
  void learnbeat(uint16_t data[2][256]);

};
extern beatdetector detector;

extern int forceEffectChange; 
extern int nextEffect; 
