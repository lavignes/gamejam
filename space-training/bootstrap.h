#ifndef BOOTSTRAP_H 
#define BOOTSTRAP_H

#include <unistd.h>
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

  float cam_x;
  float cam_y;

  unsigned tiles_vbo;
  unsigned tiles_sheet;
  Pipeline* tiles_program;

  unsigned player_vbo;  // vbo id for player
  unsigned player_spritesheet;
  float player_frame;
  float player_vx, player_vy;
  float player_x, player_y;
  Pipeline* player_program;

  char tilemap[256][256];

} GameState;

void boot_init();
void boot_shutdown();

void game_init(GameState* gs);

#endif