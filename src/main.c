#include <SDL.h>

#include "misc.h"
#include "video.h"
#include "audio.h"

static SDL_Window           *sdlWindow;
static SDL_Renderer         *sdlRenderer;
static SDL_Texture          *sdlTexture, *sdlTarget;
static SDL_Surface          *sdlSurface;
static SDL_Rect             sdlViewport;
static SDL_AudioDeviceID    sdlAudio;

static const Uint8          *keyState;

static u32                  *sdlPixels;

static SDL_Color            sdlColor;

static int                  gameRunning = 1;

int                         gameInput;

int                         videoSync = 0;

TIMER                       audioTimer;

EVENT                       Action = Loader_Action;
EVENT                       Responder = DoNothing;
EVENT                       Ticker = DoNothing;
EVENT                       Drawer = DoNothing;

void DoNothing()
{
}

void DoQuit()
{
    gameRunning = 0;
    Drawer = DoNothing;
    Ticker = DoNothing;
}

static void SdlCallback(void *unused, Uint8 *stream, int length)
{
    (void)unused;

    short   *output = (short *)stream;

    while (length)
    {
        if (Timer_Update(&audioTimer))
        {
            Audio_MusicEvent();
            Audio_SfxEvent();
            videoSync = 1;
        }

        Audio_Output(output);
        output += 2;
        length -= 4;
    }
}

void System_UpdateKeys()
{
    SDL_PumpEvents();
}

int System_IsKeyLeft()
{
    return keyState[SDL_SCANCODE_LEFT];
}

int System_IsKeyRight()
{
    return keyState[SDL_SCANCODE_RIGHT];
}

int System_IsKeyJump()
{
    return keyState[SDL_SCANCODE_SPACE];
}

static int System_GetEvent()
{
    SDL_Event   event;

    gameInput = KEY_NONE;

    if (SDL_PollEvent(&event) == 0)
    {
        return 0;
    }

    if (event.type == SDL_QUIT)
    {
        DoQuit();
    }

    if (event.type != SDL_KEYDOWN)
    {
        return 1;
    }

    if (event.key.repeat)
    {
        return 1;
    }

    switch (event.key.keysym.sym)
    {
      case SDLK_RETURN:
        gameInput = KEY_ENTER;
        break;

      case SDLK_ESCAPE:
        gameInput = KEY_ESCAPE;
        break;

      case SDLK_PAUSE:
      case SDLK_TAB:
        gameInput = KEY_PAUSE;
        break;

      case SDLK_m:
        gameInput = KEY_MUTE;
        break;

      case SDLK_0:
      case SDLK_1:
      case SDLK_2:
      case SDLK_3:
      case SDLK_4:
      case SDLK_5:
      case SDLK_6:
      case SDLK_7:
      case SDLK_8:
      case SDLK_9:
        gameInput = KEY_0 + (event.key.keysym.sym - SDLK_0);
        break;

      case SDLK_e:
      case SDLK_i:
      case SDLK_p:
      case SDLK_r:
      case SDLK_t:
      case SDLK_w:
      case SDLK_y:
        gameInput = KEY_A + (event.key.keysym.sym - SDLK_a);
        break;

      default:
        gameInput = KEY_ELSE;
    }

    return 1;
}

void System_SetPixel(int point, int index)
{
    *(sdlPixels + point) = videoPalette[index];
}

void System_Border(int index)
{
    sdlColor.r = videoPalette[index] >> 16;
    sdlColor.g = (videoPalette[index] >> 8) & 0xff;
    sdlColor.b = videoPalette[index] & 0xff;
}

int main()
{
    SDL_AudioSpec   want;
    SDL_DisplayMode mode;
    int             multiply;
    TIMER           timerFrame;
    int             frame = 0;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");

    SDL_GetDesktopDisplayMode(0, &mode);

    multiply = Video_Viewport(mode.w, mode.h, &sdlViewport.x, &sdlViewport.y, &sdlViewport.w, &sdlViewport.h);

    sdlWindow = SDL_CreateWindow("Manic Miner", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    sdlTarget = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, WIDTH * multiply, HEIGHT * multiply);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);

    SDL_ShowCursor(SDL_DISABLE);

    want.freq = SAMPLERATE;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = 256;
    want.callback = SdlCallback;

    sdlAudio = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
    SDL_PauseAudioDevice(sdlAudio, 0);

    keyState = SDL_GetKeyboardState(NULL);

    Timer_Set(&timerFrame, TICKRATE, mode.refresh_rate);
    Timer_Set(&audioTimer, TICKRATE, SAMPLERATE);

    while (gameRunning)
    {
        SDL_LockTextureToSurface(sdlTexture, NULL, &sdlSurface);
        sdlPixels = (u32 *)sdlSurface->pixels;

        frame = Timer_Update(&timerFrame);

        while (frame--)
        {
            Action();

            while (System_GetEvent())
            {
                if (gameInput != KEY_NONE)
                {
                    Responder();
                }
            }

            Ticker();
            Drawer();

            while (!videoSync)
            {
                SDL_Delay(1);
            }
            videoSync = 0;
        }

        SDL_UnlockTexture(sdlTexture);

        SDL_SetRenderDrawColor(sdlRenderer, sdlColor.r, sdlColor.g, sdlColor.b, 0xff);
        SDL_RenderClear(sdlRenderer);
        SDL_SetRenderTarget(sdlRenderer, sdlTarget);
        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
        SDL_SetRenderTarget(sdlRenderer, NULL);
        SDL_RenderCopy(sdlRenderer, sdlTarget, NULL, &sdlViewport);

        SDL_RenderPresent(sdlRenderer);
    }

    SDL_CloseAudioDevice(sdlAudio);

    SDL_DestroyTexture(sdlTexture);
    SDL_DestroyTexture(sdlTarget);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);

    SDL_Quit();

    return 0;
}
