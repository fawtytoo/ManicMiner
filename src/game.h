#define LIVES   150 * WIDTH + 4
#define SCORE   172 * WIDTH

// game
extern int  gameLevel;
extern int  gameDemo;
extern int  gameLives;
extern int  gameAir, gameAirOld;
extern int  gamePaused;
extern int  gameTicks;

extern EVENT    Game_DrawAir;
extern EVENT    Game_ExtraLife;
extern EVENT    Game_Unpause;

void Game_ScoreAdd(int);
void Game_ReduceAir(int);
void Game_GameReset(void);
void Game_CheckHighScore(void);
void Game_GotItem(int);
void Game_ChangeLevel(void);

// cheat
extern int      cheatEnabled;
extern EVENT    Cheat_Responder;

void DoCheatDisabled(void);

// gameover
void Gameover_DrawCheat(void);

// level
#define EUGENE  4
#define SKYLAB  13
#define SPG     18
#define TWENTY  19

enum
{
    T_ITEM,
    T_SWITCHOFF,
    T_SWITCHON,
    T_SPACE,
    T_SOLID,
    T_FLOOR,
    T_COLLAPSE,
    T_CONVEYL,
    T_CONVEYR,
    T_HARM,
    T_VOID
};

enum
{
    C_NONE,
    C_LEFT,
    C_RIGHT
};

extern int  levelTicks;

void Level_Init(void);
void Level_Drawer(void);
void Level_Ticker(void);
int Level_GetTileType(int);
void Level_CollapseTile(int);
void Level_TileDelete(int);
void Level_Switch(int);
void Level_SetSpgTile(int, int);
int Level_GetSpgTile(int);
int Level_ReduceItemCount(void);

// miner
extern u8   minerX, minerY;
extern int  minerTile, minerAlign;

void Miner_Init(void);
void DoMinerTicker(void);
void DoMinerDrawer(void);
void Miner_SetSeq(int, int);
void Miner_IncSeq(void);
void Miner_DrawSeqSprite(int, u8, u8);

// portal
void Portal_Init(void);
void Portal_Drawer(void);
void DoPortalTicker(void);

// robots
void Robots_Init(void);
void Robots_Drawer(void);
void Robots_Ticker(void);
void Robots_Eugene(void);
void Robots_Barrel(void);
void Robots_Kong(void);

// spg
void DoSpgTicker(void);
void DoSpgDrawer(void);

// title
void Title_ScreenCopy(void);
