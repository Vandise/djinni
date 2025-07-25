# Djinni

Djinni is a 2d Game Engine built with the isometric perspective in mind.

## Dependencies

- SDL2 (sdl2)
- SDL2 Image (sdl2_image)
- SDL2 Mixer (sdl2_mixer)
- SDL2 TTF (sdl2_ttf)

## Design

Djinni is architected to have stages ("worlds") and entities manage their own lifecycles while the engine processes different phases of execution.

### Stage Lifecycle

Stages manage the current world state. They load data, can draw UI components, or handle can entity-related events. Once a stage is flagged as "ready", the game loop will trigger *update* events on entities.

### onCreate

*Executes only once when an initial stage is set, or the stage is changed.*

Configures the world, enables inputs, and sets up any pre-defined entities. May also load data in the background or game-loop blocking.

### prepare

*Executes every frame after Djinni sets the user-defined background color and clears the frame.*

Prepares the world. Sets background colors, draws background images, checks or sets the ready state based on data loading in the background.

### update

*Executes every frame after all entities have had their updates triggered.*

Modifies the world through time-based triggers and state checks.

### draw

*Executes every frame after entities and the world has been drawn.*

Draws to the screen. HUD components and User Interface menus.

### onDestroy

*Executes once on stage change.*

Triggered to clean up memory allocations.

## Grid

Djinni uses a grid-based approach to manage physics, collisions, and drawing. There are three rings: Fine, Medium, and Coarse.

### Fine

The finest ring represents everything in the camera viewport. It uses the smallest grid size (configurable) to detect fine-level interactions between entities and the environment. It is updated and every frame.

### Medium

The medium ring contains everything within 2x outside the viewport, but not the viewport itself. It consists of an intermediate grid size (configurable) that ticks updates every 6 frames (at 60FPS) by default (configurable).

### Coarse

The coarse ring contains everything 2x outside the viewport, but not anything within 2x of the viewport. It consists of a coarse grid size (configurable) that ticks updates every 54 frames (at 60FPS) by default (configurable).

## Entities

Entities represent physical objects in the world that can interact with one another. They manage their own lifecycles and state.

## Update

*Executes based on position in the grid.*

Handles user inputs for the player and logic for non-playable objects.

## onCollide

*Executes when an entity interacts with another.*

Determines how to handle when an entity interacts with another.

## onEnterViewport

*Executes when an entity enters the viewport.*

Triggers when an entity enters the viewport.

## onExitViewport

*Executes when an entity exits the viewport.*

Triggers when an entity exits the viewport.

## onDestroy

*Executes when an entity's status is set to destroyed.*

Triggers when an entity is to be destroyed. User for creating other entities and effects based on one's destruction and clearing user data.

## onTerminate

*Executes when the game terminates and an entity is still present on the screen.*
 
Triggers to clean up user data if an entity still exists within the grid.
 