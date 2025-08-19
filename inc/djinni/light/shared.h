#ifndef DJINNI_LIGHT_SHARED
#define DJINNI_LIGHT_SHARED 1

#include "djinni/util/array.h"

/*
  Iso Shadows (DDA + Soft Penumbra)
  ---------------------------------------------------
  - Renders an isometric tilemap from a texture atlas.

  - Casts fewer "base" rays; each base ray spawns a small bundle of
    jittered sub-rays to simulate an area light / soft penumbra.

  - Each sub-ray writes darkness only AFTER the first blocker,
    so shadows extend away from the light instead of looking omni per tile.

  -----------------------------------------------

  Pre-render a solid white diamond into an offscreen texture once.
  Per tile,  set color/alpha modulation and SDL_RenderCopy it.

  ---------------------------

   cached texture (TILE_W×TILE_H)       per-frame draw
   +--------------------------+         +-----------------+
   |    white diamond (α=1)   |         | tint=(0,0,0,a)  |
   |  on transparent BG       |  --->   | render-copy     |
   +--------------------------+         +-----------------+

  --------------
  Isometric diamond footprint (screen pixels), width=TILE_W, height=TILE_H:

        (sx + TILE_W/2, sy)
               /\
              /  \
     (sx,sy+H/2)  (sx+W,sy+H/2)
              \  /
               \/
        (sx + TILE_W/2, sy + TILE_H)
*/

typedef struct Djinni_LightStruct {
  int n_rays;                 // angular samples around 360°
  int n_sub_rays;             // jittered sub-rays per base ray
  int max_daa_steps;          // DAA safety cap through the grid
  int max_alpha;              // Max shadow alpha

  float penumbra_arc_degrees; // total arc span per bundle (degrees)
  float shadow_steps;         // how fast darkness grows behind blocker
  float light_range;          // how far shadow sustains behind blocker
} Djinni_Light;


#endif