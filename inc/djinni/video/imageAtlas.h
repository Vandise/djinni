#ifndef DJINNI_VIDEO_IMAGE_ATLAS
#define DJINNI_VIDEO_IMAGE_ATLAS 1

#include "djinni/video/shared.h"
#include "djinni/util/shared.h"
#include "djinni/video/texture.h"
#include "djinni/util/array.h"

#define DJINNI_IMAGE_ATLAS_MAX_LENGTH 128

typedef struct Djinni_Video_ImageAtlas {
  char         imageFileName[DJINNI_IMAGE_ATLAS_MAX_LENGTH];
  char         dataFileName[DJINNI_IMAGE_ATLAS_MAX_LENGTH];
	Texture*     texture;

  DjinniArray* images;
} ImageAtlas;

typedef struct Djinni_Video_AtlasImage {
  char filename[DJINNI_IMAGE_ATLAS_MAX_LENGTH];

  int x;
  int y;
  int w;
  int h;

  ImageAtlas* atlas;
} AtlasImage;

struct Djinni_Video_ImageAtlasStruct {
  ImageAtlas* (*load)(Renderer*, char*, char*);
  AtlasImage* (*getIndex)(ImageAtlas*, int);
  void (*blit)(Renderer*, AtlasImage*, int, int, int, int);
  AtlasImage* (*getImage)(ImageAtlas*, char*);
};

extern struct Djinni_Video_ImageAtlasStruct Djinni_Video_Image_Atlas;

#endif