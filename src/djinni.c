#include "djinni/djinni.h"
#include "djinni/util/util.h"

static int initialize(WindowSettings ws, VideoSettings vs) {
  Djinni_Util_Logger.log_dev("Djinni.initialize");

  Djinni.windowSettings = ws;
  Djinni.videoSettings = vs;

  Djinni_Geometry.initialize();
  Djinni_Renderable.initialize();
  Djinni_Physics.initialize();
  Djinni_Game.initialize();

  Djinni.Logger = &Djinni_Util_Logger;
  Djinni.Array = &Djinni_Util_Array;

  Djinni.Video = &Djinni_Video;
  Djinni.Geometry = &Djinni_Geometry;
  Djinni.Renderable = &Djinni_Renderable;
  Djinni.Physics = &Djinni_Physics;
  Djinni.Game = &Djinni_Game;

  Djinni_Video.initialize(vs.videoFlags);

  Djinni.window = Djinni_Video.Window->create(
    ws.name, ws.posx, ws.posy, ws.width, ws.height, ws.flags
  );

  Djinni.renderer = Djinni_Video.Renderer->create(
    Djinni.window, vs.index, vs.rendererFlags
  );

  Djinni_Video.setRenderer(Djinni.renderer);

  return 0;
}

static void setFlag(const char *name, const char *value) {
  SDL_SetHint(name, value);
}

static void start(Game* game) {
  Djinni_Util_Logger.log_dev("Djinni.start( game:(%p) )", game);

  if (game->camera == NULL) {
    Djinni_Util_Logger.log_debug("Djinni.start() - No camera detected, setting default");
    // set default anchor to top left for 0,0
    game->camera = Djinni_Game.Camera->create(
      0,
      0,
      Djinni.windowSettings.width,
      Djinni.windowSettings.height
    );
  }

  Djinni_Game.Runner->execute(game);
}

static void terminate() {
  Djinni_Util_Logger.log_dev("Djinni.terminate");

  Djinni.Video->Renderer->destroy(Djinni.renderer);
  Djinni.Video->Window->destroy(Djinni.window);

  SDL_Quit();
}

struct DjinniStruct Djinni = {
  .initialize = initialize,
  .setFlag = setFlag,
  .start = start,
  .terminate = terminate
};