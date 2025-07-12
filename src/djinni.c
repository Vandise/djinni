#include "djinni/djinni.h"
#include "djinni/util/util.h"

static int initialize(WindowSettings ws, VideoSettings vs) {
  Djinni_Util_Logger.log_dev("Djinni.initialize");

  Djinni.windowSettings = ws;
  Djinni.videoSettings = vs;

  Djinni_Geometry.initialize();
  Djinni_Renderable.initialize();
  Djinni_Physics.initialize();

  Djinni.Logger = &Djinni_Util_Logger;
  Djinni.Video = &Djinni_Video;
  Djinni.Geometry = &Djinni_Geometry;
  Djinni.Renderable = &Djinni_Renderable;
  Djinni.Physics = &Djinni_Physics;

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

static void terminate() {
  Djinni_Util_Logger.log_dev("Djinni.terminate");

  Djinni.Video->Renderer->destroy(Djinni.renderer);
  Djinni.Video->Window->destroy(Djinni.window);

  SDL_Quit();
}

struct DjinniStruct Djinni = {
  .initialize = initialize,
  .setFlag = setFlag,
  .terminate = terminate
};