#ifndef GAME_H
#define GAME_H

#include "init.h"
#include "bitmap.h"
#include "shader.h"

static const float BALL_VBO[] = {

  0,   0,    0,   1- 0.5,
  0,  8,     0, 1- 0.625,
  8,  8, 0.125, 1- 0.625,
  8,  0, 0.125,   1- 0.5,
};

static const float PLAYER_VBO[] = {

  0,   0, 0,    1- 0,
  0,  24, 0,    1- 0.375,
  16, 24, 0.25, 1- 0.375,
  16,  0, 0.25, 1- 0,
};

static const float MAP_VBO[] = {

  0,      0, 0, 1- 0,
  0,    240, 0, 1- 0.46875,
  512, 240, 1, 1- 0.46875,
  512,   0, 1, 1- 0,
};

typedef struct Player {

  bool go_home;
  float x, y, vx, vy;
  int dir;
  float frame;

} Player;

typedef struct GameState {

  float cam_x, cam_y;
  unsigned my_player;

  int z_pressed;
  int x_pressed;

  Pipeline* ball_program;
  unsigned ball_vbo;
  unsigned ball_team;
  unsigned ball_player;
  float ball_x, ball_y;
  float ball_vx, ball_vy;
  float ball_frame;

  Pipeline* map_program;
  unsigned map_vbo;
  unsigned map_texture;

  Pipeline* player_program;
  unsigned player_vbo;
  unsigned player_spritesheet;

  Player team1[5];
  Player team2[5];

} GameState;

void game_loop();

void game_init(GameState* gs);

#endif /* GAME_H */