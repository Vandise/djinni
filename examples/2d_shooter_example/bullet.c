#include "game.h"

void bulletLeftViewport(Entity* e, Game* g, double dt) {
  e->status = ENTITY_DESTORYED;
}

void onBulletDestroy(Entity* e, Game* g, double dt) {
  free(e->data);
}

void onBulletCollide(Entity* self, Entity* other, Game* g, double dt) {
  EntityData* data = self->data;
  EntityData* otherData = other->data;

  if (
    data->type == PLAYER_BULLET &&
    (otherData->type == ENEMY_SHIP || otherData->type == ENEMY_BULLET)
  ) {
    self->status = ENTITY_DESTORYED;
    other->status = ENTITY_DESTORYED;
    return;
  }

  if (
    data->type == ENEMY_BULLET &&
    (otherData->type == PLAYER_SHIP || otherData->type == PLAYER_BULLET)
  ) {
    self->status = ENTITY_DESTORYED;
    other->status = ENTITY_DESTORYED;
    return;
  }
}

void createPlayerBullet(Entity* player, Game* game) {
  Coordinate pos = Djinni.Renderable->Entity->getPosition(player);

  Entity* bullet = Djinni.Renderable->Sprite->create(pos.x + 40, pos.y, "bin/gfx/playerBullet.png");
  EntityData* data = malloc(sizeof(EntityData));
  data->type = PLAYER_BULLET;

  bullet->data = data;

  bullet->onExitViewport = bulletLeftViewport;
  bullet->onDestroy = onBulletDestroy;
  bullet->onCollide = onBulletCollide;
  bullet->body.velocity.dx = 10;

  Djinni.Game->World->addEntity(game->world, bullet);
}

void createEnemyBullet(Entity* enemy, Entity* player, Game* game) {
  Coordinate pos = Djinni.Renderable->Entity->getPosition(player);
  Coordinate epos = Djinni.Renderable->Entity->getPosition(enemy);

  Entity* bullet = Djinni.Renderable->Sprite->create(epos.x + 40, epos.y, "bin/gfx/alienBullet.png");
  EntityData* data = malloc(sizeof(EntityData));
  data->type = ENEMY_BULLET;

  bullet->data = data;

  bullet->onExitViewport = bulletLeftViewport;
  bullet->onDestroy = onBulletDestroy;
  bullet->onCollide = onBulletCollide;

  LineSlope slope = Djinni.Geometry->Line->slope(pos, epos);

  bullet->body.velocity.dx = slope.dx * 6;
  bullet->body.velocity.dy = slope.dy * 6;

  Djinni.Game->World->addEntity(game->world, bullet);
}
