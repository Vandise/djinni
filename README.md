# Djinni 2d Game Engine

A 2d game engine optimized for isometric rendering.

## Entity-Component-System

Djinni uses an entity-component-system (ECS) approach to managing entities. This allows select behaviors to be applied to any user-defined entities.

### DJINNI_COMPONENT_CONTROLLABLE

Flag to query entities that are controllable by the player.

### DJINNI_COMPONENT_POSITION

Map X-Y coordinates.

### DJINNI_COMPONENT_DIRTY

Flag for determining if collision and grid checks must be executed.

### DJINNI_COMPONENT_COLLIDABLE

Whether an entity has collision detection with other entities.

### DJINNI_COMPONENT_VELOCITY

X-Y velocity of an entity.

### DJINNI_COMPONENT_SPRITE

A single texture to be drawn to the screen.

### DJINNI_COMPONENT_STATIC

Flag to optimize collision and grid update checks.

## Grid Collision System

Djinni uses a three-tier configurable grid position system for managing collision checks and triggers. Each cell is to be configured to a set amount of entities. If the cell is full, an entity will be rejected and collision checks will not be executed on it. 

```c
.grid_settings = {
  .cell_capacity = 5,
  .finest_size = 64,
  .medium_size = 128,
  .coarse_size = 256,
  .medium_ring_tick = 0.9,
  .coarse_ring_tick = 1.9
}
```

### Fine

The Finest Ring ticks every frame and consists of everything in the viewport.

### Medium

The Medium ring ticks at a customized setting and consists of everything outside 2x the viewport, but not the viewport itself.

### Coarse

The coarse ring ticks at a customized setting and consists of everything outside 2x the viewport.

## Tile Maps

To be documented.

## Video

Djinni uses SDL2 on its backend and associated libraries:

- sdl2
- sdl2_mixer
- sdl2_ttf
- sdl2_image

It is recommended that render-buffering and Video Screen Refresh Sync is enabled to eliminate frame loss and flickering.

```
Djinni_VideoSettings vs = {
  .index = 0,
  .videoFlags = IMG_INIT_PNG | IMG_INIT_JPG,
  .rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
};
```