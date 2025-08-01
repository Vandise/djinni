#ifndef DJINNI_ECS_COMPONENT_SPRITE
#define DJINNI_ECS_COMPONENT_SPRITE 1

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "djinni/common.h"
#include "djinni/ecs/shared.h"

typedef struct Djinni_SpriteStruct {
  int texture_status;

  SDL_Texture* texture;
  SDL_Rect src;

  struct {
    float x;
    float y;
  } anchor;
} Djinni_Sprite;

void djinni_ecs_component_sprite_initialize(int base_entity_count);
Djinni_Sprite* djinni_ecs_component_sprite_get(DjinniEntityId id);
void djinni_ecs_component_sprite_destroy_entity(DjinniEntityId id);
void djinni_ecs_component_sprite_pool_expand(int new_size);
void djinni_ecs_component_sprite_destroy();

#endif