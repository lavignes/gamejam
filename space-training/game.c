#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <AL/al.h>

#include "bootstrap.h"
#include "tile.h"

static inline void render(GameState* gs);
int main(int argc, char** argv) {

  int i;

  boot_init(&argc, argv);

  GameState gs;

  // Load everything importante~ into the gamestate
  game_init(&gs);

  tileset_load(&gs, "level/1.bmp");

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
      gs.player_vy -= 200 * dt;
    }

    if (glfwGetKey(GLFW_KEY_DOWN)) {
      gs.player_vy += 200 * dt;
    }

    if (glfwGetKey(GLFW_KEY_LEFT)) {
      gs.player_vx -= 200 * dt;
    }

    if (glfwGetKey(GLFW_KEY_RIGHT)) {
      gs.player_vx += 200 * dt;
    }

    gs.player_vx = clampf(gs.player_vx, -400, 400);
    gs.player_vy = clampf(gs.player_vy, -400, 400);

    gs.player_frame += dt;

    alSourcei(gs.player_source, AL_BUFFER, gs.bounce_sound->buffer);
    alSourcef(gs.player_source, AL_PITCH, (rand()%30)/10.0); 
    // Bouncy side walls
    if (gs.player_x < 0) {
      gs.player_x = 0;
      gs.player_vx = -gs.player_vx/2;
      alSourcePlay(gs.player_source);
    } else if (gs.player_x > 256*8-16) {
      gs.player_x = 256*8-16;
      gs.player_vx = -gs.player_vx/2;
      alSourcePlay(gs.player_source);
    }
    if (gs.player_y < 0) {
      gs.player_y = 0;
      gs.player_vy = -gs.player_vy/2;
      alSourcePlay(gs.player_source);
    } else if (gs.player_y > 256*8-16) {
      gs.player_y = 256*8-16;
      gs.player_vy = -gs.player_vy/2;
      alSourcePlay(gs.player_source);
    }

    float old_x = gs.player_x;
    float old_y = gs.player_y;

    gs.player_x += gs.player_vx * dt;

    for (i = 0; i < 3; i++) {

      if (gs.tilemap[(int)((gs.player_x + (i * 8)) / 8)][(int)(gs.player_y / 8)] == 3
          || gs.tilemap[(int)((gs.player_x + (i * 8)) / 8)][(int)(gs.player_y / 8) + 1] == 3
          || gs.tilemap[(int)((gs.player_x + (i * 8)) / 8)][(int)(gs.player_y / 8) + 2] == 3) {

        gs.player_x = old_x;
        alSourcePlay(gs.player_source);
        if (gs.player_vx < 0)
          gs.player_x = (int)(gs.player_x/8) * 8;
        
        else if (gs.player_vx > 0)
          gs.player_x = (int)(((gs.player_x + 16) / 8) * 8) - 16;

        gs.player_vx = -gs.player_vx/2;
      }
    }

    gs.player_y += gs.player_vy * dt;

    for (i = 0; i < 3; i++) {

      if (gs.tilemap[(int)(gs.player_x / 8)][(int)((gs.player_y + (i * 8)) / 8)] == 3
          || gs.tilemap[(int)(gs.player_x / 8) + 1][(int)((gs.player_y + (i * 8)) / 8)] == 3
          || gs.tilemap[(int)(gs.player_x / 8) + 2][(int)((gs.player_y + (i * 8)) / 8)] == 3) {

        gs.player_y = old_y;
        alSourcePlay(gs.player_source);
        if (gs.player_vy < 0)
          gs.player_y = (int)(gs.player_y/8) * 8;
        
        else if (gs.player_vy > 0)
          gs.player_y = (int)(((gs.player_y + 16) / 8) * 8) - 16;

        gs.player_vy = -gs.player_vy/2;
      }
    }

    // camera follow
    gs.cam_x = gs.player_x - 72;
    if (gs.cam_x < 0) gs.cam_x = 0;
    else if (gs.cam_x > 256*8-160) gs.cam_x = 256*8-160;

    gs.cam_y = gs.player_y - 64;
    if (gs.cam_y < 0) gs.cam_y = 0;
    else if (gs.cam_y > 256*8-144) gs.cam_y = 256*8-144;

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
  glVertexAttribPointer(gs->tiles_program->attribute[0], 2, GL_FLOAT, false, 4*sizeof(float), (void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(gs->tiles_program->attribute[1]);
  glVertexAttribPointer(gs->tiles_program->attribute[1], 2, GL_FLOAT, false, 4*sizeof(float), (void*)(2 * sizeof(float)));

  int t_x = gs->cam_x/8;
  int t_y = gs->cam_y/8;

  for (j = t_y; j < 19+t_y; j++) {
    for (i = t_x; i < 21+t_x; i++) {
      if (gs->tilemap[i][j] == 0) continue;
      glUniform2f(gs->tiles_program->uniform[2], gs->tilemap[i][j]%16, gs->tilemap[i][j]/16);
      glUniform2f(gs->tiles_program->uniform[0], roundf(8*i-gs->cam_x), roundf(8*j-gs->cam_y));
      glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
  }

  glDisableVertexAttribArray(gs->tiles_program->attribute[0]);
  glDisableVertexAttribArray(gs->tiles_program->attribute[1]);

  glUseProgram(gs->player_program->id);

  glUniform2f(gs->player_program->uniform[0], roundf(gs->player_x-gs->cam_x), roundf(gs->player_y-gs->cam_y));

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