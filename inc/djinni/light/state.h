#ifndef DJINNI_LIGHT_STATE
#define DJINNI_LIGHT_STATE 1

#include "djinni/common.h"
#include "djinni/light/shared.h"

void djinni_light_state_initialize();
void djinni_light_state_add_light(Djinni_Light* light);
DjinniArray* djinni_light_state_get_lights();
void djinni_light_state_destroy();

#endif