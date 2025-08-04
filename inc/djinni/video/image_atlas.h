#ifndef DJINNI_VIDEO_IMAGE_ATLAS
#define DJINNI_VIDEO_IMAGE_ATLAS 1

#include "djinni/video/shared.h"
#include "djinni/util/array.h"

#define DJINNI_IMAGE_ATLAS_MAX_LENGTH 128

typedef struct Djinni_Video_ImageAtlasStruct {
  char          image_file_name[DJINNI_IMAGE_ATLAS_MAX_LENGTH];
  char          data_file_name[DJINNI_IMAGE_ATLAS_MAX_LENGTH];
	SDL_Texture*  texture;

  DjinniArray* images;
} Djinni_ImageAtlas;

typedef struct Djinni_Video_AtlasImageStruct {
  char filename[DJINNI_IMAGE_ATLAS_MAX_LENGTH];

  int x;
  int y;
  int w;
  int h;

  Djinni_ImageAtlas* atlas;
} Djinni_AtlasImage;

Djinni_ImageAtlas* djinni_video_image_atlas_load(char* image_file, char* data_file);
Djinni_AtlasImage* djinni_video_image_atlas_get_image(Djinni_ImageAtlas* atlas, char* filename);
Djinni_ImageAtlas* djinni_video_image_atlas_get_index(Djinni_ImageAtlas* atlas, int index);
void djinni_video_image_atlas_destroy(Djinni_ImageAtlas* atlas);

#endif