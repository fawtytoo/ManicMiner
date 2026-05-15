//  Copyright 2021-2026 by Steve Clark

//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.

//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:

//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required. 
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.

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
