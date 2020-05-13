#if defined (NXDK)
#include <hal/video.h>
#include <windows.h>
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
    #endif

    SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO);

    // declare the needed variables
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;

    SDL_LoadWAV("songname", &wavSpec, &wavBuffer, &wavLength); // FIXME: Support user selected file

    SDL_AudioDeviceID deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0); //NULL means default

    int play = SDL_QueueAudio(deviceID, wavBuffer, wavLength); //Might compare to something for a feature in the future

    SDL_PauseAudioDevice(deviceID, 0); //SDL_PauseAudioDevice with 0 in place of pause_on means unpaused, thus playing

    SDL_Delay(246000); //FIXME: Make it so that we determine the length of a song and Delay with that length in ms

    SDL_CloseAudioDevice(deviceID);
	SDL_FreeWAV(wavBuffer);
	SDL_Quit();

    return 0;
}
