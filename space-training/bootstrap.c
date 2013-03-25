#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vbo/player.h"

#include "bootstrap.h"

void boot_init() {

  int error;

  fail_if(glfwInit() == false);

  // No window resize
  glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, true);
  // 400% gameboy scale, 5 bit rgb with 1 bit alpha, 8 bit z buffer
  fail_if(glfwOpenWindow(160 * 4, 144 * 4, 5, 5, 5, 1, 8, 0, GLFW_WINDOW) == false);

  glfwSetWindowTitle("");

  glfwEnable(GLFW_AUTO_POLL_EVENTS); // Automatically poll input on swap
  glfwSwapInterval(1); // 0: vsync off, 1: vsync on 

  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  printf("GL Version: %s\n", glGetString(GL_VERSION));
  printf("Using GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  error = glewInit();

  if (error != GLEW_OK) {
    fprintf(stderr, "GLEW error: %s\n", glewGetErrorString(error));
  }

  printf("GL Extension Wrangler: %s\n", glewGetString(GLEW_VERSION));

  glClearColor(206.0/255.0, 230.0/255.0, 165.0/255.0, 1.0f);
  glEnable(GL_SCISSOR_TEST); // For geom culling
  
  // transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // z-buffer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Set up perspective
  glOrtho(0, 160, 144, 0, -1.0, 1.0);
  glDepthRange(-1.0f, 1.0f);
  glViewport(0, 0, 160 * 4, 144 * 4);

  // This is where Ill put the logo thing
  unsigned logo = bitmap_load("bmp/logo.bmp");
  unsigned logo_vbo;
  float t0, t1, dt;
  float logo_x = 16, logo_y = -80;
  float logo_v[] = {
    0,     0, 0, 0,
    0,   128, 0, 1,
    128, 128, 1, 1,
    128,   0, 1, 0,
  };

  glGenBuffers(1, &logo_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, logo_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(logo_v)*sizeof(float), logo_v, GL_STATIC_DRAW);
 
  Pipeline* logo_program = pipeline_new(
    shader_new(SHADER_VERTEX, "shader/logo.vert"),
    shader_new(SHADER_FRAGMENT, "shader/logo.frag"));

  pipeline_attribute(logo_program, "coord", 0);
  pipeline_attribute(logo_program, "uv", 1);
  pipeline_uniform(logo_program, "pos", 0);
  pipeline_uniform(logo_program, "texture", 1);

  glUseProgram(logo_program->id);

  // Bind the logo to texture 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, logo);
  glUniform1i(logo_program->uniform[1], 0);

  // Bind logo vbo
  glBindBuffer(GL_ARRAY_BUFFER, logo_vbo);
  glEnableVertexAttribArray(logo_program->attribute[0]);
  glVertexAttribPointer(logo_program->attribute[0], 2, GL_FLOAT, false, 4*sizeof(float), (void*)0);
  glEnableVertexAttribArray(logo_program->attribute[1]);
  glVertexAttribPointer(logo_program->attribute[1], 2, GL_FLOAT, false, 4*sizeof(float), (void*)(2 * sizeof(float)));

  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers();

  sleep(1);

  t0 = glfwGetTime();
  while (logo_y < 8) {

    t1 = glfwGetTime();
    dt = t1 - t0;
    t0 = t1;

    logo_y += 50 * dt;

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUniform2f(logo_program->uniform[0], roundf(logo_x), roundf(logo_y));

    // Render logo
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glfwSwapBuffers();
  }

  glDisableVertexAttribArray(logo_program->attribute[0]);
  glDisableVertexAttribArray(logo_program->attribute[1]);

  sleep(1);
}

float TILES_VBO[] = {

  0, 0,     0,     0,
  0, 8,     0, .0625,
  8, 8, .0625, .0625,
  8, 0, .0625,     0,
};

void game_init(GameState* gs) {

  // Bind tiles vbo
  glGenBuffers(1, &gs->tiles_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, gs->tiles_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(TILES_VBO)*sizeof(float), TILES_VBO, GL_STATIC_DRAW);

  gs->tiles_sheet = bitmap_load("bmp/tiles.bmp");
  gs->tiles_program = pipeline_new(
    shader_new(SHADER_VERTEX, "shader/tiles.vert"),
    shader_new(SHADER_FRAGMENT, "shader/tiles.frag"));

  pipeline_attribute(gs->tiles_program, "coord", 0);
  pipeline_attribute(gs->tiles_program, "uv", 1);
  pipeline_uniform(gs->tiles_program, "pos", 0);
  pipeline_uniform(gs->tiles_program, "texture", 1);
  pipeline_uniform(gs->tiles_program, "tile", 2);

  // Bind player vbo
  glGenBuffers(1, &gs->player_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, gs->player_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(PLAYER_VBO)*sizeof(float), PLAYER_VBO, GL_STATIC_DRAW);

  // load player spritesheet
  gs->player_spritesheet = bitmap_load("bmp/player.bmp");
  gs->player_x = 160/2-8;
  gs->player_y = 144/2-8;
  gs->player_vx = gs->player_vy = 0;

  // create player render pipeline
  gs->player_program = pipeline_new(
    shader_new(SHADER_VERTEX, "shader/player.vert"),
    shader_new(SHADER_FRAGMENT, "shader/player.frag"));

  pipeline_attribute(gs->player_program, "coord", 0);
  pipeline_attribute(gs->player_program, "uv", 1);
  pipeline_uniform(gs->player_program, "pos", 0);
  pipeline_uniform(gs->player_program, "texture", 1);
}

void boot_shutdown() {

  glfwTerminate();
}
