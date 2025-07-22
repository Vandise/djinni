#include "game.h"

void enemyLeftViewport(Entity* e, Game* g, double dt) {
  e->status = ENTITY_DESTORYED;
  e->body.velocity.dx = 0;
  printf("enemy left viewport %p\n", e);
}

Entity* createEnemy(ViewportBounds bounds) {
  int y = (rand() % (bounds.y2 + 60 - bounds.y1 + 60)) + bounds.y1;

  Entity* enemy = Djinni.Renderable->Sprite->create(bounds.x2, y, "bin/gfx/enemy.png");
  enemy->onExitViewport = enemyLeftViewport;

  //enemy->update = playerUpdate;
  //player->onCollide = onPlayerCollide;

  enemy->body.velocity.dx = -(2 + (rand() % 4));

  return enemy;
}
