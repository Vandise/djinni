#include "game.h"

void bulletLeftViewport(Entity* e, Game* g, double dt) {
  e->status = ENTITY_DESTORYED;
}

void createPlayerBullet(Entity* player, Game* game) {
  Coordinate pos = Djinni.Renderable->Entity->getPosition(player);

  Entity* bullet = Djinni.Renderable->Sprite->create(pos.x + 40, pos.y, "bin/gfx/playerBullet.png");
  bullet->onExitViewport = bulletLeftViewport;
  bullet->body.velocity.dx = 10;

  Djinni.Game->World->addEntity(game->world, bullet);
}

void createEnemyBullet(Entity* enemy, Game* game) {
  Coordinate pos = Djinni.Renderable->Entity->getPosition(enemy);
  

  Entity* bullet = Djinni.Renderable->Sprite->create(pos.x + 40, pos.y, "bin/gfx/playerBullet.png");
  bullet->onExitViewport = bulletLeftViewport;
  bullet->body.velocity.dx = 10;

  Djinni.Game->World->addEntity(game->world, bullet);
}
