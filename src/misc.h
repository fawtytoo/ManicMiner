#include "common.h"

#define TICKRATE    60

#define SAMPLERATE  22050

typedef struct
{
    int rate;
    int acc;
    int remainder;
    int divisor;
}
TIMER;

extern UINT videoPalette[16];

void Timer_Set(TIMER *, int, int);
int Timer_Update(TIMER *);
int Video_Viewport(int, int, int *, int *, int *, int *);

void Audio_Output(short [2]);

void System_Border(int);
void System_SetPixel(int, int);
int System_IsKeyLeft(void);
int System_IsKeyRight(void);
int System_IsKeyJump(void);
void System_UpdateKeys(void);
void System_LockAudio(void);
void System_UnlockAudio(void);
