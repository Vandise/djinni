#ifndef DJINNI_GEOMETRY_SHARED
#define DJINNI_GEOMETRY_SHARED 1

#include <stdlib.h>

#define ANCHOR_DEFAULT 0.5
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

typedef struct CoordinateStruct Coordinate;
typedef struct LineStruct Line;
typedef struct LineSlopeStruct LineSlope;
typedef struct RectangleStruct Rectangle;
typedef struct ObservablePointStruct ObservablePoint;
typedef struct GridCellStruct GridCell;
typedef struct GridLevelStruct GridLevel;
typedef struct GridStruct Grid;
typedef struct GridLocationStruct GridLocation;

#endif