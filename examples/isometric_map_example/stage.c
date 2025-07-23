#include "game.h"

WorldMap* wm = NULL;

void onStageCreate(Stage* self, Game* game, Stage* previous) {
  wm = Djinni.Map->create(2560, 1440, 64, 64, ISOMETRIC_MAP_TYPE);
  Djinni.Map->load(wm, Djinni.renderer);
  //Djinni.Map->destroy(wm);
  Djinni.Game->enableInput(game);
}

void prepareStage(Stage* self, Game* game) {}

void updateStage(Stage* self, Game* game, double dt) {
  if (game->keyboard[SDL_SCANCODE_W] == 1) {
    game->camera->point.y -= 3;
  }

  if (game->keyboard[SDL_SCANCODE_A] == 1) {
    game->camera->point.x -= 3;
  }

  if (game->keyboard[SDL_SCANCODE_S] == 1) {
    game->camera->point.y += 3;
  }

  if (game->keyboard[SDL_SCANCODE_D] == 1) {
    game->camera->point.x += 3;
  }
}

void drawStage(Stage* self, Game* game, double dt) {
  Djinni.Map->draw(wm, Djinni.renderer, game->camera, dt);
}

void onDestroyStage(Stage* self, Game* game, Stage* next) {}