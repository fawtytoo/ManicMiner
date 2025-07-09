#include "misc.h"
#include "audio.h"
#include "video.h"

#include "game.h"

#define AIR     138 * WIDTH + 28

static int      gameMusic = MUS_PLAY;

static int      airData[20] = {224, 224, 220, 220, 220, 220, 220, 220, 220, 224, 220, 220, 224, 224, 224, 224, 220, 220, 224, 224};

static int      levelBorder[20] = {0xb, 0x1, 0xa, 0xd, 0xb, 0x0, 0x4, 0x5, 0x1, 0x9, 0x1, 0x5, 0xa, 0x1, 0xb, 0xd, 0x8, 0x1, 0x0, 0x5};

static int      gameScore, gameHiScore = 0;
static int      gameExtraLifeCount;

static int      gameFrame;
static TIMER    gameTimer;

int             gamePaused = 0;
int             gameLevel;
int             gameTicks;
int             gameDemo = 1;
int             gameLives;
int             gameAir, gameAirOld;

EVENT           Game_ExtraLife = DoNothing;
EVENT           Game_DrawAir = DoNothing;
EVENT           Game_Unpause = DoNothing;

EVENT           Spg_Ticker, Spg_Drawer;
EVENT           Miner_Ticker, Miner_Drawer;
EVENT           Portal_Ticker;

void Game_CheckHighScore()
{
    if (gameScore > gameHiScore)
    {
        gameHiScore = gameScore;
    }
}

void Game_DrawLives()
{
    int l, pos = LIVES;

    for (l = 0; l < gameLives - 1; l++, pos += 16)
    {
        Miner_DrawSeqSprite(pos, 0x0, 0x5);
    }
}

static void DoDrawAir()
{
    gameAirOld--;
    Video_AirBar(AIR + gameAirOld, 0x7);

    if (gameAirOld == ((gameAir + 7) / 8))
    {
        Game_DrawAir = DoNothing;
    }
}

void Game_ReduceAir(int amount)
{
    gameAir -= amount;

    if (gameAir < 0)
    {
        gameAir = 0;
    }

    if (gameAirOld > ((gameAir + 7) / 8))
    {
        Game_DrawAir = DoDrawAir;
    }
}

static void GameDrawScore(int pos, int x, int score, u8 ink)
{
    int     digit = 6, i = 9;
    char    text[11] = "\x1\x0\x2\x0" ".....0";

    text[3] = ink;
    while (digit-- && score)
    {
        text[i--] = '0' + (score % 10);
        score /= 10;
    }

    Video_WriteLarge(pos, x, text);
}

void Game_DrawScore()
{
    GameDrawScore(SCORE, WIDTH - 6 * 8 - 4, gameScore, 0xe);
}

void Game_DrawHiScore()
{
    GameDrawScore(SCORE, 5 * 8 + 4, gameHiScore, 0x6);
}

static void DoExtraLife()
{
    gameExtraLifeCount--;

    System_Border(gameExtraLifeCount >> 2);

    if (gameExtraLifeCount > 0)
    {
        return;
    }

    System_Border(levelBorder[gameLevel]);
    Game_ExtraLife = DoNothing;
}

void Game_ScoreAdd(int score)
{
    int previous = gameScore / 10000;

    gameScore += score;
    Game_DrawScore();

    if (gameScore / 10000 == previous)
    {
        return;
    }

    gameLives++;
    Game_DrawLives();
    gameExtraLifeCount = 16 << 2;
    Game_ExtraLife = DoExtraLife;
}

void Game_GotItem(int tile)
{
    Level_TileDelete(tile);

    Game_ScoreAdd(100);

    if (Level_ReduceItemCount() > 0)
    {
        return;
    }

    if (gameLevel == EUGENE)
    {
        Robots_Eugene();
    }

    Portal_Ticker = DoPortalTicker;
}

static void DoGameDrawer()
{
    if (gameMusic == MUS_PLAY)
    {
        Game_DrawLives();
    }

    Game_DrawAir();
    Game_ExtraLife();

    if (gameFrame == 0)
    {
        return;
    }

    Level_Drawer();
    Robots_Drawer();
    Miner_Drawer();
    Spg_Drawer();
    Portal_Drawer();
}

static void DoGameDrawOnce()
{
    DoGameDrawer();

    Drawer = DoNothing;
}

static void DoGameTicker()
{
    if (gameMusic == MUS_PLAY)
    {
        Miner_IncSeq();
    }

    gameFrame = Timer_Update(&gameTimer);
    if (gameFrame == 0)
    {
        return;
    }

    gameTicks++;

    Level_Ticker();
    Robots_Ticker();
    Miner_Ticker();
    Spg_Ticker();
    Portal_Ticker();

    Game_ReduceAir(1);
    if (gameAir == 0)
    {
        Action = Die_Action;
    }

    if (gameDemo == 0)
    {
        return;
    }

    if (gameTicks < 64)
    {
        return;
    }

    Action = Trans_Action;
}

static void GamePause(int paused)
{
    gamePaused = paused;

    if (paused)
    {
        Ticker = DoNothing;
        Drawer = DoNothing;
        Audio_Play(MUS_STOP);
    }
    else
    {
        Ticker = DoGameTicker;
        Drawer = DoGameDrawer;
        Audio_Play(gameMusic);
    }
}

static void DoGameUnpause()
{
    GamePause(0);
    Game_Unpause = DoNothing;
}

static void DoGameInit()
{
    int x;

    Level_Init();
    Robots_Init();
    Portal_Init();
    Portal_Ticker = DoNothing;

    Miner_Init();

    if (gameLevel == SPG)
    {
        Spg_Ticker = DoSpgTicker;
        Spg_Drawer = DoSpgDrawer;
        Spg_Ticker(); // prepare initial beam
    }
    else
    {
        Spg_Ticker = DoNothing;
        Spg_Drawer = DoNothing;
    }

    for (x = 0; x < 224; x++)
    {
        Video_AirBar(AIR + x, x < 48 ? 0x2 : 0x4);
    }

    gameAirOld = 224;
    gameAir = airData[gameLevel] * 8;
    Game_DrawAir = DoNothing;

    gameTicks = 0;

    System_Border(levelBorder[gameLevel]);

    Timer_Set(&gameTimer, 12, TICKRATE);
    gameFrame = 1; // immediate drawing

    if (gamePaused == 0)
    {
        Audio_Play(gameMusic);
        Ticker = DoGameTicker;
    }
    else
    {
        Ticker = DoNothing;
    }
}

static void DoGameDemoResponder()
{
    Action = Title_Action;
}

static void DoGameResponder()
{
    if (gameInput == KEY_PAUSE)
    {
        GamePause(1 - gamePaused);
        Game_Unpause = gamePaused ? DoGameUnpause : DoNothing;
        return;
    }
    else if (gameInput == KEY_MUTE)
    {
        gameMusic = gameMusic == MUS_PLAY ? MUS_STOP : MUS_PLAY;
        Audio_Play(gameMusic);
        Game_Unpause();
    }
    else if (gameInput == KEY_ESCAPE)
    {
        Action = Title_Action;
    }
    else
    {
        Cheat_Responder();
    }
}

void Game_GameReset()
{
    gameLives = 3;
    gameLevel = 0;
    gameScore = 0;

    gamePaused = 0;
    Game_Unpause = DoNothing;

    Game_ExtraLife = DoNothing;
    Game_DrawHiScore();

    Miner_SetSeq(0, 20);
    Game_DrawLives();

    if (cheatEnabled)
    {
        Gameover_DrawCheat();
    }

    if (gameDemo == 0)
    {
        Miner_Ticker = DoMinerTicker;
        Miner_Drawer = DoMinerDrawer;
    }
    else
    {
        Miner_Ticker = DoNothing;
        Miner_Drawer = DoNothing;
    }

    Audio_Music(MUS_GAME, MUS_STOP);
}

void Game_ChangeLevel()
{
    Ticker = DoGameInit;
    Drawer = gamePaused ? DoGameDrawOnce : DoGameDrawer;

    Action = DoNothing;
}

void Game_Action()
{
    Responder = gameDemo ? DoGameDemoResponder : DoGameResponder;
    Ticker = DoGameInit;
    Drawer = DoGameDrawer;

    Action = DoNothing;
}
