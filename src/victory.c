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

    Level_Drawer();
    Robots_Drawer();
    Portal_SwordFish();

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
