#ifndef DJINNI_GEOMETRY_RECTANGLE
#define DJINNI_GEOMETRY_RECTANGLE 1

void djinni_rectangle_concern_set_center_bound_offsets(
  int inner_w,
  int inner_h,
  int outer_w,
  int outer_h,
  int* offset_x,
  int* offset_y
);

#endif