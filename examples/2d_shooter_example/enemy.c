#include "game.h"

void enemyLeftViewport(Entity* e, Game* g, double dt) {
  e->status = ENTITY_DESTORYED;
  e->body.velocity.dx = 0;
}

void onEnemyDestroy(Entity* e, Game* g, double dt) {
  free(e->data);
}

void onEnemyCollide(Entity* self, Entity* other, Game* g, double dt) {}

void enemyUpdate(Entity* entity, Game* game, double dt) {
  EntityData* data = entity->data;
  if (--data->reload <= 0) {
    createEnemyBullet(entity, player, game);
    data->reload = rand() % 500 * 2;
  }
}

Entity* createEnemy(ViewportBounds bounds) {
  int y = (rand() % (bounds.y2 - 160 - bounds.y1 + 160)) + bounds.y1;

  Entity* enemy = Djinni.Renderable->Sprite->create(bounds.x2, y, "bin/gfx/enemy.png");
  enemy->onExitViewport = enemyLeftViewport;

  EntityData* data = malloc(sizeof(EntityData));
  data->type = ENEMY_SHIP;
  data->reload = rand() % 500 * 2;
  enemy->data = data;

  enemy->update = enemyUpdate;
  enemy->onCollide = onEnemyCollide;
  enemy->onDestroy = onEnemyDestroy;

  enemy->body.velocity.dx = -(2 + (rand() % 4));

  return enemy;
}
