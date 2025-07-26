#include "djinni/util/logger.h"
#include "djinni/geometry/geometry.h"

static void initialize() {
  Djinni_Util_Logger.log_dev("Djinni::Geometry.initialize");

  Djinni_Geometry.Coordinate = &Djinni_Geometry_Coordinate;
  Djinni_Geometry.Line = &Djinni_Geometry_Line;
  Djinni_Geometry.Rectangle = &Djinni_Geometry_Rectangle;
  Djinni_Geometry.ObservablePoint = &Djinni_Geometry_ObservablePoint;
  Djinni_Geometry.Grid = &Djinni_Geometry_Grid;
  Djinni_Geometry.Isometric = &Djinni_Geometry_Isometric;
}

struct Djinni_GeometryStruct Djinni_Geometry = {
  .initialize = initialize
};