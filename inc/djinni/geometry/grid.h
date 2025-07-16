#ifndef DJINNI_GEOMETRY_GRID
#define DJINNI_GEOMETRY_GRID 1

#include "djinni/geometry/shared.h"
#include "djinni/util/array.h"

#define DJINNI_GRID_MAX_LEVELS 3

//
// A single cell in the grid storing pointers to entities occupying it
//
typedef struct GridCellStruct {
  DjinniArray* entities;  // Pointer array of entities in this cell
  int capacity;           // Maximum capacity of the entities array
} GridCell;

//
// Represents a grid layer with uniform-sized cells covering the world at a given resolution
//
typedef struct GridLevelStruct {
  int cellSize;       // Width and height of each cell in pixels
  int width, height;  // Number of cells horizontally and vertically
  GridCell* cells;    // Array of grid cells (width * height)
} GridLevel;

//
// Full hierarchical grid system containing multiple grid levels and the pointer pool
//
typedef struct GridStruct {
  GridLevel* levels; // Array of grid levels
  int levelCount;    // Number of levels (e.g., 3)
} Grid;

struct Djinni_Geometry_GridStruct {
  Grid* (*create)(int,int,int,int,int,int);
  void (*destroy)(Grid*);
};

extern struct Djinni_Geometry_GridStruct Djinni_Geometry_Grid;

#endif