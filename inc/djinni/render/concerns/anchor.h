#ifndef DJINNI_RENDER_CONCERNS_ANCHOR
#define DJINNI_RENDER_CONCERNS_ANCHOR 1

#include "djinni/geometry/geometry.h"

Djinni_Point djinni_render_concern_get_anchor_point(
  float anchor_x, float anchor_y,
  int x, int y,
  int w, int h
);

Djinni_Point djinni_render_concern_translate_anchor_point(
  float anchor_x, float anchor_y,
  int x, int y,
  int w, int h
);

#endif