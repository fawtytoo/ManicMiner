#include "common.h"
#include "system.h"
#include "video.h"
#include "audio.h"

int     gameRunning = 1, gameInput;

int     videoFlash = 0;

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
    int time;
    int flash = 0;

    Audio_Init();
    System_Init();

    time = System_GetTime();

    while (gameRunning)
    {
        if (time > System_GetTime())
        {
            continue;
        }
        time += 50;

        Action();

        while (System_GetEvent(&gameInput))
        {
            Responder();
        }

        System_LockVideo();
        Ticker();
        Drawer();
        System_UnlockVideo();
        System_UpdateVideo();

        flash++;
        if (flash == 20)
        {
            flash = 0;
            videoFlash = 1 - videoFlash;
        }
    }

    System_Quit();

    return 0;
}
