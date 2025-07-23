#include "game.h"

void onPlayerCollide(Entity* self, Entity* other, Game* g, double dt) {}

void onPlayerDestroy(Entity* e, Game* g, double dt) {
  free(e->data);
  g->terminated = 1;
}

Entity* createPlayer() {
  Entity* player = Djinni.Renderable->Sprite->create(800, 600, "bin/gfx/player.png");
  EntityData* data = malloc(sizeof(EntityData));
  data->type = PLAYER_SHIP;
  player->data = data;

  player->update = playerUpdate;
  player->onCollide = onPlayerCollide;
  player->onDestroy = onPlayerDestroy;

  return player;
}

static double pdt = 0;

void playerUpdate(Entity* entity, Game* game, double dt) {
  entity->body.velocity.dy = entity->body.velocity.dx = 0;

  if (game->keyboard[SDL_SCANCODE_W] == 1) {
    entity->body.velocity.dy = -3;
  }

  if (game->keyboard[SDL_SCANCODE_A] == 1) {
    entity->body.velocity.dx = -3;
  }

  if (game->keyboard[SDL_SCANCODE_S] == 1) {
    entity->body.velocity.dy = 3;
  }

  if (game->keyboard[SDL_SCANCODE_D] == 1) {
    entity->body.velocity.dx = 3;
  }

  if (game->keyboard[SDL_SCANCODE_SPACE] == 1 && dt - pdt > 150 ) {
    createPlayerBullet(entity, game);
    pdt = dt;
  }
}