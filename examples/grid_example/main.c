#include "gridexample.h"

int main(void) {
  WindowSettings ws = {
    .posx = SDL_WINDOWPOS_UNDEFINED,
    .posy = SDL_WINDOWPOS_UNDEFINED,
    .width = 1200,
    .height = 720,
    .flags = 0
  };
  ws.name = strdup("Grid");

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

  Game* game = Djinni.Game->create();
  Stage* s = Djinni.Game->Stage->create(
    0,
    onCreate, prepare, update, draw, onDestroy
  );

  Djinni.Game->addStage(game, s);
  Djinni.Game->changeStage(game, 0);

  Djinni.start(game);

  Djinni.Logger->log_debug("FPS:(%d)", game->stats.fps);

  Djinni.Game->destroy(game);
  Djinni.terminate();

  return 0;
}