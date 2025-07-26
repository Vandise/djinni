#include "game.h"

WorldMap* wm = NULL;

void objectLoader(Game* game, WorldMapObject* obj) {
  printf("world map object id:%d \n", obj->id);

  if (obj->id == 0) {
    Entity* player = Djinni.Renderable->Sprite->create(24, 23, "bin/gfx/player.png");
    player->worldMapData = obj;

    printf("created entity: %p \n", player);

    Djinni.Game->World->addEntity(game->world, player);
  }
}

void onStageCreate(Stage* self, Game* game, Stage* previous) {
  //game->camera->zoom = 2.0;
  wm = Djinni.Map->create(64,64);
  Djinni.Map->setMapDataFile(wm, "bin/data/map.json");
  Djinni.Map->setObjectLoader(wm, objectLoader);
  Djinni.Game->World->setWorldMap(game->world, wm);

  Djinni.Map->load(game, wm, Djinni.renderer);
  //Djinni.Map->inspect(wm);

  Djinni.Game->enableInput(game);
}

void prepareStage(Stage* self, Game* game) {}

void updateStage(Stage* self, Game* game, double dt) {
  if (game->keyboard[SDL_SCANCODE_W] == 1) {
    game->camera->point.y -= 3;
  }

  if (game->keyboard[SDL_SCANCODE_A] == 1) {
    game->camera->point.x -= 3;
  }

  if (game->keyboard[SDL_SCANCODE_S] == 1) {
    game->camera->point.y += 3;
  }

  if (game->keyboard[SDL_SCANCODE_D] == 1) {
    game->camera->point.x += 3;
  }
}

void drawStage(Stage* self, Game* game, double dt) {
  GridLevel* level = &game->world->grid->levels[0];
  int gridCellWidth = level->cellSize;

  for (int y = 0; y < level->height; y++) {
    for (int x = 0; x < level->width; x++) {

      GridCell* cell = &level->cells[y * level->width + x];

      SDL_Rect r = {
        (x * gridCellWidth * game->camera->zoom) - game->camera->point.x,
        (y * gridCellWidth * game->camera->zoom) - game->camera->point.y,
        gridCellWidth * game->camera->zoom,
        gridCellWidth * game->camera->zoom
      };

      SDL_SetRenderDrawColor(Djinni.renderer->instance, 128, 128, 128, 255);

      SDL_RenderDrawRect(Djinni.renderer->instance, &r);
    }
  }

}

void onDestroyStage(Stage* self, Game* game, Stage* next) {}