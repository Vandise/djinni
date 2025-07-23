#include "game.h"

WorldMap* wm = NULL;

void onStageCreate(Stage* self, Game* game, Stage* previous) {
  wm = Djinni.Map->create(2560, 1440, 62, 61, ISOMETRIC_MAP_TYPE);
  Djinni.Map->load(wm, Djinni.renderer);
  //Djinni.Map->destroy(wm);
  Djinni.Game->enableInput(game);
}

void prepareStage(Stage* self, Game* game) {}

void updateStage(Stage* self, Game* game, double dt) {
}

void drawStage(Stage* self, Game* game, double dt) {
  Djinni.Map->draw(wm, Djinni.renderer, dt);
}

void onDestroyStage(Stage* self, Game* game, Stage* next) {}