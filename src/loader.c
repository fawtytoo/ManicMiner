#include "misc.h"
#include "video.h"

BYTE    loaderColour[2][256] =
{
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x20, 0x20, 0x00, 0x20, 0x20, 0x00, 0x00, 0x60, 0x60, 0x60, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x60, 0x00, 0x00, 0x00, 0x60, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x50, 0x50, 0x50, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x60, 0x00, 0x00, 0x00, 0x60, 0x00, 0x40, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x50, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x50, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x50, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    },
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x30, 0x00, 0x00, 0x20, 0x20, 0x00, 0x00, 0x20, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x50, 0x50, 0x00, 0x50, 0x50, 0x00, 0x00, 0x30, 0x00, 0x00, 0x20, 0x00, 0x20, 0x00, 0x20, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x50, 0x00, 0x50, 0x00, 0x50, 0x00, 0x30, 0x30, 0x30, 0x00, 0x20, 0x00, 0x00, 0x20, 0x20, 0x00, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00,
    }
};

int     loaderTicks = 0;

void DoLoaderDrawer()
{
    Video_CopyColour(loaderColour[videoFlash], 256, 256);
}

void DoLoaderTicker()
{
    if (loaderTicks++ == 256)
    {
        Action = Title_Action;
    }
}

void DoLoaderInit()
{
    Video_CopyColour(loaderColour[0], 256, 256);
    Video_Write(184 * WIDTH, "\x2\x7" "fawtytoo");
    Video_Write(184 * WIDTH + WIDTH - Video_TextWidth(BUILD), "\x2\x1" BUILD);

    Ticker = DoLoaderTicker;
}

void DoLoaderResponder()
{
    Action = Title_Action;
}

void Loader_Action()
{
    Responder = DoLoaderResponder;
    Ticker = DoLoaderInit;
    Drawer = DoLoaderDrawer;

    Action = DoNothing;
}
