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

// Any other main prototype will bug MinGW's SDL2
// Doesn't matter on *nix
int main(int argc, char **argv) {
  #ifdef NXDK
  //Init Xbox video
  XVideoSetMode(640, 480, 32, REFRESH_60HZ);
  #endif
  // Init the structures
  Sdl *sdl = init_sdl();
  if (sdl_init == false)
  {
    printf("Couldn't initialize SDL");
  }
  else
  {
    printf("SDL initialized");
  }
  Fractal *fractal = init_fractal();
  if (sdl_init == false)
  {
    printf("Couldn't initialize fractal");
  }
  else
  {
    printf("Fractal initialized");
  }
  

  for (int i = 0; i < SDL_NumJoysticks(); i++) { // For the time that i is smaller than the number of connected Joysticks

    if(SDL_IsGameController(i)) { // If i (which we use to iterate through the connected controllers) as a port number is a Game Controller
      controller = SDL_GameControllerOpen(i); // Open the controller
 
      if(controller) { // If we find that we opened a controller
        printf("Opened controller");
        break; // Exit the loop
      }
                 
    }
  }


  while (true) {
    draw_mandelbrot(sdl, fractal);
    is_user_moving(sdl, fractal);

    SDL_RenderPresent(sdl->renderer);

    #ifndef NXDK
    SDL_Delay(10);
    #endif
  }
  return 0;
}
