#include "misc.h"
#include "system.h"
#include "video.h"
#include "audio.h"

int     gameRunning = 1, gameInput;

int     videoFlash = 0;
int     videoSync;

EVENT   Action = Loader_Action;
EVENT   Responder = DoNothing;
EVENT   Ticker = DoNothing;
EVENT   Drawer = DoNothing;
EVENT   Flasher = DoNothing;

void DoNothing()
{
}

void DoQuit()
{
    gameRunning = 0;
    Drawer = DoNothing;
    Ticker = DoNothing;
}

int main()
{
    TIMER   timer;
    int     flash = 0;
    int     frame = 0;
    int     rate;
    int     sync;

    Audio_Init();

    rate = System_Init();
    sync = rate > TICKRATE;
    Timer_Set(&timer, TICKRATE, rate);
    if (timer.rate < 1)
    {
        timer.rate = 1;
        timer.remainder = 0;
    }
    frame = timer.rate;

    while (gameRunning)
    {
        Action();

        while (System_GetEvent(&gameInput))
        {
            Responder();
        }

        Ticker();
        Drawer();

        flash++;
        if (flash == 20)
        {
            flash = 0;
            videoFlash = 1 - videoFlash;
        }

        if (--frame > 0)
        {
            continue;
        }

        frame = Timer_Update(&timer);

        Video_Draw();

        videoSync = 0;
        do
        {
            System_Delay();
        }
        while (!videoSync && sync);

        System_UpdateVideo();
    }

    System_Quit();

    return 0;
}
