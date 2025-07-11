#include "util/logger.h"
#include "geometry/geometry.h"

static void initialize() {
  Djinni_Util_Logger.log_dev("Djinni::Geometry.initialize");

  Djinni_Geometry.Coordinate = &Djinni_Geometry_Coordinate;
  Djinni_Geometry.Line = &Djinni_Geometry_Line;
}

struct Djinni_GeometryStruct Djinni_Geometry = {
  .initialize = initialize
};