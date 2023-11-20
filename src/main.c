#include "common.h"
#include "system.h"
#include "video.h"
#include "audio.h"

int     gameRunning = 1, gameInput;

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
        Flasher();
    }

    System_Quit();

    return 0;
}
