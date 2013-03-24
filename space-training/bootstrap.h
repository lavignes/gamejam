#ifndef BOOTSTRAP_H 
#define BOOTSTRAP_H

#include <assert.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "shader.h"
#include "bitmap.h"

#define fail_if(x) do { if (x) assert(false); } while (false);

typedef struct GameState {

  long unsigned frames; // # of frames rendered
  double dt;            // dt of physics
  double fps;

  unsigned player_vbo;  // vbo id for player
  unsigned player_spritesheet;

} GameState;

void boot_init();
void boot_shutdown();

void game_init(GameState* gs);

#endif