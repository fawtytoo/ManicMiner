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

static u16      dieBlank[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static int      dieLevel;

static void DoDieDrawer()
{
    Video_LevelInkFill(dieLevel >> 1);

    Game_ExtraLife();
    Game_DrawAir(); // allows the air bar to catch up if necessary
}

static void DoDieTicker()
{
    if (dieLevel-- > 0)
    {
        return;
    }

    gameLives--;

    if (gameLives == 0)
    {
        Action = Gameover_Action;
        return;
    }

    if (gameLives < 10)
    {
        Video_Sprite(LIVES + (gameLives - 1) * 16, dieBlank, 0x0, 0x0); // blank frame
    }

    Action = Game_Action;
}

static void DoDieInit()
{
    dieLevel = 15; // make each colour last 2 frames

    Video_LevelPaperFill(0x0);
    System_Border(0x0);
    Audio_Sfx(SFX_DIE);

    Ticker = DoDieTicker;
}

void Die_Action()
{
    Responder = DoNothing;
    Ticker = DoDieInit;
    Drawer = DoDieDrawer;

    Action = DoNothing;
}
