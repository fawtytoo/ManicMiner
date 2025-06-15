#include "common.h"
#include "game.h"
#include "video.h"

static int      spgTrace[48];
static int      spgCount;

void DoSpgDrawer()
{
    Video_TileInk(spgTrace[--spgCount], 0x7);

    while (spgCount--)
    {
        Video_TilePaper(spgTrace[spgCount], 0x4);
        Video_TileInk(spgTrace[spgCount], 0x7);
    }
}

void DoSpgTicker()
{
    int tile = 23, dir = 0;
    int air = 0;
    int way[2] = {32, -1};
    int this;

    spgCount = 0;
    spgTrace[spgCount++] = tile;

    do
    {
        this = Level_GetSpgTile(tile);

        if (this & B_ROBOT)
        {
            dir = 1 - dir;
        }

        if (this & B_MINER)
        {
            air = 8;
        }

        tile += way[dir];

        spgTrace[spgCount++] = tile;
    }
    while (Level_GetTileType(tile) == T_SPACE);

    // it should not matter if Willy is offset vertically
    // the air should reduce at the same rate
    // as he does not take up any more room visually
    // so will always be 8 units in either direction
    Game_ReduceAir(air);
}
