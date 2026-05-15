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
#include "game.h"

static char     *cheatCode = "6031769";
static int      cheatPos = 0;

int             cheatEnabled = 0;
EVENT           Cheat_Responder = DoCheatDisabled;

void DoCheatEnabled()
{
    int     level = 0;
    int     i;

    for (i = 0; i < 10; i++)
    {
        if (System_IsKey(KEY_0 + i))
        {
            level = i + 1;
            break;
        }
    }

    if (!System_IsKey(KEY_ENTER))
    {
        Game_Pause(0);
        return;
    }

    if (level == 0)
    {
        return;
    }

    if (System_IsKey(KEY_LSHIFT) || System_IsKey(KEY_RSHIFT))
    {
        level += 10;
    }

    level--;
    if (level == gameLevel && gameTicks == 0)
    {
        return;
    }

    gameLevel = level;

    Action = Game_ChangeLevel;
}

void DoCheatDisabled()
{
    if (cheatCode[cheatPos] != gameInput - KEY_0 + '0')
    {
        cheatPos = 0;
        Game_Pause(0);
        return;
    }

    cheatPos++;

    if (cheatCode[cheatPos] != '\0')
    {
        return;
    }

    cheatEnabled = 1;
    cheatPos = 0;

    Gameover_DrawCheat();
    Cheat_Responder = DoCheatEnabled;
}
