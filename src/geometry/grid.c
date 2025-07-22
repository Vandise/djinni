#include "djinni/util/util.h"
#include "djinni/geometry/grid.h"
#include "djinni/renderable/renderable.h"
#include "djinni/game/camera.h"

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
  Djinni_Util_Logger.log_dev("Djinni::Geometry::Grid::insert( grid:(%p) entity:(%p) level:(%d) )", grid, e, levelIdx);

  GridLevel* level = &grid->levels[levelIdx];
  int cellSize = level->cellSize;

  //
  // Calculate grid cell bounds covering entity's bounding box
  //
  Point point = Djinni_Renderable.Entity->getRenderPoint(e);
  int renderedWidth = Djinni_Renderable.Entity->getRenderedWidth(e);
  int renderedHeight = Djinni_Renderable.Entity->getRenderedHeight(e);

  if (point.x < 0 || point.y < 0) {
    Djinni_Util_Logger.log_warn("Djinni::Geometry::Grid::insert( grid:(%p) entity:(%p) level:(%d) x:(%d) y:(%d) ) - outside of grid",
      grid, e, levelIdx, point.x, point.y
    );
    return;
  }

  int minX = (int)(point.x / cellSize);
  int minY = (int)(point.y / cellSize);
  int maxX = (int)((point.x + renderedWidth) / cellSize);
  int maxY = (int)((point.y + renderedHeight) / cellSize);

  GridLocation* location = &e->locations[levelIdx];

  //
  // Cache the occupied cells bounds and level in the entity
  //
  location->level = levelIdx;
  location->minX = minX;
  location->minY = minY;
  location->maxX = maxX;
  location->maxY = maxY;
  if (location->cells == NULL) {
    location->cells = Djinni_Util_Array.initialize(4);
  }

  //
  // Insert entity pointer into every overlapping cell
  //
  for (int y = minY; y <= maxY; y++) {
    for (int x = minX; x <= maxX; x++) {
      if (x < 0 || y < 0 || x >= level->width || y >= level->height) {
        continue;
      }
  
      GridCell* cell = &level->cells[y * level->width + x];

      if (cell->entities->used >= cell->capacity) {
        continue;  // Skip if cell full
      }

      Djinni_Util_Array.insert(cell->entities, e);
      Djinni_Util_Array.insert(location->cells, cell);
    }
  }
}

void removeEntity(Grid* grid, Entity* e) {
  for (int level = 0; level < grid->levelCount; level++) {
    GridLocation* location = &e->locations[level];

    if (location->level < 0) {
      continue;
    }

    // Fast-path: remove entity from each stored cell by cached index
    for (int i = 0; i < location->cells->used; i++) {
      GridCell* cell = location->cells->data[i];
      for (int j = 0; j < cell->entities->used; j++) {
        if (cell->entities->data[j] == e) {
          Djinni_Util_Array.removeIndex(cell->entities, j);
        }
      }
    }

    // Reset location metadata
    location->level = -1;
    location->minX = location->minY = 0;
    location->maxX = location->maxY = 0;

    // Preserve allocation; just clear use count
    location->cells->used = 0;
  }
}

static int entityNeedsCellUpdate(Grid* grid, Entity* e) {
  Point point = Djinni_Renderable.Entity->getRenderPoint(e);
  int renderedWidth = Djinni_Renderable.Entity->getRenderedWidth(e);
  int renderedHeight = Djinni_Renderable.Entity->getRenderedHeight(e);
  int result = 0;

  for (int l = 0; l < DJINNI_GRID_MAX_LEVELS; l++) {
    if (e->locations[l].level > -1) {
      int cellSize = grid->levels[l].cellSize;
      int minX = (int)(point.x / cellSize);
      int minY = (int)(point.y / cellSize);
      int maxX = (int)((point.x + renderedWidth) / cellSize);
      int maxY = (int)((point.y + renderedHeight) / cellSize);

      if (minX != e->locations[l].minX || minY != e->locations[l].minY ||
          maxX != e->locations[l].maxX || maxY != e->locations[l].maxY
      ) {
        result = 1;
        break;
      }
    }
  }

  return result;
}

static DJINNI_RING getCurrentEntityRing(Entity* e) {
  DJINNI_RING level = 0;

  for (int i = 0; i < DJINNI_GRID_MAX_LEVELS; i++) {
    if (e->locations[i].level >= 0) {
      level = i;
      break;
    }
  }

  return level;
}

static DJINNI_RING computeRingLevel(ViewportBounds viewport, Entity* e) {
  Point entitypt = Djinni_Renderable.Entity->getPosition(e);

  if (
    entitypt.x >= viewport.x1 && entitypt.x <= viewport.x2 &&
    entitypt.y >= viewport.y1 && entitypt.y <= viewport.y2
  ) {
    return DJINNI_RING_FINE;
  }

  int screenWidth = viewport.x2 - viewport.x1;
  int screenheight = viewport.y2 - viewport.y1;

  int ptx = viewport.x1 - screenWidth / 2;
  int pty = viewport.y1 - screenheight / 2;

  if (
    entitypt.x >= ptx && entitypt.x <= (screenWidth * 2) &&
    entitypt.y >= pty && entitypt.y <= (screenheight * 2)
  ) {
    return DJINNI_RING_MEDIUM;
  }

  return DJINNI_RING_COARSE;
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
            "\t\tDjinni::Geometry::Grid::Cell( cell:(%p) x:(%d) y:(%d) entities:(%d) )",
            cell, x, y, cell->entities->used
          );

          for (int i = 0; i < cell->entities->used; i++) {
            Djinni_Util_Logger.log_debug(
              "\t\t\tDjinni::Geometry::Grid::Cell::Entity( entity:(%p) )",
              cell->entities->data[i]
            );
          }

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
  .computeRingLevel = computeRingLevel,
  .getCurrentEntityRing = getCurrentEntityRing,
  .entityNeedsCellUpdate = entityNeedsCellUpdate,
  .inspect = inspect,
  .destroy = destroy
};