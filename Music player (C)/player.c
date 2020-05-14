#if defined (NXDK)
#include <hal/video.h>
#include <windows.h>
#include <hal/xbox.h>
#include <hal/debug.h>
#include <SDL.h>
#include <SDL_audio.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#endif
#include <stdio.h>

int main()
{
    #if defined (NXDK)
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
    debugClearScreen();
    #endif

    SDL_Init(SDL_INIT_AUDIO);

    // declare the needed variables
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;

    SDL_LoadWAV("D:\\nxdk.wav", &wavSpec, &wavBuffer, &wavLength); // FIXME: Support user selected file

    SDL_AudioDeviceID deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0); //NULL means default

    int play = SDL_QueueAudio(deviceID, wavBuffer, wavLength); //Might compare to something for a feature in the future

    SDL_PauseAudioDevice(deviceID, 0); //SDL_PauseAudioDevice with 0 in place of pause_on means unpaused, thus playing

    SDL_Delay(3000); //FIXME: Make it so that we determine the length of a song and Delay with that length in ms

    SDL_CloseAudioDevice(deviceID);
	SDL_FreeWAV(wavBuffer);
	SDL_Quit();

    return 0;
}
