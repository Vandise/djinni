#include <SDL2/SDL_image.h>
#include "djinni/video/video.h"
#include "djinni/util/json.h"
#include "djinni/util/file.h"
#include "djinni/video/image_atlas.h"

static void load_atlas_data(Djinni_ImageAtlas* atlas) {
  cJSON *root, *node;
  char* text = djinni_read_file(atlas->data_file_name);

  root = cJSON_Parse(text);

  for (node = root->child; node != NULL; node = node->next) {
		char* filename = cJSON_GetObjectItem(node, "filename")->valuestring;

    Djinni_AtlasImage* img = malloc(sizeof(Djinni_AtlasImage));
    strcpy(img->filename, filename);
    img->x = cJSON_GetObjectItem(node, "x")->valueint;
    img->y = cJSON_GetObjectItem(node, "y")->valueint;
    img->w = cJSON_GetObjectItem(node, "w")->valueint;
    img->h = cJSON_GetObjectItem(node, "h")->valueint;
    img->atlas = atlas;

    djinni_array_insert(atlas->images, img);
  }

  cJSON_Delete(root);
  free(text);
}

Djinni_ImageAtlas* djinni_video_image_atlas_load(char* image_file, char* data_file) {
  Djinni_ImageAtlas* atlas = malloc(sizeof(Djinni_ImageAtlas));
  atlas->texture = IMG_LoadTexture(djinni_video_renderer(), image_file);

  strcpy(atlas->image_file_name, image_file);
  strcpy(atlas->data_file_name, data_file);

  atlas->images = djinni_array_initialize(16);

  load_atlas_data(atlas);

  return atlas;
}

Djinni_AtlasImage* djinni_video_image_atlas_get_image(Djinni_ImageAtlas* atlas, char* filename) {
  for (int i = 0; i < atlas->images->used; i++) {
    Djinni_AtlasImage* img = atlas->images->data[i];
    if (strcmp(img->filename, filename) == 0) {
      return img;
    }
  }

  return NULL;
}

Djinni_ImageAtlas* djinni_video_image_atlas_get_index(Djinni_ImageAtlas* atlas, int index) {
  return atlas->images->data[index];
}

void djinni_video_image_atlas_destroy(Djinni_ImageAtlas* atlas) {
  djinni_array_destroy(atlas->images, free);
  SDL_DestroyTexture(atlas->texture);
  free(atlas);
}