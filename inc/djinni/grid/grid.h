#ifndef DJINNI_GRID_GRID
#define DJINNI_GRID_GRID 1

#include "djinni/common.h"
#include "djinni/grid/shared.h"
#include "djinni/grid/concerns/entity_concern.h"
#include "djinni/ecs/shared.h"
#include "djinni/util/array.h"

//
// A single cell in the grid storing pointers to entities occupying it
//
typedef struct Djinni_GridCellStruct {
  DjinniArray* entities;  // Pointer array of entities in this cell
  int capacity;           // Maximum capacity of the entities array
} Djinni_GridCell;

//
// Represents a grid layer with uniform-sized cells covering the world at a given resolution
//
typedef struct Djinni_GridLevelStruct {
  int cell_size;              // Width and height of each cell in pixels
  int width, height;         // Number of cells horizontally and vertically
  Djinni_GridCell* cells;    // Array of grid cells (width * height)
} Djinni_GridLevel;

//
// Full hierarchical grid system containing multiple grid levels and the pointer pool
//
typedef struct Djinni_GridStruct {
  Djinni_GridLevel* levels; // Array of grid levels
  int level_count;          // Number of levels (e.g., 3)
} Djinni_Grid;

typedef struct Djinni_GridLocationStruct {
  int level;             // Grid level where the entity was inserted
  int minX, minY;        // Cached previous grid cell bounds (min coords)
  int maxX, maxY;        // Cached previous grid cell bounds (max coords)

  DjinniArray* cells;    // occupying cells
} Djinni_GridLocation;

Djinni_Grid* djinni_grid_create(
  int cell_capacity,
  int world_width,
  int world_height,
  int finest_size,
  int mid_size,
  int coarse_size
);

void djinni_grid_initialize_level(
  Djinni_GridLevel* level,
  int cell_size,
  int cell_capacity,
  int world_width,
  int world_height
);

void djinni_grid_insert(Djinni_Grid* grid, DjinniEntityId id, DJINNI_GRID_RING level_index);
void djinni_grid_remove(Djinni_Grid* grid, DjinniEntityId id);
void djinni_grid_destroy(Djinni_Grid* grid);

#endif