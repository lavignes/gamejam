#ifndef PLAYER_H
#define PLAYER_H

/**
 * Format x, y, s, t
 * the player sheet uses 2 frames next to eachother
 * all sprites face left by default, and are
 * ordered
 * left, down, up
 */

static float PLAYER_VBO[] = {

  // Frame 1
  0,   0,   0, 1.0 - 0,
  0,  16,   0, 1.0 - .25,
  16, 16, .25, 1.0 - .25,
  16,  0, .25, 1.0 - 0,

  // Frame 2
  0,   0, .25, 1.0 - 0,
  0,  16, .25, 1.0 - .25,
  16, 16, .5,  1.0 - .25,
  16,  0, .5,  1.0 - 0,
};

#endif