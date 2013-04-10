#include "fifa.h"

void fifa_step(GameState* gs, float dt) {

  gs->team1[0].frame += 4 * dt;
}

void fifa_draw(GameState* gs) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Draw map //////////////////////////////////////////////////////////////
  glUseProgram(gs->map_program->id);
  glBindBuffer(GL_ARRAY_BUFFER, gs->map_vbo);
  glEnableVertexAttribArray(gs->map_program->attribute[0]);
  glVertexAttribPointer(gs->map_program->attribute[0], 2, GL_FLOAT, false, 4*sizeof(float), (void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(gs->map_program->attribute[1]);
  glVertexAttribPointer(gs->map_program->attribute[1], 2, GL_FLOAT, false, 4*sizeof(float), (void*)(2 * sizeof(float)));
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gs->map_texture);
  glUniform1i(gs->map_program->uniform[0], 0);
  glUniform2f(gs->map_program->uniform[1], 0, 0);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableVertexAttribArray(gs->map_program->attribute[0]);
  glDisableVertexAttribArray(gs->map_program->attribute[1]);

  // Draw player ///////////////////////////////////////////////////////////
  glUseProgram(gs->player_program->id);
  // Bind player vbo
  glBindBuffer(GL_ARRAY_BUFFER, gs->player_vbo);
  glEnableVertexAttribArray(gs->player_program->attribute[0]);
  glVertexAttribPointer(gs->player_program->attribute[0], 2, GL_FLOAT, false, 4*sizeof(float), (void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(gs->player_program->attribute[1]);
  glVertexAttribPointer(gs->player_program->attribute[1], 2, GL_FLOAT, false, 4*sizeof(float), (void*)(2 * sizeof(float)));

  // Bind the player to texture 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gs->player_spritesheet);
  glUniform1i(gs->player_program->uniform[0], 0);

  // set player position frame and direction
  glUniform2f(gs->player_program->uniform[1], gs->team1[0].x, gs->team1[0].y);
  glUniform1i(gs->player_program->uniform[2], (int) gs->team1[0].frame % 3);
  glUniform1i(gs->player_program->uniform[3], gs->team1[0].dir);

  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  // cleanup
  glDisableVertexAttribArray(gs->player_program->attribute[0]);
  glDisableVertexAttribArray(gs->player_program->attribute[1]);

  glfwSwapBuffers();
}