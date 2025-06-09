#include "common.h"

#define TICKRATE    60

#define SAMPLERATE  22050

// palette ---------------------------------------------------------------------
typedef struct
{
    u8      r;
    u8      g;
    u8      b;
    u8      padding;    // unused
}
COLOUR;

extern const COLOUR     videoColour[16];

// timer -----------------------------------------------------------------------
typedef struct
{
    int rate;
    int acc;
    int remainder;
    int divisor;
}
TIMER;

void Timer_Set(TIMER *, int, int);
int Timer_Update(TIMER *);

// external --------------------------------------------------------------------
int Video_Viewport(int, int, int *, int *, int *, int *);

void System_Border(int);
void System_SetPixel(int, int);
int System_IsKeyLeft(void);
int System_IsKeyRight(void);
int System_IsKeyJump(void);
void System_UpdateKeys(void);
