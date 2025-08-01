#include "djinni/geometry/point.h"
/*
  
Renders the top-left corner of the isometric tile as the draw point. Rendering at (x, y), the top of the diamond is placed at ( screen x and y )
       (sx, sy)
          /\
        /    \
       /      \
      /        \
     /          \
    /            \

Y is centered within the grid to prevent negative coordinates. 0,0 will be center-left.

*/
void djinni_point_concern_point_to_iso(int map_height, int* x_ptr, int* y_ptr, int width, int height) {
  int x = *x_ptr;
  int y = *y_ptr;

  *x_ptr = ((x * width / 2) + (y * width / 2));
  *y_ptr = (map_height / 2) - ((y * (height / 2) / 2) - (x * (height / 2) / 2));
}