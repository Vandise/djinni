#ifndef __GAME_H
#define __GAME_H 1

#include "djinni/djinni.h"

void onStageCreate(Stage* self, Game* game, Stage* previous);
void prepareStage(Stage* self, Game* game);
void updateStage(Stage* self, Game* game, double dt);
void drawStage(Stage* self, Game* game, double dt);
void onDestroyStage(Stage* self, Game* game, Stage* next);

#endif