#include "game.h"
#include "fifa.h"

void game_loop() {
  GameState gs;
  game_init(&gs);

  float dt = 0, t0 = 0, t1 = 0;
  float ft = 0, f = 0, fps = 0;

  while (glfwGetWindowParam(GLFW_OPENED)) {
    t1 = glfwGetTime();
    dt = t1 - t0;
    t0 = t1;

    if (f > 60) {
      fps = f / (t1 - ft);
      printf("%.1lf fps\n", fps);
      ft = glfwGetTime();
      f = 0;
    }
    f++;

    fifa_step(&gs, dt);
    fifa_draw(&gs);
  }
}

void game_init(GameState* gs) {
  int i;

  gs->cam_x = 0;
  gs->cam_y = 0;

  gs->my_player = 4;

  // Bind map vbo
  glGenBuffers(1, &gs->map_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, gs->map_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(MAP_VBO)*sizeof(float), MAP_VBO, GL_STATIC_DRAW);
  gs->map_texture = bitmap_load("bmp/map.bmp");
  gs->map_program = pipeline_new(
    shader_new(SHADER_VERTEX, "shader/map.vert"),
    shader_new(SHADER_FRAGMENT, "shader/map.frag"));
  pipeline_attribute(gs->map_program, "coord", 0);
  pipeline_attribute(gs->map_program, "st", 1);
  pipeline_uniform(gs->map_program, "tex", 0);
  pipeline_uniform(gs->map_program, "pos", 1);

  // Bind player vbo
  glGenBuffers(1, &gs->player_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, gs->player_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(PLAYER_VBO)*sizeof(float), PLAYER_VBO, GL_STATIC_DRAW);

  // Load player spritesheet
  gs->player_spritesheet = bitmap_load("bmp/player.bmp");

  // create player render pipeline
  gs->player_program = pipeline_new(
    shader_new(SHADER_VERTEX, "shader/player.vert"),
    shader_new(SHADER_FRAGMENT, "shader/player.frag"));

  // define player attributes and uniforms
  pipeline_attribute(gs->player_program, "coord", 0);
  pipeline_attribute(gs->player_program, "st", 1);

  pipeline_uniform(gs->player_program, "tex", 0);
  pipeline_uniform(gs->player_program, "pos", 1);
  pipeline_uniform(gs->player_program, "frame", 2);
  pipeline_uniform(gs->player_program, "dir", 3);
  pipeline_uniform(gs->player_program, "color", 4);

  // Bind ball vbo
  glGenBuffers(1, &gs->ball_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, gs->ball_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(BALL_VBO)*sizeof(float), BALL_VBO, GL_STATIC_DRAW);
  gs->ball_program = pipeline_new(
    shader_new(SHADER_VERTEX, "shader/ball.vert"),
    shader_new(SHADER_FRAGMENT, "shader/ball.frag"));
  pipeline_attribute(gs->ball_program, "coord", 0);
  pipeline_attribute(gs->ball_program, "st", 1);
  pipeline_uniform(gs->ball_program, "tex", 0);
  pipeline_uniform(gs->ball_program, "pos", 1);
  pipeline_uniform(gs->ball_program, "frame", 2);

  gs->ball_team = 1;
  gs->ball_player = 1;
  gs->ball_x = 246;
  gs->ball_y = 100;
  gs->ball_frame = 0;

  // Setup teams
  for (i = 0; i < 6; i++) {
    gs->team1[i].go_home = false;
    gs->team1[i].x = gs->team1[i].y = 0;
    gs->team1[i].dir = gs->team1[i].frame = 0;
    gs->team2[i].go_home = false;
    gs->team2[i].x = gs->team2[i].y = 0;
    gs->team2[i].dir = gs->team2[i].frame = 0;
  }

  gs->team1[gs->my_player].x = gs->ball_x;
  gs->team1[gs->my_player].y = gs->ball_y;
}
