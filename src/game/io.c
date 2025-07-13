#include <SDL2/SDL.h>
#include "djinni/util/util.h"
#include "djinni/game/game.h"

static void keyUp(Game* game, SDL_KeyboardEvent *event) {
	if (event->repeat == 0 && event->keysym.scancode < MAX_IO_KEY_CODE) {
		game->keyboard[event->keysym.scancode] = 0;
	}
}

static void keyDown(Game* game, SDL_KeyboardEvent *event) {
	if (event->repeat == 0 && event->keysym.scancode < MAX_IO_KEY_CODE) {
		game->keyboard[event->keysym.scancode] = 1;
	}
}

static void process(Game* game) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {

  	if (!game->inputEnabled) { continue; }

		switch (event.type) {
			case SDL_QUIT:
        game->terminated = 1;
				break;
			case SDL_KEYDOWN:
				keyDown(game, &event.key);
				break;
			case SDL_KEYUP:
				keyUp(game, &event.key);
				break;
			default:
				break;
		}
	}
}

struct Djinni_Game_InputStruct Djinni_Game_Input = {
  .process = process
};