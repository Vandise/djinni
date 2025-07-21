#include "djinni/djinni.h"

Entity* createPlayer();
void onPlayerCollide(Entity* self, Entity* other, Game* g, double dt);
void playerUpdate(Entity* entity, Game* game, double dt);

void onStageCreate(Stage* self, Game* game, Stage* previous);
void prepareStage(Stage* self, Game* game);
void updateStage(Stage* self, Game* game, double dt);
void drawStage(Stage* self, Game* game, double dt);
void onDestroyStage(Stage* self, Game* game, Stage* next);