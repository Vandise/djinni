#ifndef DJINNI_GEOMETRY_POINT
#define DJINNI_GEOMETRY_POINT 1

typedef struct Djinni_PointStruct {
  int x;
  int y;
} Djinni_Point;

void djinni_point_concern_point_to_iso(int map_height, int* x_ptr, int* y_ptr, int width, int height);

#endif