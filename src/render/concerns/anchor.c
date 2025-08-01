#include "djinni/render/concerns/anchor.h"

Djinni_Point djinni_render_concern_get_anchor_point(
  float anchor_x, float anchor_y,
  int x, int y,
  int w, int h
) {
  Djinni_Point p;

  if (anchor_x < DJINNI_ANCHOR_DEFAULT && anchor_x != 0) {
    p.x = x + w * (DJINNI_ANCHOR_DEFAULT - anchor_x);
  } else {
    p.x = x + w * anchor_x;
  }

  if (anchor_y < DJINNI_ANCHOR_DEFAULT && anchor_y != 0) {
    p.y = y + h * (DJINNI_ANCHOR_DEFAULT - anchor_y);
  } else {
    p.y = y + h * anchor_y;
  }

  return p;
}

Djinni_Point djinni_render_concern_translate_anchor_point(
  float anchor_x, float anchor_y,
  int x, int y,
  int w, int h
) {
  int ax = w * anchor_x;
  int ay = h * anchor_y;
  int dx = x - ax;
  int dy = y - ay;

  Djinni_Point p = {
    .x = dx,
    .y = dy
  };

  return p;
}