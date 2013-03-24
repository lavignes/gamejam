#ifndef PLAYER_H
#define PLAYER_H

/**
 * Format x, y, u, v
 * the player sheet uses 2 frames next to eachother
 * all sprites face left by default, and are
 * ordered
 * left, down, up
 */

static float PLAYER_VBO[] = {

  // Frame 1
  0,   0,   0,   0,
  0,  16,   0, .25,
  16, 16, .25, .25,
  16,  0, .25,   0,

  // Frame 2
  16,  0, .25,   0,
  16, 16, .25, .25,
  32, 16,  .5, .25,
  32,  0,  .5,   0,

};

#endif