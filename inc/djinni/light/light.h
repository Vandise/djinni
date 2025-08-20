#ifndef DJINNI_LIGHT
#define DJINNI_LIGHT 1

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <string.h>

#include "djinni/light/shared.h"
#include "djinni/light/state.h"
#include "djinni/map/shared.h"

void djinni_light_generate_shadow_texture(int tile_width, int tile_height);
SDL_Texture* djinni_light_get_shadow_texture();
void djinni_light_generate_shadows(Djinni_Map* djinni_map, Djinni_Light* light);
void djinni_light_destroy_shadow_texture();

#endif