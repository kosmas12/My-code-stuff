// This file has no copyright.

#ifndef HEADERS_MYFRACTAL_H_
#define HEADERS_MYFRACTAL_H_

#if defined(NXDK)
#include <SDL.h>
#include <SDL_gamecontroller.h>
#else
#include <SDL2/SDL.h>
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
  for (x = 0; x < xFrame; x++) {
    c.r = ((x - xFrame / 2) / (0.5 * xFrame * fractal->zoom)) - fractal->xMove;

    for (y = 0; y < yFrame; y++) {
      c.i = ((y - yFrame / 2) /
        (0.5 * yFrame * fractal->zoom)) - fractal->yMove;

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
        SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 255, 255);
        SDL_RenderDrawPoint(sdl->renderer, x, y);
      } else {
        // Not in the set
        SDL_SetRenderDrawColor(sdl->renderer, 0, 0, i * (255 / fractal->iMax),
          255);
        SDL_RenderDrawPoint(sdl->renderer, x, y);
      }

      // Render using SDL_RenderDrawPoint() is slow and should
      // be replaced by SDL_RenderDrawPoints()
    }
  }
}

void print_verbose(Fractal *fractal) {
  /* // Print some variables on console
  // We need to know the OS
  // In order to run the right command
  #ifdef __unix__
    system("clear");
  #elif defined(_WIN32) || defined(_WIN64)
    system("cls");
  #endif
  printf(" ~ x position :\t%f\n", fractal->xMove);
  printf(" ~ y position :\t%f\n", fractal->yMove);
  printf(" ~ zoom :\t%f\n", fractal->zoom);
  printf(" ~ iterations :\t%f\n", fractal->iMax); */
}


void is_user_moving(Sdl *sdl, Fractal *fractal) {

  // Delta time to sync everything
  float delta = 0.30;

  // Movement speed
  float moveStep = 0.5;
  float zoomStep = 3.0;

  bool is_left_analog_left = where_is_xaxis() < -0.5f; // Initialize a boolean to check if Xamount is lesser than -0.5. If so, the left analog is left

  bool is_left_analog_right = where_is_xaxis() > 0.5f; // Initialize a boolean to check if Xamount is greater than 0.5. If so, the left analog is right

  bool was_left_analog_left = false;

  bool was_left_analog_right = false;

  bool is_left_analog_down = -where_is_yaxis() < -0.5f;

  bool is_left_analog_up = -where_is_yaxis() > 0.5f; 

  bool was_left_analog_down = false;

  bool was_left_analog_up = false;

  bool is_left_trigger_down = where_is_tlaxis() < -0.5f; 

  bool is_right_trigger_down = where_is_traxis() < -0.5f;

  bool was_left_trigger_down = false;

  bool was_right_trigger_down = false;

  // Everything is adapted to current zoom
  if (isNewlyPressed(is_left_analog_left, &was_left_analog_left)) {
    fractal->xMove = fractal->xMove + (moveStep / fractal->zoom * delta);
    draw_cross(sdl);
    print_verbose(fractal);
  } else if (isNewlyPressed(is_left_analog_right, &was_left_analog_right)) {
    fractal->xMove = fractal->xMove - (moveStep / fractal->zoom * delta);
    draw_cross(sdl);
    print_verbose(fractal);
  } else if (isNewlyPressed(is_left_analog_up, &was_left_analog_up)) {
    fractal->yMove = fractal->yMove + (moveStep / fractal->zoom * delta);
    draw_cross(sdl);
    print_verbose(fractal);
  } else if (isNewlyPressed(is_left_analog_down, &was_left_analog_down)) {
    fractal->yMove = fractal->yMove - (moveStep / fractal->zoom * delta);
    draw_cross(sdl);
    print_verbose(fractal);
  } else if (isNewlyPressed(is_right_trigger_down, &was_right_trigger_down)) {
    fractal->zoom = fractal->zoom + (moveStep * fractal->zoom * delta);
    fractal->iMax = fractal->iMax + zoomStep * delta;
    draw_cross(sdl);
    print_verbose(fractal);
  } else if (isNewlyPressed(is_left_trigger_down, &was_left_trigger_down)
    // User is not allowed to zoom back past 0.3
    && (fractal->zoom - (moveStep * fractal->zoom * delta)) > 0.3) {
    fractal->zoom = fractal->zoom - (moveStep * fractal->zoom * delta);
    fractal->iMax = fractal->iMax - zoomStep * delta;
    draw_cross(sdl);
    print_verbose(fractal);
  }
}

#endif  // HEADERS_MYFRACTAL_H_
