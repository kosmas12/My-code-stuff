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

void Quit(SDL_AudioDeviceID deviceID, Uint8 *wavBuffer) {
    SDL_CloseAudioDevice(deviceID);
	SDL_FreeWAV(wavBuffer);
	SDL_Quit();
}

int main()
{
    char* fileToPlay = "nxdk.wav";

    #if defined (NXDK)
    XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
    debugClearScreen();
    #endif

    SDL_Init(SDL_INIT_AUDIO|SDL_INIT_JOYSTICK);

    // declare the needed variables
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_GameController *controller;
    
    for (int i = 0; i < SDL_NumJoysticks(); i++) { // For the time that i is smaller than the number of connected Joysticks

        if(SDL_IsGameController(i)) { // If i (which we use to iterate through the connected controllers) as a port number is a Game Controller
            controller = SDL_GameControllerOpen(i); // Open the controller

            if(controller) { // If we find that we opened a controller
                printf("Opened controller %s on port %d\n", SDL_GameControllerName(controller), i);
                break; // Exit the loop
            }
                
        }
        else
        {
            printf("Couldn't open a controller on port%d\n", i);
        }
    }
    
    SDL_LoadWAV(fileToPlay, &wavSpec, &wavBuffer, &wavLength); // FIXME: Ask for file at runtime

    SDL_AudioDeviceID deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0); //NULL means default

    int play = SDL_QueueAudio(deviceID, wavBuffer, wavLength); //Might compare to something for a feature in the future
    
    while (1)
    {   
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                Quit(deviceID, wavBuffer);
            }
        }

        if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
        {
            SDL_PauseAudioDevice(deviceID, 1);
        }
        else
        {
            SDL_PauseAudioDevice(deviceID, 0); //SDL_PauseAudioDevice with 0 in place of pause_on means unpaused, thus playing
        }
        //FIXME: Make it run for duration of song
    }

    return 0;
}
