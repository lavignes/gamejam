#include <stdio.h>
#include <math.h>

#include "bootstrap.h"

static inline void render(GameState* gs);
int main(int argc, char** argv) {

  boot_init();

  GameState gs;

  // Load everything importante~ into the gamestate
  game_init(&gs);

  glfwSetWindowTitle("Space Training");
  glClearColor(0.13725490196f, 0.25882352941f, 0.2f, 1.0f); // dark background

  float t0, t1, dt;
  t0 = glfwGetTime();
  // Game loop
  while (glfwGetWindowParam(GLFW_OPENED) == true) {
    t1 = glfwGetTime();
    dt = t1 - t0;
    t0 = t1;

    if (gs.frames > 60) {

      gs.fps = gs.frames / (t1 - gs.dt);
      printf("%.1lf fps\n", gs.fps);
      gs.dt = glfwGetTime();
      gs.frames = 0;
    }
    gs.frames++;

    
    if (glfwGetKey(GLFW_KEY_UP)) {
      gs.player_vy -= 100 * dt;
    }

    if (glfwGetKey(GLFW_KEY_DOWN)) {
      gs.player_vy += 100 * dt;
    }

    if (glfwGetKey(GLFW_KEY_LEFT)) {
      gs.player_vx -= 100 * dt;
    }

    if (glfwGetKey(GLFW_KEY_RIGHT)) {
      gs.player_vx += 100 * dt;
    }

    gs.player_frame += dt;
    gs.player_x += gs.player_vx * dt;
    gs.player_y += gs.player_vy * dt;

    render(&gs);
  }
  
  boot_shutdown();

  return 0;
}

static inline void render(GameState* gs) {

  int i, j;

  // Clear screen
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(gs->tiles_program->id);

  // Bind the tiles to texture 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gs->tiles_sheet);
  glUniform1i(gs->tiles_program->uniform[1], 0);

  // Bind tiles vbo
  glBindBuffer(GL_ARRAY_BUFFER, gs->tiles_vbo);
  glEnableVertexAttribArray(gs->tiles_program->attribute[0]);
  glVertexAttribPointer(gs->tiles_program->attribute[0], 1, GL_FLOAT, false, 5*sizeof(float), (void*)0);
  glEnableVertexAttribArray(gs->tiles_program->attribute[1]);
  glVertexAttribPointer(gs->tiles_program->attribute[1], 2, GL_FLOAT, false, 5*sizeof(float), (void*)(1 * sizeof(float)));
  glEnableVertexAttribArray(gs->tiles_program->attribute[2]);
  glVertexAttribPointer(gs->tiles_program->attribute[2], 2, GL_FLOAT, false, 5*sizeof(float), (void*)(3 * sizeof(float)));

  for (j = 0; j < 19; j++) {
    for (i = 0; i < 21; i++) {
      if (gs->tilemap[i][j] == 0) continue;
      glUniform2f(gs->tiles_program->uniform[0], 8*i, 8*j);
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
  }

  glDisableVertexAttribArray(gs->tiles_program->attribute[0]);
  glDisableVertexAttribArray(gs->tiles_program->attribute[1]);
  glDisableVertexAttribArray(gs->tiles_program->attribute[2]);

  glUseProgram(gs->player_program->id);

  //glUniform2f(gs->player_program->uniform[0], roundf(gs->player_x), roundf(gs->player_y));
  glUniform2f(gs->player_program->uniform[0], gs->player_x, gs->player_y);

  // Bind the player to texture 0
  glBindTexture(GL_TEXTURE_2D, gs->player_spritesheet);
  glUniform1i(gs->player_program->uniform[1], 0);

  // Bind player vbo
  glBindBuffer(GL_ARRAY_BUFFER, gs->player_vbo);
  glEnableVertexAttribArray(gs->player_program->attribute[0]);
  glVertexAttribPointer(gs->player_program->attribute[0], 2, GL_FLOAT, false, 4*sizeof(float), (void*)0);
  glEnableVertexAttribArray(gs->player_program->attribute[1]);
  glVertexAttribPointer(gs->player_program->attribute[1], 2, GL_FLOAT, false, 4*sizeof(float), (void*)(2 * sizeof(float)));

  glDrawArrays(GL_TRIANGLE_FAN, 4*((int)gs->player_frame % 2), 4);

  glDisableVertexAttribArray(gs->player_program->attribute[0]);
  glDisableVertexAttribArray(gs->player_program->attribute[1]);

  glfwSwapBuffers();
}