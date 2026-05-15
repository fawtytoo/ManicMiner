//  Copyright 2021-2026 by Steve Clark

//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.

//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:

//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required. 
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.

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
