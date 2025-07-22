#include "game.h"

Entity* createPlayer() {
  Entity* player = Djinni.Renderable->Sprite->create(800, 600, "bin/gfx/player.png");
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

  if (game->keyboard[SDL_SCANCODE_SPACE] == 1 && dt - pdt > 150 ) {
    createPlayerBullet(entity, game);
    pdt = dt;
  }
}