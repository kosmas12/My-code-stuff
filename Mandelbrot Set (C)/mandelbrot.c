#if defined (NXDK)
#include <hal/debug.h>
#include <hal/video.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#endif
#include "headers/myStructures.h"
#include "headers/mySDL.h"
#include "headers/myFractal.h"
#include <assert.h>

// Any other main prototype will bug MinGW's SDL2
// Doesn't matter on *nix
int main(int argc, char **argv) {

  bool joy_open = false;

  float delta = 0.30;

  float moveStep = 0.5;
  float zoomStep = 3.0;

  #ifdef NXDK
  //Init Xbox video
  XVideoSetMode(640, 480, 32, REFRESH_60HZ);
  #endif
  // Init the structures
  Sdl *sdl = init_sdl();

  if (sdl_init == false)
  {
    fprintf(stderr, "Couldn't initialize SDL\n");
  }
  else
  {
    fprintf(stdout, "SDL initialized\n");
  }
  Fractal *fractal = init_fractal();
  if (sdl_init == false)
  {
    fprintf(stderr, "Couldn't initialize fractal\n");
  }
  else
  {
    fprintf(stdout, "Fractal initialized\n");
  }
  
  for (int i = 0; i < SDL_NumJoysticks(); i++) { // For the time that i is smaller than the number of connected Joysticks

    if(SDL_IsGameController(i)) { // If i (which we use to iterate through the connected controllers) as a port number is a Game Controller
      controller = SDL_GameControllerOpen(i); // Open the controller

      if(controller) { // If we find that we opened a controller
        fprintf(stdout, "Opened %s in port %d\n", SDL_GameControllerName(controller), i);
        joy_open = true;
        break; // Exit the loop
        }
      }
  }

  if (!joy_open)
  {
    fprintf(stderr, "Couldn't open a joystick.\n");
  }
  


  while (true) {
    draw_mandelbrot(sdl, fractal);
    is_user_moving(sdl, fractal);
    draw_mandelbrot(sdl, fractal);
  }
  return 0;
}