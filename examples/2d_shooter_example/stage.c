#include "game.h"

void onStageCreate(Stage* self, Game* game, Stage* previous) {

  Djinni.Game->World->addEntity(game->world, createPlayer());

  Djinni.Game->enableInput(game);
}

void prepareStage(Stage* self, Game* game) {}
void updateStage(Stage* self, Game* game, double dt) {}
void drawStage(Stage* self, Game* game, double dt) {}
void onDestroyStage(Stage* self, Game* game, Stage* next) {}