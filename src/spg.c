#include "common.h"
#include "game.h"
#include "video.h"

void DoSpgDrawer()
{
    u16     tile = 23, dir = 32;
    int air = 0;
    int this;

    do
    {
        Video_TilePaper(tile, 0x4);
        Video_TileInk(tile, 0x7);

        this = Level_GetSpgTile(tile);

        if (this & B_ROBOT)
        {
            dir ^= ((255 << 8) | 223);
        }

        if (this & B_MINER)
        {
            air = 8;
        }

        tile += dir;
    }
    while (Level_GetTileType(tile) == T_SPACE);

    Video_TileInk(tile, 0x7);

    // it should not matter if Willy is offset vertically
    // the air should reduce at the same rate
    // as he does not take up any more room visually
    // so will always be 8 units in either direction
    Game_ReduceAir(air);
}
