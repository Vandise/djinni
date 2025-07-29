#ifndef DJINNI_VIDEO_WINDOW
#define DJINNI_VIDEO_WINDOW 1

#include "djinni/video/shared.h"

void djinni_video_window_initialize(const char *title, int x, int y, int w, int h, int flags);
SDL_Window* djinni_video_window_get();
void djinni_video_window_destroy();

#endif