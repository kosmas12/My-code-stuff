// This file has no copyright.

#ifndef HEADERS_MYSDL_H_
#define HEADERS_MYSDL_H_

#if defined(NXDK)
#include <SDL.h>
#include <SDL_gamecontroller.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#endif
#include <stdbool.h>

bool sdl_init = false;

SDL_GameController *controller = NULL; // We initialize a controller and give it a NULL value for now

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

Sdl *init_sdl() {
  
  Sdl *sdl = malloc(sizeof(Sdl));

  SDL_Init(SDL_INIT_GAMECONTROLLER|SDL_INIT_VIDEO);

  sdl->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

  sdl->surface = SDL_GetWindowSurface(sdl->window);

  #if !defined(NXDK)
  assert(sdl->surface != NULL);
  #endif
  
  sdl_init = true;

  return sdl;
}


/*
void draw_cross(Sdl *sdl) {
  // Draw a little cross when user zooms in/out or moves
  int length = 15;
  SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);

  SDL_RenderDrawLine(sdl->renderer, WINDOW_WIDTH / 2 - length / 2,
    WINDOW_HEIGHT / 2, WINDOW_WIDTH / 2 + length / 2, WINDOW_HEIGHT / 2);
  SDL_RenderDrawLine(sdl->renderer, WINDOW_WIDTH / 2, WINDOW_HEIGHT
    / 2 - length / 2, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + length / 2);
} */

#endif  // HEADERS_MYSDL_H_
