#include "djinni/util/util.h"
#include "djinni/geometry/grid.h"
#include "djinni/renderable/renderable.h"

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
  level->cells = malloc(sizeof(GridCell) * level->width * level->height);

  // Initialize each cell with a fixed capacity array of entity pointers
  for (int i = 0; i < level->width * level->height; i++) {
    level->cells[i].capacity = cellCapacity;
    level->cells[i].entities = Djinni_Util_Array.initialize(cellCapacity);
  }
}

static void insert(Grid* grid, Entity* e, DJINNI_RING levelIdx) {
  GridLevel* level = &grid->levels[levelIdx];
  int cellSize = level->cellSize;

  //
  // Calculate grid cell bounds covering entity's bounding box
  //
  Point point = Djinni_Renderable.Entity->getRenderPoint(e);
  int renderedWidth = Djinni_Renderable.Entity->getRenderedWidth(e);
  int renderedHeight = Djinni_Renderable.Entity->getRenderedHeight(e);

  int minX = (int)(point.x / cellSize);
  int minY = (int)(point.y / cellSize);
  int maxX = (int)((point.x + renderedWidth) / cellSize);
  int maxY = (int)((point.y + renderedHeight) / cellSize);

  //
  // Insert entity pointer into every overlapping cell
  //
  for (int y = minY; y <= maxY; y++) {
    for (int x = minX; x <= maxX; x++) {
      if (x < 0 || y < 0 || x >= level->width || y >= level->height) continue;
      GridCell* cell = &level->cells[y * level->width + x];

      if (cell->entities->used >= cell->capacity) {
        continue;  // Skip if cell full
      }

      Djinni_Util_Array.insert(cell->entities, e);
    }
  }

  //
  // Cache the occupied cells bounds and level in the entity
  //
  e->locations[levelIdx].level = levelIdx;
  e->locations[levelIdx].minX = minX;
  e->locations[levelIdx].minY = minY;
  e->locations[levelIdx].maxX = maxX;
  e->locations[levelIdx].maxY = maxY;
}

void removeEntity(Grid* grid, Entity* e) {
  for (int l = 0; l < grid->levelCount; l++) {
    GridLocation* b = &e->locations[l];

    // entity is not in this ring
    if (b->level < 0) {
      continue;
    }

    GridLevel* level = &grid->levels[l];

    for (int y = b->minY; y <= b->maxY; y++) {
      for (int x = b->minX; x <= b->maxX; x++) {
        
        if (x < 0 || y < 0 || x >= level->width || y >= level->height) {
          continue;
        }

        GridCell* cell = &level->cells[y * level->width + x];
        for (int i = 0; i < cell->entities->used; i++) {
          if (cell->entities->data[i] == e) {
            Djinni_Util_Array.removeIndex(cell->entities, i);
            break;
          }
        }

      }
    }

    e->locations[l].level = -1;
    e->locations[l].minX = 0;
    e->locations[l].minY = 0;
    e->locations[l].maxX = 0;
    e->locations[l].maxY = 0;
  }
}


static void inspect(Grid* grid) {

  Djinni_Util_Logger.log_debug(
    "Djinni::Geometry::Grid.inspect( grid:(%p), levels:(%d) )",
    grid, grid->levelCount
  );

  for (int l = 0; l < grid->levelCount; l++) {
    GridLevel* level = &grid->levels[l];

    Djinni_Util_Logger.log_debug(
      "\tDjinni::Geometry::Grid::Level( level:(%d) cellsize:(%d) n-cells-x:(%d) n-cells-y:(%d) )",
      l, level->cellSize, level->width, level->height
    );

    int totalEntities = 0;
    for (int y = 0; y < level->height; y++) {
      for (int x = 0; x < level->width; x++) {
        GridCell* cell = &level->cells[y * level->width + x];
        if (cell->entities->used > 0) {

          Djinni_Util_Logger.log_debug(
            "\t\tDjinni::Geometry::Grid::Cell( x:(%d) y:(%d) entities:(%d) )",
            x, y, cell->entities->used
          );

          totalEntities += cell->entities->used;
        }
      }
    }

    Djinni_Util_Logger.log_debug("\tDjinni::Geometry::Grid::Level( entities:(%d) )", totalEntities);

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
  .insert = insert,
  .removeEntity = removeEntity,
  .inspect = inspect,
  .destroy = destroy
};