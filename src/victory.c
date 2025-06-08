#include "misc.h"
#include "video.h"
#include "game.h"
#include "audio.h"

static u16      minerSprite[16] = {24, 248, 496, 208, 248, 240, 96, 240, 504, 1020, 2046, 1782, 248, 474, 782, 900};

static int      victoryTimer;

static void DoVictoryTicker()
{
    if (victoryTimer-- == 0)
    {
        Action = Trans_Action;
    }
}

static void DoVictoryInit()
{
    System_Border(0);

    Portal_Init();
    Level_Drawer();
    Robots_Drawer();
    Portal_Drawer();

    Video_SpriteBlend(24 * WIDTH + 19 * 8, minerSprite, 0x7);

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
