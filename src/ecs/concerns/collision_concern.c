#include "djinni/ecs/ecs.h"
#include "djinni/grid/grid.h"
#include "djinni/geometry/rectangle.h"

DjinniEntityId djinni_ecs_collision_concern_create_rectangle(int x, int y, int width, int height, int solid) {
  DjinniEntityId id = djinni_ecs_create_entity(DJINNI_COMPONENT_POSITION | DJINNI_COMPONENT_COLLIDABLE);

  Djinni_Position* position = djinni_ecs_component_position_get(id);
    position->x = x;
    position->y = y;

  Djinni_Collidable* collision_box = djinni_ecs_component_collision_get(id);
    collision_box->width  = width;
    collision_box->height = height;
    collision_box->solid = solid;
    collision_box->parent_x_offset = 0;
    collision_box->parent_y_offset = 0;

  return id;
}

DjinniEntityId djinni_ecs_collision_concern_create_centered_child_rectangle(
  int x, int y, int width, int height, int parent_width, int parent_height, int solid
) {
  DjinniEntityId id = djinni_ecs_collision_concern_create_rectangle(x, y, width, height, solid);
  Djinni_Collidable* collision_box = djinni_ecs_component_collision_get(id);
  Djinni_Position* position = djinni_ecs_component_position_get(id);

  djinni_rectangle_concern_set_center_bound_offsets(
    width,
    height,
    parent_width,
    parent_height,
    &(collision_box->parent_x_offset),
    &(collision_box->parent_y_offset)
  );

  return id;
}