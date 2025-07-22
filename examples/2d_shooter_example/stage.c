#include "game.h"

static int enemySpawnTimer = 0;

void onStageCreate(Stage* self, Game* game, Stage* previous) {
  Djinni.Game->World->addEntity(game->world, createPlayer());

  game->camera->point.x += 400;
  game->camera->point.y += 400;

  Djinni.Game->enableInput(game);
}

void prepareStage(Stage* self, Game* game) {}

void updateStage(Stage* self, Game* game, double dt) {
  if (--enemySpawnTimer <= 0) {
    ViewportBounds bounds = Djinni.Game->Camera->getViewportBounds(game->camera);
    Entity* enemy = createEnemy(bounds);

    Djinni.Game->World->addEntity(game->world, enemy);

    enemySpawnTimer = 30 + (rand() % 180);
  }
}

void drawStage(Stage* self, Game* game, double dt) {}
void onDestroyStage(Stage* self, Game* game, Stage* next) {}