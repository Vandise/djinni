#ifndef DJINNI_ECS_SYSTEMS_ENTITY_DISPATCH_SYSTEM
#define DJINNI_ECS_SYSTEMS_ENTITY_DISPATCH_SYSTEM 1

#include "djinni/ecs/shared.h"
#include "djinni/grid/grid.h"
#include "djinni/ecs/systems/collision_system.h"

void djinni_ecs_system_dispatch(DJINNI_GRID_RING ring, double dt);

#endif