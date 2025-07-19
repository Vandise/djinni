#ifndef DJINNI_GEOMETRY_GRID
#define DJINNI_GEOMETRY_GRID 1

#include "djinni/game/shared.h"
#include "djinni/geometry/shared.h"
#include "djinni/renderable/shared.h"
#include "djinni/util/array.h"

typedef enum {
  DJINNI_RING_FINE,
  DJINNI_RING_MEDIUM,
  DJINNI_RING_COARSE
} DJINNI_RING;

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

//
// A cache for an entity and its bounds in a grid cell
//
typedef struct GridLocationStruct {
  int level;             // Grid level where the entity was inserted
  int minX, minY;        // Cached previous grid cell bounds (min coords)
  int maxX, maxY;        // Cached previous grid cell bounds (max coords)

  DjinniArray* cells;    // occupying cells
  DjinniArray* indicies; // entities index for each cell
} GridLocation;

struct Djinni_Geometry_GridStruct {
  Grid* (*create)(int,int,int,int,int,int);
  void (*insert)(Grid*, Entity*, DJINNI_RING);
  void (*removeEntity)(Grid*, Entity*);
  DJINNI_RING (*computeRingLevel)(ViewportBounds, Entity*);
  DJINNI_RING (*getCurrentEntityRing)(Entity*);
  int (*entityNeedsCellUpdate)(Grid*, Entity*);
  void (*inspect)(Grid*);
  void (*destroy)(Grid*);
};

extern struct Djinni_Geometry_GridStruct Djinni_Geometry_Grid;

#endif