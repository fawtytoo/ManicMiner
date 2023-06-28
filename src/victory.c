#include "common.h"
#include "video.h"
#include "game.h"
#include "audio.h"
#include "system.h"

int victoryTimer;

void DoVictoryTicker()
{
    if (victoryTimer-- == 0)
    {
        Action = Trans_Action;
    }
}

void DoVictoryInit()
{
    System_Border(0);

    Portal_Init();
    Level_Drawer();
    Robots_Drawer();
    Portal_Drawer();

    Video_SpriteBlend(24 * WIDTH + 19 * 8, minerSprite[4], 0x7);

    victoryTimer = 50 * 9;

    Audio_Play(MUS_STOP);
    Audio_Sfx(SFX_VICTORY);

    Ticker = DoVictoryTicker;
}

void Victory_Action()
{
    Responder = DoNothing;
    Ticker = DoVictoryInit;
    Drawer = DoNothing;

    Action = DoNothing;
}
