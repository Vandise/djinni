#ifndef DJINNI_GAME_INPUT
#define DJINNI_GAME_INPUT 1

#include "djinni/game/shared.h"

typedef struct Djinni_Game_IOStruct {
  int enabled;
  int keyboard[MAX_IO_KEY_CODE];
} Djinni_GameIO;

void djinni_game_input_initialize();
int djinni_game_input_key_pressed(int key);
int djinni_game_input_enabled();
void djinni_game_input_enable();
void djinni_game_input_disable();
void djinni_game_input_process();
void djinni_game_input_destroy();

#endif