#include "game.h"

void bulletLeftViewport(Entity* e, Game* g, double dt) {
  e->status = ENTITY_DESTORYED;
}

void createBullet(Entity* player, Game* game) {
  Coordinate pos = Djinni.Renderable->Entity->getPosition(player);

  Entity* bullet = Djinni.Renderable->Sprite->create(pos.x + 20, pos.y, "bin/gfx/playerBullet.png");
  bullet->onExitViewport = bulletLeftViewport;
  bullet->body.velocity.dx = 10;

  Djinni.Game->World->addEntity(game->world, bullet);
}

Entity* createPlayer() {
  Entity* player = Djinni.Renderable->Sprite->create(1150, 400, "bin/gfx/player.png");
  player->update = playerUpdate;
  //player->onCollide = onPlayerCollide;

  return player;
}

void onPlayerCollide(Entity* self, Entity* other, Game* g, double dt) {
  printf("Player collides with: %p\n", other);
}

static double pdt = 0;

void playerUpdate(Entity* entity, Game* game, double dt) {
  entity->body.velocity.dy = entity->body.velocity.dx = 0;

  if (game->keyboard[SDL_SCANCODE_W] == 1) {
    entity->body.velocity.dy = -2;
  }

  if (game->keyboard[SDL_SCANCODE_A] == 1) {
    entity->body.velocity.dx = -2;
  }

  if (game->keyboard[SDL_SCANCODE_S] == 1) {
    entity->body.velocity.dy = 2;
  }

  if (game->keyboard[SDL_SCANCODE_D] == 1) {
    entity->body.velocity.dx = 2;
  }

  if (game->keyboard[SDL_SCANCODE_SPACE] == 1 && dt - pdt > 250 ) {
    createBullet(entity, game);
    pdt = dt;
  }
}