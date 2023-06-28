#include "common.h"
#include "game.h"

char    cheatCode[2][11] = {"6031769.", "writetyper."};
int     cheatCodeUsed = -1;
int     cheatEnabled = 0;
int     cheatPos = 0;
int     cheatLevel[2];
EVENT   Cheat_Responder = DoCheatDisabled;

void DoCheatEnabled()
{
    int level;

    if (gameInput == KEY_ENTER && cheatPos > 0)
    {
        level = cheatLevel[0];
        if (cheatPos == 2)
        {
            level = level * 10 + cheatLevel[1];
        }
        level--;

        cheatPos = 0;

        if (level < 0 || level > TWENTY || level == gameLevel)
        {
            return;
        }

        gameLevel = level;

        Action = Game_Action;
        return;
    }

    if (gameInput < KEY_0 || gameInput > KEY_9)
    {
        cheatPos = 0;
        return;
    }

    cheatLevel[cheatPos] = gameInput - KEY_0;
    cheatPos++;
}

void DoCheatDisabled()
{
    if (cheatCode[0][cheatPos] == gameInput - KEY_0 + '0' && cheatCodeUsed != 1)
    {
        cheatCodeUsed = 0;
    }
    else if (cheatCode[1][cheatPos] == gameInput - KEY_A + 'a' && cheatCodeUsed != 0)
    {
        cheatCodeUsed = 1;
    }
    else
    {
        cheatCodeUsed = -1;
        cheatPos = 0;
        return;
    }

    cheatPos++;

    if (cheatCode[cheatCodeUsed][cheatPos] != '.')
    {
        return;
    }

    cheatEnabled = 1;
    cheatPos = 0;

    Gameover_DrawCheat();
    Cheat_Responder = DoCheatEnabled;
}
