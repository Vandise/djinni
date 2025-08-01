#include "djinni/grid/grid.h"
#include "djinni/ecs/ecs.h"

Djinni_Grid* djinni_grid_create(
  int cell_capacity,
  int world_width,
  int world_height,
  int finest_size,
  int mid_size,
  int coarse_size
) {
  int cell_sizes[DJINNI_GRID_MAX_LEVELS] = {
    finest_size, mid_size, coarse_size
  };

  Djinni_Grid* grid = malloc(sizeof(Djinni_Grid));

  grid->levels = malloc(sizeof(Djinni_GridLevel) * DJINNI_GRID_MAX_LEVELS);
  grid->level_count = DJINNI_GRID_MAX_LEVELS;

  for (int i = 0; i < DJINNI_GRID_MAX_LEVELS; i++) {
    djinni_grid_initialize_level(
      &grid->levels[i], cell_sizes[i], cell_capacity, world_width, world_height
    );
  }

  return grid;
}

void djinni_grid_initialize_level(
  Djinni_GridLevel* level,
  int cell_size,
  int cell_capacity,
  int world_width,
  int world_height
) {
  level->cell_size = cell_size;
  level->width = (world_width + cell_size - 1) / cell_size;
  level->height = (world_height + cell_size - 1) / cell_size;
  level->cells = malloc(sizeof(Djinni_GridCell) * level->width * level->height);

  // Initialize each cell with a fixed capacity array of entity pointers
  for (int i = 0; i < level->width * level->height; i++) {
    level->cells[i].capacity = cell_capacity;
    level->cells[i].entities = djinni_array_initialize(cell_capacity);
  }
}

void djinni_grid_insert(Djinni_Grid* grid, DjinniEntityId id, DJINNI_GRID_RING level_index) {
  Djinni_GridLevel* level = &grid->levels[level_index];
  int cell_size = level->cell_size;

  //
  // Calculate grid cell bounds covering entity's bounding box
  //
  Djinni_Collidable* collision_box = djinni_ecs_component_collision_get(id);
  Djinni_Position* position = djinni_ecs_component_position_get(id);

  int rendered_width = collision_box->width;
  int rendered_height = collision_box->height;

  if (position->x < 0 || position->y < 0) {
    return;
  }

  int minX = (int)(position->x / cell_size);
  int minY = (int)(position->y / cell_size);
  int maxX = (int)((position->x + rendered_width) / cell_size);
  int maxY = (int)((position->y + rendered_height) / cell_size);

  //
  // Cache the occupied cells bounds and level in the entity
  //
  collision_box->grid_cache.level = level_index;
  collision_box->grid_cache.minX = minX;
  collision_box->grid_cache.minY = minY;
  collision_box->grid_cache.maxX = maxX;
  collision_box->grid_cache.maxY = maxY;

  if (collision_box->grid_cache.cells == NULL) {
    collision_box->grid_cache.cells = djinni_array_initialize(4);
  }

  //
  // Insert entity pointer into every overlapping cell
  //
  for (int y = minY; y <= maxY; y++) {
    for (int x = minX; x <= maxX; x++) {
      if (x < 0 || y < 0 || x >= level->width || y >= level->height) {
        continue;
      }

      Djinni_GridCell* cell = &level->cells[y * level->width + x];

      if (cell->entities->used >= cell->capacity) {
        continue;  // Skip if cell full
      }

      // resize the array if needed then insert indirectly
      int* idptr = malloc(sizeof(int));
      *idptr = id;
      djinni_array_insert(cell->entities, idptr);
      djinni_array_insert(collision_box->grid_cache.cells, cell);
    }
  }
}

void djinni_grid_remove(Djinni_Grid* grid, DjinniEntityId id) {
  Djinni_Collidable* collision_box = djinni_ecs_component_collision_get(id);

  if (collision_box->grid_cache.level < 0) {
    return;
  }

  // Fast-path: remove entity from each stored cell by cached index
  for (int i = 0; i < collision_box->grid_cache.cells->used; i++) {
    Djinni_GridCell* cell = collision_box->grid_cache.cells->data[i];
    for (int j = 0; j < cell->entities->used; j++) {
      if (*((int*)cell->entities->data[j]) == id) {
        djinni_array_delete_index(cell->entities, j, free);
      }
    }
  }

  // Reset location metadata
  collision_box->grid_cache.level = -1;
  collision_box->grid_cache.minX = collision_box->grid_cache.minY = 0;
  collision_box->grid_cache.maxX = collision_box->grid_cache.maxY = 0;

  // Preserve allocation; just clear use count
  collision_box->grid_cache.cells->used = 0;
}

void djinni_grid_destroy(Djinni_Grid* grid) {
  if (grid == NULL) { return; }

  for (int i = 0; i < grid->level_count; i++) {
    Djinni_GridLevel* level = &grid->levels[i];

    for (int j = 0; j < level->width * level->height; j++) {
      djinni_array_destroy(level->cells[j].entities, free);
    }

    free(grid->levels[i].cells);
  }

  free(grid->levels);
  free(grid);
}