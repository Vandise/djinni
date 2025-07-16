#include "djinni/util/util.h"
#include "djinni/geometry/grid.h"

static void initializeLevel(
  GridLevel* level,
  int cellSize,
  int cellCapacity,
  int worldWidth,
  int worldHeight
);

//
// Initialize the full hierarchical grid with multiple levels
//
static Grid* create(
  int cellCapacity,
  int worldWidth,
  int worldHeight,
  int finestSize,
  int midSize,
  int coarseSize
) {

  int cellSizes[DJINNI_GRID_MAX_LEVELS] = {
    finestSize, midSize, coarseSize
  };

  Grid* grid = malloc(sizeof(Grid));

  grid->levels = malloc(sizeof(GridLevel) * DJINNI_GRID_MAX_LEVELS);
  grid->levelCount = DJINNI_GRID_MAX_LEVELS;

  for (int i = 0; i < DJINNI_GRID_MAX_LEVELS; i++) {
    initializeLevel(
      &grid->levels[i], cellSizes[i], cellCapacity, worldWidth, worldHeight
    );
  }

  return grid;
}

static void initializeLevel(
  GridLevel* level,
  int cellSize,
  int cellCapacity,
  int worldWidth,
  int worldHeight
){
  level->cellSize = cellSize;
  level->width = (worldWidth + cellSize - 1) / cellSize;
  level->height = (worldHeight + cellSize - 1) / cellSize;

  // Allocate the grid cells array
  level->cells = malloc(sizeof(GridCell) * level->width * level->height);

  // Initialize each cell with a fixed capacity array of entity pointers
  for (int i = 0; i < level->width * level->height; i++) {
    level->cells[i].capacity = cellCapacity;
    level->cells[i].entities = Djinni_Util_Array.initialize(cellCapacity);
  }
}

static void destroy(Grid* grid) {
  for (int i = 0; i < grid->levelCount; i++) {
    free(grid->levels[i].cells);
  }
  free(grid->levels);
  free(grid);
}

struct Djinni_Geometry_GridStruct Djinni_Geometry_Grid = {
  .create = create,
  .destroy = destroy
};