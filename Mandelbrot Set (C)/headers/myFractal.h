// This file has no copyright.

#ifndef HEADERS_MYFRACTAL_H_
#define HEADERS_MYFRACTAL_H_

#if defined(NXDK)
#include <SDL.h>
#include <SDL_gamecontroller.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#endif
#include "myStructures.h"
#include "mySDL.h"

bool fractal_init = false;

Fractal *init_fractal() {
  Fractal *fractal = malloc(sizeof(Fractal));

  // Used to move camera
  fractal->xMove = 0;
  fractal->yMove = 0;

  // Used to change the zoom and precision
  fractal->zoom = 0.3;
  fractal->iMax = 60;

  fractal_init = true;

  return fractal;
} 

void draw_mandelbrot(Sdl *sdl, Fractal *fractal) {
  int i;

  SDL_LockSurface(sdl->surface);

  uint32_t *pixels = sdl->surface->pixels;
  SDL_PixelFormat *pixelFormat = sdl->surface->format;

  int xFrame = WINDOW_WIDTH;
  int yFrame = WINDOW_HEIGHT;

  // Formula is Z(n+1) = Z(n)^2 + C
  // https://en.wikipedia.org/wiki/Mandelbrot_set
  Complex c;
  Complex z;

  // Coordinate of each point
  int x;
  int y;

  // Calculate all the y for every x
  for (y = 0; y < yFrame; y++) {
    c.i = ((y - yFrame / 2) / (0.5 * yFrame * fractal->zoom)) - fractal->yMove;

    for (x = 0; x < xFrame; x++) {
      c.r = ((x - xFrame / 2) / (0.5 * xFrame * fractal->zoom)) - fractal->xMove;

      z.r = 0;
      z.i = 0;

      i = 0;


      // Iterate in order to know if a certain point is in the set or not
      do {
        z.b = z.r;
        z.r = z.r * z.r - z.i * z.i + c.r;
        z.i = 2 * z.i * z.b + c.i;
        i++;
      } while (z.r * z.r + z.i * z.i < 4 && i < fractal->iMax);
      // We don't use square root in order to reduce calculation time

       if (i >= fractal->iMax) {
        // In the set
        pixels[(y * xFrame + x)] = SDL_MapRGB(pixelFormat, 0, 0, 255);
      } else {
        // Not in the set
        pixels[(y * xFrame + x)] = SDL_MapRGB(pixelFormat, 0, 0, (i * (255 / fractal->iMax)));
      }

      // Render using SDL_RenderDrawPoint() is slow and should
      // be replaced by SDL_RenderDrawPoints()
    }
  }
  SDL_UnlockSurface(sdl->surface);
  SDL_UpdateWindowSurface(sdl->window);
  printf("Left drawing state\n");
}


void is_user_moving(Sdl *sdl, Fractal *fractal) {

  // Delta time to sync everything
  float delta = 0.30;

  // Movement speed
  float moveStep = 0.5;
  float zoomStep = 3.0;

  float Xamount = getAxis(SDL_CONTROLLER_AXIS_LEFTX); // Xamount is the amount returned by getAxis for the X axis of the left analog stick

  float Yamount = -getAxis(SDL_CONTROLLER_AXIS_LEFTY); // Yamount is -getAxis() because of inverted Y axis in SDL
  
  bool is_left_analog_left = Xamount < -0.5f;

  bool is_left_analog_right = Xamount > 0.5f;

  bool was_left_analog_left = false;

  bool was_left_analog_right = false;

  bool is_left_analog_down = Yamount < -0.5f;

  bool is_left_analog_up = Yamount > 0.5f; 

  bool was_left_analog_down = false;

  bool was_left_analog_up = false;

  bool is_x_down = true;

  bool is_b_down = true;

  bool was_x_down = false;

  bool was_b_down = false;

  if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X))
  {
    is_x_down = true;
  }
  else
  {
    is_x_down = false;
  }

  if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
  {
    is_b_down = true;
  }
  else
  {
    is_b_down = false;
  }

  // Everything is adapted to current zoom
  if (isNewlyPressed(is_left_analog_left, &was_left_analog_left)) {
    fractal->xMove = fractal->xMove + (moveStep / fractal->zoom * delta);
  } else if (isNewlyPressed(is_left_analog_right, &was_left_analog_right)) {
    fractal->xMove = fractal->xMove - (moveStep / fractal->zoom * delta);
  } else if (isNewlyPressed(is_left_analog_up, &was_left_analog_up)) {
    fractal->yMove = fractal->yMove + (moveStep / fractal->zoom * delta);
  } else if (isNewlyPressed(is_left_analog_down, &was_left_analog_down)) {
    fractal->yMove = fractal->yMove - (moveStep / fractal->zoom * delta);
  } else if (isNewlyPressed(is_b_down, &was_b_down)) {
    fractal->zoom = fractal->zoom + (moveStep * fractal->zoom * delta);
    fractal->iMax = fractal->iMax + zoomStep * delta;
  } else if (isNewlyPressed(is_x_down, &was_x_down) && (fractal->zoom - (moveStep * fractal->zoom * delta)) > 0.3) {
    fractal->zoom = fractal->zoom - (moveStep * fractal->zoom * delta);
    fractal->iMax = fractal->iMax - zoomStep * delta;
  }
} 

#endif  // HEADERS_MYFRACTAL_H_
