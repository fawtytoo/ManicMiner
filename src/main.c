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
    Audio_Init();
    System_Init();

    while (gameRunning)
    {
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
