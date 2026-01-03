#define WIDTH   256
#define HEIGHT  192

enum
{
    KEY_LEFT,
    KEY_RIGHT,
    KEY_JUMP,
    KEY_ENTER,
    KEY_LSHIFT,
    KEY_RSHIFT,
    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_ESCAPE,
    KEY_PAUSE,
    KEY_MUTE,
    KEY_ELSE,
    KEY_NONE
};

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

typedef void (*EVENT)(void);

extern EVENT    Action, Responder, Ticker, Drawer;
extern int      gameInput;

void DoNothing(void);
void DoQuit(void);

void Loader_Action(void);
void Title_Action(void);
void Game_Action(void);
void Gameover_Action(void);
void Trans_Action(void);
void Die_Action(void);
void Victory_Action(void);
