#include "game.h"

void enemyLeftViewport(Entity* e, Game* g, double dt) {
  e->status = ENTITY_DESTORYED;
  e->body.velocity.dx = 0;
  printf("enemyLeftViewport: %p\n", e);
}

void onEnemyCollide(Entity* self, Entity* other, Game* g, double dt) {

}

Entity* createEnemy(ViewportBounds bounds) {
  int y = (rand() % (bounds.y2 - 160 - bounds.y1 + 160)) + bounds.y1;

  Entity* enemy = Djinni.Renderable->Sprite->create(bounds.x2, y, "bin/gfx/enemy.png");
  enemy->onExitViewport = enemyLeftViewport;

  //enemy->update = playerUpdate;
  enemy->onCollide = onEnemyCollide;

  enemy->body.velocity.dx = -(2 + (rand() % 4));

  printf("Created enemy: %p at %d / %d \n", enemy, bounds.x2, y);

  return enemy;
}
