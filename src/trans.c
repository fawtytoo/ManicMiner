#include "common.h"
#include "video.h"
#include "game.h"
#include "audio.h"

static int      transLevel;

static void DoTransDrawer()
{
    if (gameDemo == 0)
    {
        Game_DrawAir();
        Game_ExtraLife();
    }

    if (transLevel > 0)
    {
        Video_LevelPaperFill(transLevel >> 3);
        Video_LevelInkFill(transLevel & 0x7);
    }
}

static void DoTransTicker()
{
    if (transLevel > 1)
    {
        transLevel--;
        return;
    }

    if (gameDemo == 0)
    {
        if (transLevel == 1)
        {
            transLevel--;
            Audio_Sfx(SFX_AIR);
        }

        if (gameAir > 8)
        {
            Game_ScoreAdd(8);
            Game_ReduceAir(8);
        }
        else if (gameAir > 0)
        {
            Game_ScoreAdd(gameAir);
            Game_ReduceAir(gameAir);
        }

        if (gameAirOld > 0)
        {
            return;
        }
    }

    if (gameLevel++ == TWENTY)
    {
        gameLevel = 0;
    }

    Action = Game_Action;
}

static void DoTransInit()
{
    transLevel = 63;

    if (gameDemo == 0)
    {
        Audio_Play(MUS_STOP);
    }

    Ticker = DoTransTicker;
}

// this is needed for demo mode
static void DoTransResponder()
{
    Action = Title_Action;
}

void Trans_Action()
{
    Responder = gameDemo ? DoTransResponder : DoNothing;
    Ticker = DoTransInit;
    Drawer = DoTransDrawer;

    Action = DoNothing;
}
