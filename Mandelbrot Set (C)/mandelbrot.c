#if defined (NXDK)
#include <SDL.h>
#include <SDL_gamecontroller.h>
#include <hal/debug.h>
#include <hal/video.h>
#include <windows.h>
#define printf(...) debugPrint(__VA_ARGS__)
#else
#include <SDL2/SDL.h>
#endif
#include "headers/mySDL.h"
#include "headers/myStructures.h"
#include "headers/myFractal.h"

// Any other main prototype will bug MinGW's SDL2
// Doesn't matter on *nix
int main(int argc, char **argv) {
  //Initialize Xbox video
  XVideoSetMode(640, 480, 32, REFRESH_60HZ);
  // Init the structures
  Sdl *sdl = init_sdl();
  Fractal *fractal = init_fractal();

  // Init console
  print_verbose(fractal);

  // User can exit program using escape
  while (is_user_pressing_escape(sdl) == 0) {
    draw_mandelbrot(sdl, fractal);
    is_user_moving(sdl, fractal);

    SDL_RenderPresent(sdl->renderer);

    SDL_Delay(10);
  }

  free_everything(sdl, fractal);
  return 0;
}
