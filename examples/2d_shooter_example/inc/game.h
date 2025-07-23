#ifndef __GAME_H
#define __GAME_H 1

#include "djinni/djinni.h"

typedef enum {
  PLAYER_BULLET,
  ENEMY_BULLET,
  PLAYER_SHIP,
  ENEMY_SHIP
} GAME_ENTITY_TYPE;

typedef struct EntityDataStruct {
  GAME_ENTITY_TYPE type;
  int reload;
} EntityData;

extern Entity* player;

Entity* createPlayer();
void onPlayerCollide(Entity* self, Entity* other, Game* g, double dt);
void playerUpdate(Entity* entity, Game* game, double dt);

Entity* createEnemy(ViewportBounds bounds);

void onStageCreate(Stage* self, Game* game, Stage* previous);
void prepareStage(Stage* self, Game* game);
void updateStage(Stage* self, Game* game, double dt);
void drawStage(Stage* self, Game* game, double dt);
void onDestroyStage(Stage* self, Game* game, Stage* next);

void bulletLeftViewport(Entity* e, Game* g, double dt);
void createPlayerBullet(Entity* player, Game* game);
void createEnemyBullet(Entity* enemy, Entity* player, Game* game);

#endif