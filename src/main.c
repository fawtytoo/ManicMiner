#include <SDL.h>

#include "misc.h"
#include "video.h"
#include "audio.h"

SDL_Window          *sdlWindow;
SDL_Renderer        *sdlRenderer;
SDL_Texture         *sdlTexture, *sdlTarget;
SDL_Surface         *sdlSurface;
SDL_Rect            sdlViewport;
SDL_AudioDeviceID   sdlAudio;

const BYTE          *keyState;

UINT                *sdlPixels;

SDL_Color           sdlColor;

int     gameRunning = 1, gameInput;

int     videoFlash = 0;
int     videoSync;

EVENT   Action = Loader_Action;
EVENT   Responder = DoNothing;
EVENT   Ticker = DoNothing;
EVENT   Drawer = DoNothing;
EVENT   Flasher = DoNothing;

void DoNothing()
{
}

void DoQuit()
{
    gameRunning = 0;
    Drawer = DoNothing;
    Ticker = DoNothing;
}

void System_LockAudio()
{
    SDL_LockAudioDevice(sdlAudio);
}

void System_UnlockAudio()
{
    SDL_UnlockAudioDevice(sdlAudio);
}

void SdlCallback(void *unused, Uint8 *stream, int length)
{
    (void)unused;

    short   *output = (short *)stream;

    while (length)
    {
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

int System_GetEvent(int *key)
{
    SDL_Event   event;

    *key = KEY_NONE;

    if (SDL_PollEvent(&event) == 0)
    {
        return 0;
    }

    if (event.type == SDL_QUIT)
    {
        DoQuit();
        return 1;
    }

    if (event.type == SDL_WINDOWEVENT)
    {
        if (event.window.event == SDL_WINDOWEVENT_SHOWN)
        {
            SDL_ShowCursor(SDL_DISABLE);
        }
    }

    if (event.type != SDL_KEYDOWN)
    {
        return 0;
    }

    if (event.key.repeat)
    {
        return 0;
    }

    switch (event.key.keysym.sym)
    {
      case SDLK_RETURN:
        *key = KEY_ENTER;
        break;

      case SDLK_ESCAPE:
        *key = KEY_ESCAPE;
        break;

      case SDLK_PAUSE:
      case SDLK_TAB:
        *key = KEY_PAUSE;
        break;

      case SDLK_m:
        *key = KEY_MUTE;
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
        *key = KEY_0 + (event.key.keysym.sym - SDLK_0);
        break;

      case SDLK_e:
      case SDLK_i:
      case SDLK_p:
      case SDLK_r:
      case SDLK_t:
      case SDLK_w:
      case SDLK_y:
        *key = KEY_A + (event.key.keysym.sym - SDLK_a);
        break;

      default:
        *key = KEY_ELSE;
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
    TIMER   timer;
    int     flash = 0;
    int     frame = 0;
    int     sync;

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

    want.freq = SAMPLERATE;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = 256;
    want.callback = SdlCallback;

    sdlAudio = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
    SDL_PauseAudioDevice(sdlAudio, 0);

    keyState = SDL_GetKeyboardState(NULL);

    Audio_Init();

    sync = mode.refresh_rate > TICKRATE;
    Timer_Set(&timer, TICKRATE, mode.refresh_rate);
    if (timer.rate < 1)
    {
        timer.rate = 1;
        timer.remainder = 0;
    }
    frame = timer.rate;

    while (gameRunning)
    {
        Action();

        while (System_GetEvent(&gameInput))
        {
            Responder();
        }

        SDL_LockTextureToSurface(sdlTexture, NULL, &sdlSurface);
        sdlPixels = (UINT *)sdlSurface->pixels;
        Ticker();
        Drawer();
        SDL_UnlockTexture(sdlTexture);

        flash++;
        if (flash == 20)
        {
            flash = 0;
            videoFlash = 1 - videoFlash;
        }

        if (--frame > 0)
        {
            continue;
        }

        frame = Timer_Update(&timer);

        SDL_SetRenderDrawColor(sdlRenderer, sdlColor.r, sdlColor.g, sdlColor.b, 0xff);
        SDL_RenderClear(sdlRenderer);
        SDL_SetRenderTarget(sdlRenderer, sdlTarget);
        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, NULL);
        SDL_SetRenderTarget(sdlRenderer, NULL);
        SDL_RenderCopy(sdlRenderer, sdlTarget, NULL, &sdlViewport);

        videoSync = 0;
        do
        {
            SDL_Delay(1);
        }
        while (!videoSync && sync);

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
