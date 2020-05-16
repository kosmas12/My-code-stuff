#if defined (NXDK)
#include <hal/video.h>
#include <windows.h>
#include <hal/xbox.h>
#include <hal/debug.h>
#include <SDL.h>
#include <SDL_audio.h>
#define printf(...) debugPrint(__VA_ARGS__)
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#endif
#include <stdio.h>
#include <stdbool.h>

static Uint8 *audio_position; 
static Uint32 audio_length;

void audio_callback(void *userdata, Uint8 *stream, int len) {
	
	if (audio_length ==0)
		return;
	
	len = ( len > audio_length ? audio_length : len );
	SDL_memcpy (stream, audio_position, len); // simply copy from one buffer into the other
	
	audio_position += len;
	audio_length -= len;
}

void Quit(SDL_AudioDeviceID deviceID, Uint8 *wavBuffer) {
    SDL_CloseAudioDevice(deviceID);
	SDL_FreeWAV(wavBuffer);
	SDL_Quit();
}

static bool isNewlyPressed(bool is_held, bool *was_held) {

  if (is_held) { // If the button is held in this frame
    if (*was_held) { // If it was held in the previous frame
      return false; // The button isn't newly pressed
    } 
    else { // If it wasn't held in the previous frame
      *was_held = true; // Set was_held to true because the current frame will be the previous frame in the next frame
      return true; // The button is newly pressed, so we return true
    } 
        
  } 
  else { // If it isn't held in the current frame
    *was_held = false; // Set was_held to false
    return true; // The button isn't pressed at all, so we return false
  }
}

int main()
{

  bool a_is_held = true;
  bool a_was_held = true;

  #if defined (NXDK)
  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
  debugClearScreen();
  #endif

  SDL_Init(SDL_INIT_AUDIO|SDL_INIT_JOYSTICK);

  // declare the needed variables
  char* fileToPlay = "pausetest.wav";
  SDL_AudioSpec wavSpec;
  Uint32 wavLength;
  Uint8 *wavBuffer;
  SDL_GameController *controller;
  int controllerport = 0;
  char* controllername = NULL;

  for (int i = 0; i < SDL_NumJoysticks(); i++) { // For the time that i is smaller than the number of connected Joysticks

    if(SDL_IsGameController(i)) { // If i (which we use to iterate through the connected controllers) as a port number is a Game Controller
      controller = SDL_GameControllerOpen(i); // Open the controller

      if(controller) { // If we find that we opened a controller
        controllerport = i;
        controllername = SDL_GameControllerName(controller);
        break; // Exit the loop
      }
                
    }
  }
    
  SDL_LoadWAV(fileToPlay, &wavSpec, &wavBuffer, &wavLength);//FIXME: Ask for file at runtime

  wavSpec.callback = audio_callback;
	wavSpec.userdata = NULL;

  audio_position = wavBuffer;
	audio_length = wavLength;

  SDL_AudioDeviceID deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0); //NULL means default

  #if !defined (NXDK)
  printf("Opened controller: %s on port %d\n", controllername, controllerport);
  printf("Now playing: %s\n", fileToPlay);
  #endif

  while (audio_length > 0) {   
    #if defined (NXDK)
    XVideoWaitForVBlank();

    debugClearScreen();

    printf("Opened controller: %s on port %d\n", controllername, controllerport);
    printf("Now playing: %s\n", fileToPlay);
    #endif

      SDL_Event event;

      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
        {
          Quit(deviceID, wavBuffer);
        }
      }
        
      if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)) {
        SDL_PauseAudioDevice(deviceID, 1);
      }
      else {
        SDL_PauseAudioDevice(deviceID, 0); //SDL_PauseAudioDevice with 0 in place of pause_on means unpaused, thus playing
      }
      //FIXME: Make it run for duration of song
  }

  return 0;
}
