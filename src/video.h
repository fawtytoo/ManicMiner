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

#define B_LEVEL  1
#define B_ROBOT  2
#define B_MINER  4

#define TILE2PIXEL(t)   (((t & 992) << 6) | ((t & 31) << 3))

#define KEYBOARD    128 * WIDTH

void Video_CopyColour(u8 *, int, int);
void Video_CopyBytes(u8 *);
void Video_Write(int, char *);
void Video_WriteLarge(int, int, char *);
void Video_Sprite(int, u16 *, u8, u8);
void Video_SpriteBlend(int, u16 *, u8);
int Video_Miner(int, u16 *, u8);
int Video_Tile(int, u8 *, u8, u8, int);
void Video_PixelFill(int, int, u8);
void Video_LevelInkFill(u8);
void Video_LevelPaperFill(u8);
void Video_TileInk(int, u8);
void Video_TilePaper(int, u8);
void Video_AirBar(int, u8);
void Video_PianoKey(int, int, int);
int Video_TextWidth(char *);
