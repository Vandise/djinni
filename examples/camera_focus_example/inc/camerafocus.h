#ifndef __CAMERA_FOCUS_EXAMPLE
#define __CAMERA_FOCUS_EXAMPLE 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "djinni/djinni.h"

void onCreate(Stage* self, Game* game, Stage* previous);
void prepare(Stage* self, Game* game);
void update(Stage* self, Game* game, double dt);
void draw(Stage* self, Game* game, double dt);
void onDestroy(Stage* self, Game* game, Stage* next);

#endif