#include "misc.h"
#include "video.h"
#include "game.h"
#include "audio.h"

// start positions -------------------------------------------------------------
typedef struct
{
    int     x, y;
    int     frame;
    u8      ink;
}
START;

static START    minerStart[20] =
{
    {.x =  2, .y = 13, .frame = 4, .ink = 0x8},
    {.x =  2, .y = 13, .frame = 4, .ink = 0x7},
    {.x =  2, .y = 13, .frame = 4, .ink = 0x7},
    {.x = 29, .y = 13, .frame = 3, .ink = 0x4},
    {.x =  1, .y =  3, .frame = 4, .ink = 0x7},
    {.x = 15, .y =  3, .frame = 0, .ink = 0x6},
    {.x =  2, .y = 13, .frame = 4, .ink = 0x7},
    {.x =  2, .y = 13, .frame = 4, .ink = 0x7},
    {.x =  1, .y = 13, .frame = 4, .ink = 0x7},
    {.x =  1, .y =  4, .frame = 4, .ink = 0x7},
    {.x =  3, .y =  1, .frame = 4, .ink = 0x7},
    {.x =  2, .y = 13, .frame = 4, .ink = 0x7},
    {.x = 29, .y = 13, .frame = 4, .ink = 0x7},
    {.x = 29, .y = 13, .frame = 4, .ink = 0x0},
    {.x =  2, .y = 13, .frame = 4, .ink = 0x7},
    {.x =  2, .y = 13, .frame = 4, .ink = 0x7},
    {.x =  1, .y =  3, .frame = 0, .ink = 0x7},
    {.x = 29, .y = 13, .frame = 0, .ink = 0x7},
    {.x = 14, .y = 10, .frame = 4, .ink = 0x5},
    {.x = 27, .y = 13, .frame = 3, .ink = 0x7}
};

// jump stage info -------------------------------------------------------------
typedef struct
{
    int     y;
    int     tile;
    int     align;
    // sfx
    int     pitch;
    int     duration;
}
JUMP;

static JUMP     jumpInfo[18] =
{
    {.y = -4, .tile = -32, .align = 6, .pitch = 72, .duration = 5},
    {.y = -4, .tile =   0, .align = 4, .pitch = 74, .duration = 5},
    {.y = -3, .tile = -32, .align = 6, .pitch = 76, .duration = 4},
    {.y = -3, .tile =   0, .align = 6, .pitch = 78, .duration = 4},
    {.y = -2, .tile =   0, .align = 4, .pitch = 80, .duration = 3},
    {.y = -2, .tile = -32, .align = 6, .pitch = 82, .duration = 3},
    {.y = -1, .tile =   0, .align = 6, .pitch = 84, .duration = 2},
    {.y = -1, .tile =   0, .align = 6, .pitch = 86, .duration = 2},
    {.y =  0, .tile =   0, .align = 6, .pitch = 88, .duration = 1},
    {.y =  0, .tile =   0, .align = 6, .pitch = 88, .duration = 1},
    {.y =  1, .tile =   0, .align = 6, .pitch = 86, .duration = 2},
    {.y =  1, .tile =   0, .align = 6, .pitch = 84, .duration = 2},
    {.y =  2, .tile =  32, .align = 4, .pitch = 82, .duration = 3},
    {.y =  2, .tile =   0, .align = 6, .pitch = 80, .duration = 3},
    {.y =  3, .tile =   0, .align = 6, .pitch = 78, .duration = 4},
    {.y =  3, .tile =  32, .align = 4, .pitch = 76, .duration = 4},
    {.y =  4, .tile =   0, .align = 6, .pitch = 74, .duration = 5},
    {.y =  4, .tile =  32, .align = 4, .pitch = 72, .duration = 5}
};

// -----------------------------------------------------------------------------
#define D_LEFT      0
#define D_RIGHT     1
#define D_JUMP      2

static u16      minerSprite[8][16] =
{
    { 1536, 15872, 31744, 13312, 15872, 15360,  6144, 15360, 32256, 32256, 63232, 64256, 15360, 30208, 28160, 30464},
    {  384,  3968,  7936,  3328,  3968,  3840,  1536,  3840,  7040,  7040,  7040,  7552,  3840,  1536,  1536,  1792},
    {   96,   992,  1984,   832,   992,   960,   384,   960,  2016,  2016,  3952,  4016,   960,  1888,  1760,  1904},
    {   24,   248,   496,   208,   248,   240,    96,   240,   504,  1020,  2046,  1782,   248,   474,   782,   900},
    {   96,   124,    62,    44,   124,    60,    24,    60,   126,   126,   239,   223,    60,   110,   118,   238},
    {  384,   496,   248,   176,   496,   240,    96,   240,   504,   472,   472,   440,   240,    96,    96,   224},
    { 1536,  1984,   992,   704,  1984,   960,   384,   960,  2016,  2016,  3824,  3568,   960,  1760,  1888,  3808},
    { 6144,  7936,  3968,  2816,  7936,  3840,  1536,  3840,  8064, 16320, 32736, 28512,  7936, 23424, 28864,  8640}
};

static int      minerFrame, minerDir, minerMove;
static int      minerAir, jumpStage;
static u8       minerInk;

u8              minerX, minerY;
int             minerTile, minerAlign;

// sprite sequencing -----------------------------------------------------------
static u8       minerSeqIndex;
static TIMER    minerSeqTimer;

void Miner_SetSeq(int index, int speed)
{
    Timer_Set(&minerSeqTimer, 1, speed);
    minerSeqIndex = index;
}

void Miner_IncSeq()
{
    minerSeqIndex += Timer_Update(&minerSeqTimer);
    minerSeqIndex &= 7;
}

void Miner_DrawSeqSprite(int pos, u8 paper, u8 ink)
{
    Video_Sprite(pos, minerSprite[minerSeqIndex], paper, ink);
}

// -----------------------------------------------------------------------------
static int IsSolid(int tile)
{
    if (Level_GetTileType(tile) == T_SOLID)
    {
        return 1;
    }

    if (Level_GetTileType(tile + 32) == T_SOLID)
    {
        return 1;
    }

    if (Level_GetTileType(tile + 64) == T_SOLID)
    {
        if (minerAlign == 6)
        {
            return 1;
        }

        if (minerAir == 1 && jumpStage > 9)
        {
            minerAir = 0;
        }
    }

    return 0;
}

static void MoveLeftRight()
{
    if (minerMove == 0)
    {
        return;
    }

    if (minerDir == D_LEFT)
    {
        if (minerFrame < 3)
        {
            minerFrame++;
            return;
        }

        if (IsSolid(minerTile - 1))
        {
            return;
        }

        minerTile--;
        minerX -= 8;
        minerFrame = 0;
    }
    else
    {
        if (minerFrame < 7)
        {
            minerFrame++;
            return;
        }

        if (IsSolid(minerTile + 2))
        {
            return;
        }

        minerTile++;
        minerX += 8;
        minerFrame = 4;
    }
}

static void MinerMove()
{
    int     i, tile, conveyDir = C_NONE, type[2];
    JUMP    *jump;

    if (minerAir == 1)
    {
        jump = &jumpInfo[jumpStage];

        tile = minerTile + jump->tile;
        if (Level_GetTileType(tile) == T_SOLID || Level_GetTileType(tile + 1) == T_SOLID)
        {
            minerAir = 2;
            minerMove = 0;
            return;
        }

        Audio_MinerSfx(jump->pitch, jump->duration);

        minerY += jump->y;
        minerTile = tile;
        minerAlign = jump->align;
        jumpStage++;

        if (jumpStage == 18)
        {
            minerAir = 6;
            return;
        }

        if (jumpStage != 13 && jumpStage != 16)
        {
            MoveLeftRight();
            return;
        }
    }

    if (minerAlign == 4)
    {
        tile = minerTile + 64;
        type[0] = Level_GetTileType(tile);
        type[1] = Level_GetTileType(tile + 1);

        if (type[0] == T_COLLAPSE)
        {
            Level_CollapseTile(tile);
        }

        if (type[1] == T_COLLAPSE)
        {
            Level_CollapseTile(tile + 1);
        }

        if (type[0] == T_HARM || type[1] == T_HARM)
        {
            if (minerAir == 1 && (type[0] <= T_SPACE || type[1] <= T_SPACE))
            {
                MoveLeftRight();
            }
            else
            {
                Action = Die_Action;
            }
            return;
        }

        if (type[0] > T_SPACE || type[1] > T_SPACE)
        {
            if (minerAir >= 12)
            {
                Action = Die_Action;
                return;
            }

            minerAir = 0;

            if (type[0] == T_CONVEYL || type[1] == T_CONVEYL)
            {
                conveyDir = C_LEFT;
            }
            else if (type[0] == T_CONVEYR || type[1] == T_CONVEYR)
            {
                conveyDir = C_RIGHT;
            }

            System_UpdateKeys();

            i = 0;

            if (System_IsKeyLeft() || conveyDir == C_LEFT)
            {
                i += 1;
            }

            if (System_IsKeyRight() || conveyDir == C_RIGHT)
            {
                i += 2;
            }

            if (i == 0)
            {
                minerMove = 0;
            }
            else if (i == 1)
            {
                if (minerDir == D_RIGHT)
                {
                    minerDir = D_LEFT;
                    minerMove = 0;
                    minerFrame ^= 7;
                }
                else
                {
                    minerMove = 1;
                }
            }
            else if (i == 2)
            {
                if (minerDir == D_LEFT)
                {
                    minerDir = D_RIGHT;
                    minerMove = 0;
                    minerFrame ^= 7;
                }
                else
                {
                    minerMove = 1;
                }
            }

            if (System_IsKeyJump())
            {
                minerAir = 1;
                jumpStage = 0;
            }

            MoveLeftRight();
            return;
        }
    }

    if (minerAir == 1)
    {
        MoveLeftRight();
        return;
    }

    minerMove = 0;
    if (minerAir == 0)
    {
        minerAir = 2;
        return;
    }

    minerAir++;

    Audio_MinerSfx(78 - minerAir, 4);
    minerY += 4;
    minerAlign = 4;
    if (minerY & 7)
    {
        minerAlign = 6;
    }
    else
    {
        minerTile += 32;
    }
}

void DoMinerTicker()
{
    int cell, tile, type;
    int adj = 1;

    MinerMove();

    tile = minerTile;
    for (cell = 0; cell < minerAlign; cell++, tile += adj, adj ^= 30)
    {
        Level_SetSpgTile(tile, B_MINER);

        type = Level_GetTileType(tile);
        if (type == T_ITEM)
        {
            Game_GotItem(tile);
        }
        else if (type == T_SWITCHOFF)
        {
            Level_Switch(tile);
        }
        else if (type == T_HARM)
        {
            Action = Die_Action;
        }
    }
}

void DoMinerDrawer()
{
    Video_Miner((minerY << 8) | minerX, minerSprite[minerFrame], minerInk);
}

void Miner_Init()
{
    START   *start = &minerStart[gameLevel];

    minerX = start->x * 8;
    minerY = start->y * 8;
    minerTile = start->y * 32 + start->x;
    minerAlign = 4;
    minerFrame = start->frame;
    minerDir = start->frame >> 2;
    minerMove = 0;
    minerAir = 0;

    minerInk = start->ink;
}
