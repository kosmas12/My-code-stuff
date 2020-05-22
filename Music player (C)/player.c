#if defined (NXDK)
#include <hal/video.h>
#include <windows.h>
#include <hal/xbox.h>
#include <hal/debug.h>
#include <SDL.h>
#include <SDL_audio.h>
#include <nxdk/mount.h>
#define printf(...) debugPrint(__VA_ARGS__)
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#endif
#include <stdio.h>
#include <stdbool.h>

static Uint8 *audio_position = NULL; 
static Uint32 audio_length = NULL;
Uint8 *wavBuffer = NULL;
char* fileToPlay = "CivilSin.wav";
SDL_AudioDeviceID deviceID = NULL;
SDL_GameController *controller = NULL;

void audio_callback(void *userdata, Uint8 *stream, int len) {
	
	if (audio_length ==0)
		return;
	
	len = ( len > audio_length ? audio_length : len );
	SDL_memcpy (stream, audio_position, len); // simply copy from one buffer into the other
	
	audio_position += len;
	audio_length -= len;
}

static void Quit(SDL_AudioDeviceID deviceID, Uint8 *wavBuffer) {
  SDL_CloseAudioDevice(deviceID);
	SDL_FreeWAV(wavBuffer);
	SDL_Quit();
} 

static void PlayFile() {

  Uint32 wavLength;
  SDL_AudioSpec wavSpec;

  SDL_LoadWAV(fileToPlay, &wavSpec, &wavBuffer, &wavLength);//FIXME: Ask for file at runtime
  wavSpec.callback = audio_callback;
	wavSpec.userdata = NULL;

  audio_position = wavBuffer;
	audio_length = wavLength;

  deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0); //NULL means default
}

static void Init() {

  #if defined (NXDK)
  XVideoSetMode(640, 480, 32, REFRESH_DEFAULT);
  debugClearScreen();
  #endif

  SDL_Init(SDL_INIT_AUDIO|SDL_INIT_JOYSTICK);
}


static int FileBrowser() {

  #if defined (NXDK)
  char *totalDirsFiles[5] = { NULL };
  size_t currentFileDirCount = 0;
  WIN32_FIND_DATA findFileData;
  HANDLE hFind;

  while (true)
  {
    XVideoWaitForVBlank();
    SDL_GameControllerUpdate();
    debugClearScreen();

    printf("Supported files on D:\n");
    hFind = FindFirstFile("D:\\*.wav", &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
    printf("FindFirstHandle() failed!\n");
    return 1;
    }

    do {
      if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        currentFileDirCount++;
        printf("Directory: ");
      } 
      else {
        currentFileDirCount++;
        printf("File: ");
      }
      printf("%s\n", findFileData.cFileName);
    } 
    while (FindNextFile(hFind, &findFileData) != 0);

    realloc(totalDirsFiles, currentFileDirCount);
    printf("\n");

    DWORD error = GetLastError();
    if (error == ERROR_NO_MORE_FILES) {
      printf("Total number of files and directories: %d\n", currentFileDirCount);
    } 
    else {
      printf("Error: %x\n", error);
    }
      if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
      {
        FindClose(hFind);
        break;
      }
  }
  #endif

  return 0;
}

int main()
{
  Init();

  //Open controller
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

  #if defined (NXDK)
  if (FileBrowser() == 1)
  {
    while (true)
    {
      printf("Error! Halting execution!");
    }
    return 1;
  }
  #endif

  PlayFile();

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
      
  }

  Quit(deviceID, wavBuffer);
  return 0;
}
