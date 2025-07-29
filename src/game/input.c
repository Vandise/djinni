#include <stdlib.h>
#include <SDL2/SDL.h>
#include "djinni/game/input.h"
#include "djinni/game/game.h"

static Djinni_GameIO* game_io = NULL;

static void key_up(SDL_KeyboardEvent *event) {
	if (event->repeat == 0 && event->keysym.scancode < MAX_IO_KEY_CODE) {
		game_io->keyboard[event->keysym.scancode] = 0;
	}
}

static void key_down(SDL_KeyboardEvent *event) {
	if (event->repeat == 0 && event->keysym.scancode < MAX_IO_KEY_CODE) {
		game_io->keyboard[event->keysym.scancode] = 1;
	}
}

void djinni_game_input_initialize() {
  game_io = malloc(sizeof(Djinni_GameIO));
  game_io->enabled = 0;

  for (int i = 0; i < MAX_IO_KEY_CODE; i++) {
    game_io->keyboard[i] = 0;
  }
}

void djinni_game_input_process() {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      djinni_game_terminate();
      break;
    }

    if (event.type == SDL_KEYDOWN) {
      key_down(&event.key);
      continue;
    }

    if (event.type == SDL_KEYUP) {
      key_up(&event.key);
    }
  }
}

inline int djinni_game_input_key_pressed(int key) {
  return game_io->keyboard[key];
}

inline int djinni_game_input_enabled() {
  return game_io->enabled;
}

inline void djinni_game_input_enable() {
  game_io->enabled = 1;
}

inline void djinni_game_input_disable() {
  game_io->enabled = 0;
}

void djinni_game_input_destroy() {
  free(game_io);
}