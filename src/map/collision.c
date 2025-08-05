#include "djinni/map/map.h"

static void load_definitions(Djinni_Map* djinni_map, cJSON* definitions_node) {
  int n_objects = cJSON_GetArraySize(definitions_node);

  djinni_map->collision_definitions = djinni_array_initialize(n_objects);

  for (int i = 0; i < n_objects; i++) {
    cJSON* definition_node = cJSON_GetArrayItem(definitions_node, i);

    Djinni_MapCollisionDefinition* definition = malloc(sizeof(Djinni_MapCollisionDefinition));
      strncpy(definition->name, cJSON_GetObjectItem(definition_node, "name")->valuestring, DJINNI_MAP_MAX_DEFINITION_LEN);
      definition->shapes = NULL;

    cJSON* shapes_node = cJSON_GetObjectItem(definition_node, "shapes");

    if (shapes_node != NULL) {
      int n_shapes = cJSON_GetArraySize(shapes_node);

      definition->shapes = djinni_array_initialize(n_shapes);

      for (int j = 0; j < n_shapes; j++) {
        cJSON* shape_node = cJSON_GetArrayItem(shapes_node, j);
          cJSON* type = cJSON_GetObjectItem(shape_node, "type");
          cJSON*  x1 = cJSON_GetObjectItem(shape_node, "x1");
          cJSON*  x2 = cJSON_GetObjectItem(shape_node, "x2");
          cJSON*  width = cJSON_GetObjectItem(shape_node, "width");
          cJSON*  height = cJSON_GetObjectItem(shape_node, "height");

        Djinni_MapCollisionShape* shape = malloc(sizeof(Djinni_MapCollisionShape));
          shape->type = type->valueint;
          shape->x1 = x1 != NULL ? x1->valueint : -1;
          shape->x2 = x2 != NULL ? x2->valueint : -1;
          shape->width = width != NULL ? width->valueint : -1;
          shape->height = height != NULL ? height->valueint : -1;

        djinni_array_insert(definition->shapes, shape);
      }
    }

    djinni_array_insert(djinni_map->collision_definitions, definition);

  }
}

void djinni_map_collision_definitions_load(Djinni_Map* djinni_map, cJSON* root) {
  djinni_map->collision_definitions = NULL;

  cJSON* collision_node = cJSON_GetObjectItem(root, "collision");

  if (collision_node != NULL) {
    cJSON* definitions_node = cJSON_GetObjectItem(collision_node, "definitions");
    if (definitions_node != NULL) { load_definitions(djinni_map, definitions_node); }
  }
}