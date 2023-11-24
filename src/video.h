#define B_LEVEL  1
#define B_ROBOT  2
#define B_MINER  4

#define TILE2PIXEL(t)   (((t & 992) << 6) | ((t & 31) << 3))

void Video_CopyColour(BYTE *, int, int);
void Video_CopyBytes(BYTE *);
void Video_Write(int, char *);
void Video_WriteLarge(int, int, char *);
void Video_DrawPiano(void);
void Video_Sprite(int, WORD *, BYTE, BYTE);
void Video_SpriteBlend(int, WORD *, BYTE);
void Video_Miner(int, WORD *, BYTE);
int Video_Tile(int, BYTE *, BYTE, BYTE, int);
void Video_PixelFill(int, int, BYTE);
void Video_LevelInkFill(BYTE);
void Video_LevelPaperFill(BYTE);
void Video_TileInk(int, BYTE);
void Video_TilePaper(int, BYTE);
void Video_AirBar(int, BYTE);
void Video_PianoKey(int, BYTE);
int Video_TextWidth(char *);
