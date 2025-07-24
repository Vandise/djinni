#include "game.h"

WorldMap* wm = NULL;

void onStageCreate(Stage* self, Game* game, Stage* previous) {
  //game->camera->zoom = 2.0;
  wm = Djinni.Map->create();
  Djinni.Map->load(wm, Djinni.renderer, "bin/data/map.json");
  Djinni.Map->inspect(wm);

  //Djinni.Map->destroy(wm);
  
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
  //Djinni.Video->ImageAtlas->blit(Djinni.renderer, img, 200, 200, 64, 64);

  Djinni.Map->draw(wm, Djinni.renderer, game->camera, dt);


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