// This file has no copyright.

#ifndef HEADERS_MYSTRUCTURES_H_
#define HEADERS_MYSTRUCTURES_H_
#if defined(NXDK)
#include <SDL.h>
#include <SDL_gamecontroller.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#endif

// This has to be a square
#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480

#define WINDOW_TITLE "Mandelbrot Fractal, by Geographer (Xbox port by Kosmas12)"

typedef struct Sdl {
  SDL_Window *window;
  SDL_Surface *surface;
  SDL_PixelFormat *format;
  SDL_Event event;
} Sdl;

typedef struct Complex {
  // Real part, imaginary part and a backup
  double r;
  double i;
  double b;
} Complex;

typedef struct Fractal {
  // See myFractal.h
  double xMove;
  double yMove;
  double zoom;
  unsigned int iMax;
} Fractal;

void free_everything(Sdl *sdl, Fractal *fractal) {

  SDL_DestroyWindow(sdl->window);
  free(sdl);

  free(fractal);

  SDL_Quit();

  // You may observe memory leaks using this program
  // SDL2 leaks by itself
}

#endif  // HEADERS_MYSTRUCTURES_H_
