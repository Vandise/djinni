#include "djinni/render/map.h"
#include "djinni/map/map.h"
#include "djinni/map/state.h"

void djinni_map_render_map(double dt) {
  Djinni_Map* djinni_map = djinni_map_state_get_map();

  for (int i = 0; i < DJINNI_MAP_N_LAYERS; i++) {
    Djinni_MapLayer* layer = &(djinni_map->layers[i]);

    if (layer->id >= 0 && layer->draw != NULL) {
      layer->draw(layer->id, dt);
    }
  }
}