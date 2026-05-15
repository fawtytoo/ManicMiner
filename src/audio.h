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
