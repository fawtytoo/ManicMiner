#define MUS_TITLE   0
#define MUS_GAME    1

#define MUS_STOP    0
#define MUS_PLAY    1

enum
{
    SFX_DIE,
    SFX_KONG,
    SFX_GAMEOVER,
    SFX_AIR,
    SFX_VICTORY,
    SFX_NONE
};

extern int  audioMusicPlaying;

void Audio_Output(short [2]);
void Audio_MusicEvent(void);
void Audio_SfxEvent(void);

void Audio_Music(int, int);
void Audio_Play(int);
void Audio_MinerSfx(int, int);
void Audio_Sfx(int);
void Audio_Drawer(void);
