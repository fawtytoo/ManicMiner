#include "common.h"
#include "video.h"
#include "audio.h"
#include "game.h"

static u16      plinthSprite[16] = {65535, 29262, 35409, 43605, 19026, 4680, 8772, 10836, 10836, 10836, 10836, 10836, 10836, 10836, 10836, 10836};
static u16      bootSprite[16] = {10944, 13632, 16320, 2304, 2304, 8064, 4224, 4224, 4480, 8768, 8376, 22820, 17474, 17410, 17410, 65535};
static u16      minerSprite[16] = {96, 992, 1984, 832, 992, 960, 384, 960, 2016, 2016, 3952, 4016, 960, 1888, 1760, 1904};

static int      bootTicks;

static char     textGame[18] = "\x1\x0\x2\x0" "G " "\x2\x0" "a " "\x2\x0" "m " "\x2\x0" "e";
static char     textOver[18] = "\x1\x0\x2\x0" "O " "\x2\x0" "v " "\x2\x0" "e " "\x2\x0" "r";

void Gameover_DrawCheat()
{
    Video_Sprite(LIVES + WIDTH - 3 * 8, bootSprite, 0x0, 0x5);
}

static void DoGameoverDrawer()
{
    if (bootTicks <= 96)
    {
        Video_Sprite((bootTicks & 126) * WIDTH + 15 * 8, bootSprite, 0x0, 0x7);
        Video_LevelPaperFill((bootTicks & 12) >> 2);
    }

    if (bootTicks < 96)
    {
        return;
    }

    Video_WriteLarge(48 * WIDTH, 7 * 8, textGame);
    Video_WriteLarge(48 * WIDTH, 18 * 8, textOver);
}

static void DoGameoverTicker()
{
    int c = bootTicks >> 2;

    textGame[3] = c++ & 0x7;
    textGame[7] = c++ & 0x7;
    textGame[11] = c++ & 0x7;
    textGame[15] = c++ & 0x7;
    textOver[3] = c++ & 0x7;
    textOver[7] = c++ & 0x7;
    textOver[11] = c++ & 0x7;
    textOver[15] = c++ & 0x7;

    bootTicks++;

    if (bootTicks == 256)
    {
        Action = Title_Action;
    }
}

static void DoGameoverInit()
{
    Game_CheckHighScore();

    Video_PixelFill(0, 128 * WIDTH, 0x0);
    Video_Sprite(112 * WIDTH + 15 * 8, plinthSprite, 0x0, 0x7);
    Video_Sprite(96 * WIDTH + 15 * 8, minerSprite, 0x0, 0x7);

    bootTicks = 0;

    Audio_Play(MUS_STOP);
    Audio_Sfx(SFX_GAMEOVER);

    Ticker = DoGameoverTicker;
}

void Gameover_Action()
{
    Responder = DoNothing;
    Ticker = DoGameoverInit;
    Drawer = DoGameoverDrawer;

    Action = DoNothing;
}
