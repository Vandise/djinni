#ifndef DJINNI_VIDEO_RENDERER
#define DJINNI_VIDEO_RENDERER 1

#include "djinni/video/shared.h"

void djinni_video_renderer_initialize(int index, int flags);
void djinni_video_renderer_reset_draw_color();
void djinni_video_renderer_set_background_color(Color c);
void djinni_video_renderer_set_draw_color(Color c);
void djinni_video_renderer_present();
void djinni_video_renderer_clear();
void djinni_video_renderer_destroy();

#endif