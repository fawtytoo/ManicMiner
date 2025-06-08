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

    if (gameLives == 0)
    {
        Action = Gameover_Action;
        return;
    }

    Video_Sprite(LIVES + (gameLives - 1) * 16, dieBlank, 0x0, 0x0); // blank frame

    Action = Game_Action;
}

static void DoDieInit()
{
    gameLives--;

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
