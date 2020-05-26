#if defined (NXDK)
#include <string.h>
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
static Uint32 audio_length = 0;
Uint8 *wavBuffer = NULL;
char fileToPlay[140];
SDL_AudioDeviceID deviceID = 0;
SDL_GameController *controller = NULL;

void audio_callback(void *userdata, Uint8 *stream, int len) {
	
	if (audio_length ==0)
		return;
	
	len = (len > audio_length ? audio_length : len);
	SDL_memcpy(stream, audio_position, len); // simply copy from one buffer into the other
	
	audio_position += len;
	audio_length -= len;
}

static void Quit(SDL_AudioDeviceID deviceID, Uint8 *wavBuffer) {
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
        return false; // The button isn't pressed at all, so we return false
    }
}

float remap(float value, float from_min, float from_max, float to_min, float to_max) { // Function to remap values to other values
  value = ((value - from_min) / (from_max - from_min)) * (to_max - to_min) + to_min;
  return value;
}

static float getAxis(int sdl_axis) { // Function to get an axis from the controller

  const float deadzone = 0.2f;

  // Get input in range -1 to +1
  float amount = (float)SDL_GameControllerGetAxis(controller, sdl_axis) / (float)0x8000;

  // Reject if the stick is in deadzone
  if (fabsf(amount) < deadzone) {
    return 0.0f;
  }

  if (amount > 0.0f) {
  amount = remap(amount,  deadzone, +1.0f,  0.0f, +1.0f); // Remap from +[0.2, 1.0] to +[0.0, 1.0]
  } 
  else {
  amount = -remap(-amount,  deadzone, +1.0f,  0.0f, +1.0f); // Remap from -[0.2, 1.0] to -[0.0, 1.0]
  }
  return amount;
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

#if defined (NXDK)
typedef struct
{
  int fileIndex;
  char fileName[50];
}file;


static int FileBrowser() {
  WIN32_FIND_DATA findFileData;
  HANDLE hFind;

  file foundFiles[50];
  int currentIndex = 0;

  size_t currentFileDirCount = 0;

  hFind = FindFirstFileA("D:\\*.wav", &findFileData);

  do {
    XVideoWaitForVBlank();
    debugClearScreen();
    if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      printf("Directory: ");
    } 
    else {
      printf("File: ");
    }
    foundFiles[currentFileDirCount].fileIndex = currentFileDirCount;
    strcpy(foundFiles[currentFileDirCount].fileName, findFileData.cFileName);
    currentFileDirCount++;
  } 
  while (FindNextFileA(hFind, &findFileData) != 0);

  while (true)
  {
    XVideoWaitForVBlank();
    SDL_GameControllerUpdate();
    debugClearScreen();
    
    for (int i = 0; i < currentFileDirCount; i++)
    {
      printf("%d ", foundFiles[i].fileIndex);
      printf("%s\n",foundFiles[i].fileName);
    }

    DWORD error = GetLastError();
    if (error == ERROR_NO_MORE_FILES) {
      printf("\nTotal number of files and directories: %d\n", currentFileDirCount);
    } 
    else {
      printf("Error: %x\n", error);
    }
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
      if (currentIndex != 0) {
        currentIndex--;
      } 
      else {
        currentIndex = 0;
      }
    }
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP)) {
      if (currentIndex != currentFileDirCount - 1) {
        currentIndex++;
      } 
      else {
        currentIndex = currentFileDirCount - 1;
      }
    }
    

    printf("\nYour current selected file is: %s (Index number %d)\n", foundFiles[currentIndex].fileName, currentIndex);
    if (SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A)) {
      strcpy(fileToPlay, foundFiles[currentIndex].fileName);
      FindClose(hFind);
      break;
    }
  }

  return 0;
}
#endif

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
  #else
  printf("Tell me the path to the file that you want to play: ");
  
  scanf("%[^\t\n]", fileToPlay);
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
  #if defined(NXDK)
  XReboot();
  #endif
  return 0;
}
