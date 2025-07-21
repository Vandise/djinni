#include "game.h"

int main(void) {
  WindowSettings ws = {
    .posx = SDL_WINDOWPOS_UNDEFINED,
    .posy = SDL_WINDOWPOS_UNDEFINED,
    .width = 1280,
    .height = 720,
    .flags = 0
  };
  ws.name = strdup("2d Shooter");

  VideoSettings vs = {
    .index = 0,
    .videoFlags = IMG_INIT_PNG | IMG_INIT_JPG,
    .rendererFlags = SDL_RENDERER_ACCELERATED
  };

  Djinni.setFlag(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  Djinni.initialize(ws, vs);

  Djinni.Logger->log_level = DJINNI_LOG_DEBUG;

  Color background = {
    .r = 0,
    .g = 0,
    .b = 0,
    .a = 255
  };

  Djinni.Video->Renderer->setBackgroundColor(Djinni.renderer, background);

  WorldSettings worldSettings = {
    .width = 1280,
    .height = 720,
    .finestGridSize = 32,
    .mediumGridSize = 64,
    .coarseGridSize = 128,
    .gridCellCapacity = 10
  };

  Game* game = Djinni.Game->create(worldSettings);

  Stage* s = Djinni.Game->Stage->create(
    0,
    onStageCreate, prepareStage, updateStage, drawStage, onDestroyStage
  );

  Djinni.Game->addStage(game, s);

  Djinni.start(game, 0);

  Djinni.Logger->log_debug("FPS:(%d)", game->stats.fps);

  Djinni.Game->destroy(game);
  Djinni.terminate();

  return 0;
}