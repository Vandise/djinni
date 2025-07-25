#include <SDL2/SDL_image.h>
#include "djinni/util/json.h"
#include "djinni/renderable/renderable.h"
#include "djinni/video/imageAtlas.h"

static void loadAtlasData(ImageAtlas* atlas) {
  cJSON *root, *node;
  char* text = djinniReadFile(atlas->dataFileName);

  root = cJSON_Parse(text);

  for (node = root->child; node != NULL; node = node->next) {
		char* filename = cJSON_GetObjectItem(node, "filename")->valuestring;

    AtlasImage* img = malloc(sizeof(AtlasImage));
    strcpy(img->filename, filename);
    img->x = cJSON_GetObjectItem(node, "x")->valueint;
    img->y = cJSON_GetObjectItem(node, "y")->valueint;
    img->w = cJSON_GetObjectItem(node, "w")->valueint;
    img->h = cJSON_GetObjectItem(node, "h")->valueint;
    img->atlas = atlas;

    Djinni_Util_Array.insert(atlas->images, img);
  }

  cJSON_Delete(root);
  free(text);
}

static ImageAtlas* load(Renderer* r, char* imageFile, char* dataFile) {
  ImageAtlas* atlas = malloc(sizeof(ImageAtlas));

  atlas->texture = Djinni_Video_Texture.load(r, imageFile);

  strcpy(atlas->imageFileName, imageFile);
  strcpy(atlas->dataFileName, dataFile);

  atlas->images = Djinni_Util_Array.initialize(16);

  loadAtlasData(atlas);

  return atlas;
}

static AtlasImage* getImage(ImageAtlas* atlas, char* filename) {
  for (int i = 0; i < atlas->images->used; i++) {
    AtlasImage* img = atlas->images->data[i];
    if (strcmp(img->filename, filename) == 0) {
      return img;
    }
  }

  return NULL;
}

static AtlasImage* getIndex(ImageAtlas* atlas, int index) {
  return atlas->images->data[index];
}

static void blit(Renderer* r, AtlasImage* img, int x, int y, int w, int h) {
	SDL_Rect src;
  	src.x = img->x;
  	src.y = img->y;
  	src.w = img->w;
  	src.h = img->h;

	SDL_Rect dest;
  	dest.x = x;
  	dest.y = y;
  	dest.w = w;
  	dest.h = h;

	SDL_RenderCopy(r->instance, img->atlas->texture->instance, &src, &dest);
}

static void destroy(ImageAtlas* atlas) {
  Djinni_Util_Array.destroy(atlas->images, free);
  Djinni_Video_Texture.destroy(atlas->texture);
  free(atlas);
}

struct Djinni_Video_ImageAtlasStruct Djinni_Video_Image_Atlas = {
  .load = load,
  .getImage = getImage,
  .getIndex = getIndex,
  .blit = blit,
  .destroy = destroy,
};