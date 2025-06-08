#define B_LEVEL  1
#define B_ROBOT  2
#define B_MINER  4

#define TILE2PIXEL(t)   (((t & 992) << 6) | ((t & 31) << 3))

void Video_CopyColour(u8 *, int, int);
void Video_CopyBytes(u8 *);
void Video_Write(int, char *);
void Video_WriteLarge(int, int, char *);
void Video_DrawPiano(void);
void Video_Sprite(int, u16 *, u8, u8);
void Video_SpriteBlend(int, u16 *, u8);
void Video_Miner(int, u16 *, u8);
int Video_Tile(int, u8 *, u8, u8, int);
void Video_PixelFill(int, int, u8);
void Video_LevelInkFill(u8);
void Video_LevelPaperFill(u8);
void Video_TileInk(int, u8);
void Video_TilePaper(int, u8);
void Video_AirBar(int, u8);
void Video_PianoKey(int, u8);
int Video_TextWidth(char *);
