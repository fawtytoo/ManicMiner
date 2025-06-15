#include "misc.h"
#include "video.h"
#include "game.h"
#include "audio.h"

enum
{
    D_LEFT,
    D_RIGHT,
    D_JUMP
};

typedef struct
{
    int     x, y;
    int     frame;
    int     dir;
    u8      ink;
}
MINER;

typedef struct
{
    int y;
    int tile;
    int align;
    // sfx
    int length;
    int pitch;
}
JUMP;

static u16      minerSprite[8][16] =
{
    {96, 124, 62, 44, 124, 60, 24, 60, 126, 126, 239, 223, 60, 110, 118, 238},
    {384, 496, 248, 176, 496, 240, 96, 240, 504, 472, 472, 440, 240, 96, 96, 224},
    {1536, 1984, 992, 704, 1984, 960, 384, 960, 2016, 2016, 3824, 3568, 960, 1760, 1888, 3808},
    {6144, 7936, 3968, 2816, 7936, 3840, 1536, 3840, 8064, 16320, 32736, 28512, 7936, 23424, 28864, 8640},
    {24, 248, 496, 208, 248, 240, 96, 240, 504, 1020, 2046, 1782, 248, 474, 782, 900},
    {96, 992, 1984, 832, 992, 960, 384, 960, 2016, 2016, 3952, 4016, 960, 1888, 1760, 1904},
    {384, 3968, 7936, 3328, 3968, 3840, 1536, 3840, 7040, 7040, 7040, 7552, 3840, 1536, 1536, 1792},
    {1536, 15872, 31744, 13312, 15872, 15360, 6144, 15360, 32256, 32256, 63232, 64256, 15360, 30208, 28160, 30464}
};

static MINER    minerStart[20] =
{
    {2, 13, 0, D_RIGHT, 0x8}, {2, 13, 0, D_RIGHT, 0x7}, {2, 13, 0, D_RIGHT, 0x7}, {29, 13, 4, D_LEFT, 0x4},
    {1, 3, 0, D_RIGHT, 0x7}, {15, 3, 7, D_LEFT, 0x6}, {2, 13, 0, D_RIGHT, 0x7}, {2, 13, 0, D_RIGHT, 0x7},
    {1, 13, 0, D_RIGHT, 0x7}, {1, 4, 0, D_RIGHT, 0x7}, {3, 1, 0, D_RIGHT, 0x7}, {2, 13, 0, D_RIGHT, 0x7},
    {29, 13, 0, D_RIGHT, 0x7}, {29, 13, 0, D_RIGHT, 0x0}, {2, 13, 0, D_RIGHT, 0x7}, {2, 13, 0, D_RIGHT, 0x7},
    {1, 3, 7, D_LEFT, 0x7}, {29, 13, 7, D_LEFT, 0x7}, {14, 10, 0, D_RIGHT, 0x5}, {27, 13, 4, D_LEFT, 0x7}
};

static JUMP     jumpInfo[18] =
{
    {-4, -32, 6, 5, 72},
    {-4, 0, 4, 5, 74},
    {-3, -32, 6, 4, 76},
    {-3, 0, 6, 4, 78},
    {-2, 0, 4, 3, 80},
    {-2, -32, 6, 3, 82},
    {-1, 0, 6, 2, 84},
    {-1, 0, 6, 2, 86},
    {0, 0, 6, 1, 88},
    {0, 0, 6, 1, 88},
    {1, 0, 6, 2, 86},
    {1, 0, 6, 2, 84},
    {2, 32, 4, 3, 82},
    {2, 0, 6, 3, 80},
    {3, 0, 6, 4, 78},
    {3, 32, 4, 4, 76},
    {4, 0, 6, 5, 74},
    {4, 32, 4, 5, 72}
};

static int      minerFrame, minerDir, minerMove;
static int      minerAir, jumpStage;
static u8       minerInk;

static u8       minerSeqIndex;
static int      minerSeq[8] = {0, 1, 2, 3, 7, 6, 5, 4};
static TIMER    minerTimer;

u8              minerX, minerY;
int             minerTile, minerAlign;

void Miner_SetSeq(int index, int speed)
{
    Timer_Set(&minerTimer, 1, speed);
    minerSeqIndex = index;
}

void Miner_IncSeq()
{
    minerSeqIndex += Timer_Update(&minerTimer);
    minerSeqIndex &= 7;
}

void Miner_DrawSeqSprite(int pos, u8 paper, u8 ink)
{
    Video_Sprite(pos, minerSprite[minerSeq[minerSeqIndex]], paper, ink);
}

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
        if (minerFrame > 4)
        {
            minerFrame--;
            return;
        }

        if (IsSolid(minerTile - 1))
        {
            return;
        }

        minerTile--;
        minerX -= 8;
        minerFrame = 7;
    }
    else
    {
        if (minerFrame < 3)
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
        minerFrame = 0;
    }
}

static void MinerMove()
{
    int i, tile, conveyDir = C_NONE, type[2];

    if (minerAir == 1)
    {
        tile = minerTile + jumpInfo[jumpStage].tile;
        if (Level_GetTileType(tile) == T_SOLID || Level_GetTileType(tile + 1) == T_SOLID)
        {
            minerAir = 2;
            minerMove = 0;
            return;
        }

        Audio_MinerSfx(jumpInfo[jumpStage].pitch, jumpInfo[jumpStage].length);

        minerY += jumpInfo[jumpStage].y;
        minerTile = tile;
        minerAlign = jumpInfo[jumpStage].align;
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
                    minerFrame += 4;
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
                    minerFrame &= 3;
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
    MINER   *start = &minerStart[gameLevel];

    minerX = start->x * 8;
    minerY = start->y * 8;
    minerTile = start->y * 32 + start->x;
    minerAlign = 4;
    minerFrame = start->frame;
    minerDir = start->dir;
    minerMove = 0;
    minerAir = 0;

    minerInk = start->ink;
}
