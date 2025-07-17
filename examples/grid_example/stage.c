#include "gridexample.h"

static double switchMs = 0;
static int displayLevel = 0;
static int removed = 0;

static Entity* player = NULL;
static Grid* grid;

void onCreate(Stage* self, Game* game, Stage* previous) {
  grid = Djinni.Geometry->Grid->create(
    20,
    Djinni.windowSettings.width,
    Djinni.windowSettings.height,
    32,
    64,
    128
  );

  player = Djinni.Renderable->Sprite->create(
    Djinni.windowSettings.width/2,
    Djinni.windowSettings.height/2,
    "bin/gfx/player.png"
  );

  Djinni.Geometry->Grid->insert(grid, player, DJINNI_RING_FINE);
  Djinni.Geometry->Grid->inspect(grid);

  Djinni.Game->World->addEntity(game->world, player);

  Djinni.Game->enableInput(game);
}

void prepare(Stage* self, Game* game) {}
void update(Stage* self, Game* game, double dt) {
  if ((dt - switchMs) > 2000) {
    switchMs = dt;

    if (displayLevel++ >= 2) {
      displayLevel = 0;
      if (!removed) {
        Djinni.Geometry->Grid->removeEntity(grid, player);
        removed = 1;
      }
      Djinni.Geometry->Grid->inspect(grid);
    }
  }
}

void draw(Stage* self, Game* game, double dt) {

  Color collides[] = {
    { .r = 255, .g = 255, .b = 0, .a = 255 },
    { .r = 255, .g = 255, .b = 0, .a = 255 },
    { .r = 255, .g = 255, .b = 0, .a = 255 }
  };

  Color none[] = {
    { .r = 128, .g = 128, .b = 128, .a = 128 },
    { .r = 128, .g = 128, .b = 128, .a = 128 },
    { .r = 128, .g = 128, .b = 128, .a = 128 }
  };

  //
  //use grid-levelCount for all levels
  //
  for (int levelidx = displayLevel; levelidx < displayLevel+1; levelidx++) {
    GridLevel* level = &grid->levels[levelidx];
    int gridCellWidth = level->cellSize;

    for (int y = 0; y < level->height; y++) {
      for (int x = 0; x < level->width; x++) {
        
        GridCell* cell = &level->cells[y * level->width + x];

        SDL_Rect r = {
          x * gridCellWidth,
          y * gridCellWidth,
          gridCellWidth,
          gridCellWidth
        };

        if (cell->entities->used > 0) {
          Color c = collides[levelidx];
          SDL_SetRenderDrawColor(Djinni.renderer->instance, c.r, c.g, c.b, c.a);
        } else {
          Color c = none[levelidx];
          SDL_SetRenderDrawColor(Djinni.renderer->instance, c.r, c.g, c.b, c.a);
        }

        SDL_RenderDrawRect(Djinni.renderer->instance, &r);
      }
    }
  }
}

void onDestroy(Stage* self, Game* game, Stage* next) {
  Djinni.Geometry->Grid->destroy(grid);
}