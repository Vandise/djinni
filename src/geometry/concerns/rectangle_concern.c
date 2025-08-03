#include "djinni/geometry/rectangle.h"

void djinni_rectangle_concern_set_center_bound_offsets(
  int inner_w,
  int inner_h,
  int outer_w,
  int outer_h,

  int* offset_x,
  int* offset_y
) {
    *offset_x = (outer_w - inner_w) / 2;
    *offset_y = (outer_h - inner_h) / 2;
}