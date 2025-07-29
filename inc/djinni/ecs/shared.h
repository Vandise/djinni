#ifndef DJINNI_ECS_SHARED
#define DJINNI_ECS_SHARED 1

#include <stdlib.h>

#define DjinniEntityId int

#define DJINNI_COMPONENT_CONTROLLABLE (1 << 0)
#define DJINNI_COMPONENT_POSITION     (1 << 1)
#define DJINNI_COMPONENT_MOVEABLE     (1 << 2)
#define DJINNI_COMPONENT_COLLIDABLE   (1 << 3)
#define DJINNI_COMPONENT_SPRITE       (1 << 4)

#endif