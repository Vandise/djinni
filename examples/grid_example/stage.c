#include "gridexample.h"

static double switchMs = 0;
static int displayLevel = 0;
static int removed = 0;

static Camera* camera = NULL;
static Entity* player = NULL;
static Entity* enemy1 = NULL;
static Entity* enemy2 = NULL;

static Grid* grid;
static int cameraWH;

void onCreate(Stage* self, Game* game, Stage* previous) {
  cameraWH = Djinni.windowSettings.width / 4;

  camera = Djinni.Game->Camera->create(
    0,
    0,
    cameraWH,
    cameraWH
  );

  Djinni.Game->setCamera(game, camera);

  grid = game->world->grid;

  /*
    Djinni.windowSettings.width,
    Djinni.windowSettings.height

    some test renders:
      0,0 to show the grid is only positive
  */

  player = Djinni.Renderable->Sprite->create(
    100,
    100,
    "bin/gfx/player.png"
  );

  // center the camera
  Djinni.Game->Camera->follow(camera, player, 0, 0);

  // ring 2
  enemy1 = Djinni.Renderable->Sprite->create(
    cameraWH + 100,
    cameraWH + 100,
    "bin/gfx/enemy.png"
  );

  // ring 3
  enemy2 = Djinni.Renderable->Sprite->create(
    cameraWH + 700,
    cameraWH + 300,
    "bin/gfx/enemy.png"
  );

  Djinni.Game->World->addEntity(game->world, player);
  Djinni.Game->World->addEntity(game->world, enemy1);
  Djinni.Game->World->addEntity(game->world, enemy2);

  Djinni.Geometry->Grid->inspect(grid);

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


  SDL_Rect cameraBounds = {
    .x = camera->point.x,
    .y = camera->point.y,
    .w = camera->screenWidth,
    .h = camera->screenHeight
  };

  SDL_SetRenderDrawColor(Djinni.renderer->instance, 255, 255, 0, 128);
  SDL_RenderDrawRect(Djinni.renderer->instance, &cameraBounds);

  SDL_Rect ring1Bounds = {
    .x = (camera->point.x),
    .y = (camera->point.y),
    .w = (camera->screenWidth * 2),
    .h = (camera->screenHeight * 2)
  };

  SDL_SetRenderDrawColor(Djinni.renderer->instance, 0, 255, 0, 255);
  SDL_RenderDrawRect(Djinni.renderer->instance, &ring1Bounds);


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
          (x * gridCellWidth)-camera->point.x,
          (y * gridCellWidth)-camera->point.y,
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