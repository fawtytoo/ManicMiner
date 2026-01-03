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
