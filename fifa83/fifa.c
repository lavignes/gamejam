#include "fifa.h"

#define clampf(v, min, max) (v < min)? min : (v > max)? max : v

#define centerf(v, fric, tol) (v < -tol)? v + fric : (v > tol)? v - fric : 0

bool bb_intersect(float x1, float w1, float y1, float h1, float x2, float w2, float y2, float h2) {
  return (x1 <= w2 && x2 <= w1 && y1 <= h2 && y2 <= h1);
}

void fifa_step(GameState* gs, float dt) {

  unsigned i;
  if (glfwGetKey(GLFW_KEY_UP))
    gs->team1[gs->my_player].vy -= 500 * dt;
  else if (glfwGetKey(GLFW_KEY_DOWN))
    gs->team1[gs->my_player].vy += 500 * dt;

  if (glfwGetKey(GLFW_KEY_LEFT)) gs->team1[gs->my_player].vx -= 500 * dt;
  else if (glfwGetKey(GLFW_KEY_RIGHT))
    gs->team1[gs->my_player].vx += 500 * dt;

  if (gs->z_pressed == GLFW_RELEASE) {
    if ((gs->z_pressed = glfwGetKey('Z')) == GLFW_PRESS)
      gs->my_player = (gs->my_player + 1) % 5;
  } else gs->z_pressed = glfwGetKey('Z');

  if (gs->x_pressed == GLFW_RELEASE) {
    if ((gs->x_pressed = glfwGetKey('X')) == GLFW_PRESS) {
      if (gs->ball_team == 1 && gs->ball_player == gs->my_player) {
        gs->ball_team = 0;
        gs->ball_x = gs->ball_x + ((gs->team1[gs->ball_player].dir)? -8 : 8);
        gs->ball_vx = gs->team1[gs->ball_player].vx * 2;
        gs->ball_vy = gs->team1[gs->ball_player].vy * 2;
      }
    }
  } else gs->x_pressed = glfwGetKey('X');

  gs->cam_x = clampf(gs->team1[gs->my_player].x - 120, 0, 256);

  if (gs->ball_team == 1) {
    gs->ball_x = gs->team1[gs->ball_player].x + ((gs->team1[gs->ball_player].dir)? -4 : 12);
    gs->ball_y = gs->team1[gs->ball_player].y+18;
    gs->ball_frame += sqrtf(powf(gs->team1[gs->ball_player].vx, 2) + powf(gs->team1[gs->ball_player].vy, 2)) * 0.1 * dt;

    if (gs->ball_player != 0)
    for (i = 0; i < 5; i++) {
      if (bb_intersect(gs->team2[i].x, gs->team2[i].x+16, gs->team2[i].y+16, gs->team2[i].y+24,
                       gs->ball_x, gs->ball_x+8, gs->ball_y, gs->ball_y+8)) {
        gs->ball_team = 2;
        gs->ball_player = i;
        break;
      }
    }

  } else if (gs->ball_team == 2) {
    gs->ball_x = gs->team2[gs->ball_player].x + ((gs->team2[gs->ball_player].dir)? -4 : 12);
    gs->ball_y = gs->team2[gs->ball_player].y+18;
    gs->ball_frame += sqrtf(powf(gs->team2[gs->ball_player].vx, 2) + powf(gs->team2[gs->ball_player].vy, 2)) * 0.1 * dt;

    if (gs->ball_player != 0)
    for (i = 0; i < 5; i++) {
      if (bb_intersect(gs->team1[i].x, gs->team1[i].x+16, gs->team1[i].y+16, gs->team1[i].y+24,
                       gs->ball_x, gs->ball_x+8, gs->ball_y, gs->ball_y+8)) {
        gs->ball_team = 1;
        gs->my_player = gs->ball_player = i;
        break;
      }
    }
  } else {
    gs->ball_vx = centerf(gs->ball_vx, 200 * dt, 1);
    gs->ball_vy = centerf(gs->ball_vy, 200 * dt, 1);
    gs->ball_x += gs->ball_vx * dt;
    gs->ball_y += gs->ball_vy * dt;
    gs->ball_frame += sqrtf(powf(gs->ball_vx, 2) + powf(gs->ball_vy, 2)) * 0.1 * dt;

    gs->ball_x = clampf(gs->ball_x, 26, 482);
    gs->ball_y = clampf(gs->ball_y, 22, 204);

    for (i = 0; i < 5; i++) {
      if (bb_intersect(gs->team1[i].x, gs->team1[i].x+16, gs->team1[i].y+16, gs->team1[i].y+24,
                       gs->ball_x, gs->ball_x+8, gs->ball_y, gs->ball_y+8)) {
        gs->ball_team = 1;
        gs->my_player = gs->ball_player = i;
        break;
      }
    }

    for (i = 0; i < 5; i++) {
      if (bb_intersect(gs->team2[i].x, gs->team2[i].x+16, gs->team2[i].y+16, gs->team2[i].y+24,
                       gs->ball_x, gs->ball_x+8, gs->ball_y, gs->ball_y+8)) {
        gs->ball_team = 2;
        gs->ball_player = i;
        break;
      }
    }
  }

  for (i = 0; i < 5; i++) {

    if (i != gs->my_player) {
      switch (i) {
        case 0: ai_goalie1(gs, i, dt); break;
        case 1: ai_centerl1(gs, i, dt); break;
        case 2: ai_centerr1(gs, i, dt); break;
        case 3: ai_forwardl1(gs, i, dt); break;
        case 4: ai_forwardr1(gs, i, dt); break;
      }
    } else {
      gs->team1[i].go_home = true;
    }

    // Clamp
    gs->team1[i].vx = clampf(gs->team1[i].vx, -80, 80);
    gs->team1[i].vy = clampf(gs->team1[i].vy, -50, 50);
    // Friction
    gs->team1[i].vx = centerf(gs->team1[i].vx, 200 * dt, 1);
    gs->team1[i].vy = centerf(gs->team1[i].vy, 200 * dt, 1);

    gs->team1[i].dir = (gs->team1[i].vx < 0);
    gs->team1[i].x += gs->team1[i].vx * dt;
    gs->team1[i].y += gs->team1[i].vy * dt;
    // Restrict
    gs->team1[i].x = clampf(gs->team1[i].x, 42, 458);
    gs->team1[i].y = clampf(gs->team1[i].y, 22, 188);
    gs->team1[i].frame += sqrtf(powf(gs->team1[i].vx, 2) + powf(gs->team1[i].vy, 2)) * 0.1 * dt;
  }

  for (i = 0; i < 5; i++) {

    switch (i) {
      case 0: ai_goalie2(gs, i, dt); break;
      case 1: ai_centerl2(gs, i, dt); break;
      case 2: ai_centerr2(gs, i, dt); break;
      case 3: ai_forwardl2(gs, i, dt); break;
      case 4: ai_forwardr2(gs, i, dt); break;
    }

    // Clamp
    gs->team2[i].vx = clampf(gs->team2[i].vx, -80, 80);
    gs->team2[i].vy = clampf(gs->team2[i].vy, -50, 50);
    // Friction
    gs->team2[i].vx = centerf(gs->team2[i].vx, 200 * dt, 1);
    gs->team2[i].vy = centerf(gs->team2[i].vy, 200 * dt, 1);

    gs->team2[i].dir = (gs->team2[i].vx <= 0);
    gs->team2[i].x += gs->team2[i].vx * dt;
    gs->team2[i].y += gs->team2[i].vy * dt;
    // Restrict
    gs->team2[i].x = clampf(gs->team2[i].x, 42, 458);
    gs->team2[i].y = clampf(gs->team2[i].y, 22, 188);
    gs->team2[i].frame += sqrtf(powf(gs->team2[i].vx, 2) + powf(gs->team2[i].vy, 2)) * 0.1 * dt;
  }
}

void fifa_draw(GameState* gs) {
  unsigned i;
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
  glUniform2f(gs->map_program->uniform[1], 0 - gs->cam_x, 0);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableVertexAttribArray(gs->map_program->attribute[0]);
  glDisableVertexAttribArray(gs->map_program->attribute[1]);


  // Draw map //////////////////////////////////////////////////////////////
  glUseProgram(gs->ball_program->id);
  glBindBuffer(GL_ARRAY_BUFFER, gs->ball_vbo);
  glEnableVertexAttribArray(gs->ball_program->attribute[0]);
  glVertexAttribPointer(gs->ball_program->attribute[0], 2, GL_FLOAT, false, 4*sizeof(float), (void*)(0 * sizeof(float)));
  glEnableVertexAttribArray(gs->ball_program->attribute[1]);
  glVertexAttribPointer(gs->ball_program->attribute[1], 2, GL_FLOAT, false, 4*sizeof(float), (void*)(2 * sizeof(float)));
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, gs->player_spritesheet);
  glUniform1i(gs->ball_program->uniform[0], 0);
  glUniform2f(gs->ball_program->uniform[1], gs->ball_x - gs->cam_x, gs->ball_y);
  glUniform1i(gs->ball_program->uniform[2], (int) gs->ball_frame % 3);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableVertexAttribArray(gs->ball_program->attribute[0]);
  glDisableVertexAttribArray(gs->ball_program->attribute[1]);

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

  for (i = 0; i < 5; i++) {
    // set player position frame and direction
    glUniform2f(gs->player_program->uniform[1], gs->team1[i].x - gs->cam_x, gs->team1[i].y);
    glUniform1i(gs->player_program->uniform[2], (int) gs->team1[i].frame % 3);
    glUniform1i(gs->player_program->uniform[3], gs->team1[i].dir);
    if (i == gs->my_player)
      glUniform3f(gs->player_program->uniform[4], 0.4, 0.4, 1);
    else glUniform3f(gs->player_program->uniform[4], 0, 0, 1);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  }

  for (i = 0; i < 5; i++) {
    // set player position frame and direction
    glUniform2f(gs->player_program->uniform[1], gs->team2[i].x - gs->cam_x, gs->team2[i].y);
    glUniform1i(gs->player_program->uniform[2], (int) gs->team2[i].frame % 3);
    glUniform1i(gs->player_program->uniform[3], gs->team2[i].dir);
    glUniform3f(gs->player_program->uniform[4], 1, 0, 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  }

  // cleanup
  glDisableVertexAttribArray(gs->player_program->attribute[0]);
  glDisableVertexAttribArray(gs->player_program->attribute[1]);

  glfwSwapBuffers();
}

void ai_goalie1(GameState* gs, unsigned pos, float dt) {
  if (gs->ball_x < 246 && gs->ball_team != 1) {
    if (gs->team1[pos].x < gs->ball_x)
      gs->team1[pos].vx += 300 * dt;
    else if (gs->team1[pos].x > gs->ball_x)
      gs->team1[pos].vx -= 300 * dt;
    if (gs->team1[pos].y < gs->ball_y)
      gs->team1[pos].vy += 300 * dt;
    else if (gs->team1[pos].y > gs->ball_y)
      gs->team1[pos].vy -= 300 * dt;
  }

  if (!gs->team1[pos].go_home) {
    gs->team1[pos].x = clampf(gs->team1[pos].x, 42, 58);
    gs->team1[pos].y = clampf(gs->team1[pos].y, 44, 154);
  } else {
    if (gs->team1[pos].x > 86) gs->team1[pos].vx -= 300 * dt;
    else gs->team1[pos].go_home = false;
  }
}

void ai_forwardl1(GameState* gs, unsigned pos, float dt) {
  if (gs->ball_x > 246 && gs->ball_team != 1) {
    if (gs->team1[pos].x < gs->ball_x)
      gs->team1[pos].vx += 300 * dt;
    else if (gs->team1[pos].x > gs->ball_x)
      gs->team1[pos].vx -= 300 * dt;
    if (gs->ball_y-16 < 100) {
      if (gs->team1[pos].y < gs->ball_y-16)
        gs->team1[pos].vy += 300 * dt;
      else if (gs->team1[pos].y > gs->ball_y-16)
        gs->team1[pos].vy -= 300 * dt;
    }
  }

  if (!gs->team1[pos].go_home) {
    gs->team1[pos].x = clampf(gs->team1[pos].x, 246, 458);
    gs->team1[pos].y = clampf(gs->team1[pos].y, 22, 100);
  } else {
    if (gs->team1[pos].x < 246) gs->team1[pos].vx += 300 * dt;
    else gs->team1[pos].go_home = false;
  }
}

void ai_forwardr1(GameState* gs, unsigned pos, float dt) {
  if (gs->ball_x > 246 && gs->ball_team != 1) {
    if (gs->team1[pos].x < gs->ball_x)
      gs->team1[pos].vx += 300 * dt;
    else if (gs->team1[pos].x > gs->ball_x)
      gs->team1[pos].vx -= 300 * dt;
    if (gs->ball_y-16 > 100) {
      if (gs->team1[pos].y < gs->ball_y-16)
        gs->team1[pos].vy += 300 * dt;
      else if (gs->team1[pos].y > gs->ball_y-16)
        gs->team1[pos].vy -= 300 * dt;
    }
  }

  if (!gs->team1[pos].go_home) {
    gs->team1[pos].x = clampf(gs->team1[pos].x, 246, 458);
    gs->team1[pos].y = clampf(gs->team1[pos].y, 100, 188);
  } else {
    if (gs->team1[pos].x < 246) gs->team1[pos].vx += 300 * dt;
    else gs->team1[pos].go_home = false;
  }
}

void ai_centerl1(GameState* gs, unsigned pos, float dt) {
  if (gs->ball_x < 246 && gs->ball_team != 1) {
    if (gs->team1[pos].x < gs->ball_x)
      gs->team1[pos].vx += 300 * dt;
    else if (gs->team1[pos].x > gs->ball_x)
      gs->team1[pos].vx -= 300 * dt;
    if (gs->ball_y-16 < 100) {
      if (gs->team1[pos].y < gs->ball_y-16)
        gs->team1[pos].vy += 300 * dt;
      else if (gs->team1[pos].y > gs->ball_y-16)
        gs->team1[pos].vy -= 300 * dt;
    }
  }

  if (!gs->team1[pos].go_home) {
    gs->team1[pos].x = clampf(gs->team1[pos].x, 58, 246);
    gs->team1[pos].y = clampf(gs->team1[pos].y, 22, 100);
  } else {
    if (gs->team1[pos].x > 246) gs->team1[pos].vx -= 300 * dt;
    else gs->team1[pos].go_home = false;
  }
}

void ai_centerr1(GameState* gs, unsigned pos, float dt) {
  if (gs->ball_x < 246 && gs->ball_team != 1) {
    if (gs->team1[pos].x < gs->ball_x)
      gs->team1[pos].vx += 300 * dt;
    else if (gs->team1[pos].x > gs->ball_x)
      gs->team1[pos].vx -= 300 * dt;
    if (gs->ball_y-16 > 100) {
      if (gs->team1[pos].y < gs->ball_y-16)
        gs->team1[pos].vy += 300 * dt;
      else if (gs->team1[pos].y > gs->ball_y-16)
        gs->team1[pos].vy -= 300 * dt;
    }
  }

  if (!gs->team1[pos].go_home) {
    gs->team1[pos].x = clampf(gs->team1[pos].x, 58, 246);
    gs->team1[pos].y = clampf(gs->team1[pos].y, 100, 188);
  } else {
    if (gs->team1[pos].x > 246) gs->team1[pos].vx -= 300 * dt;
    else gs->team1[pos].go_home = false;
  }
}

void ai_goalie2(GameState* gs, unsigned pos, float dt) {
  if (gs->ball_x > 246 && gs->ball_team != 2) {
    if (gs->team2[pos].x < gs->ball_x)
      gs->team2[pos].vx += 300 * dt;
    else if (gs->team2[pos].x > gs->ball_x)
      gs->team2[pos].vx -= 300 * dt;
    if (gs->team2[pos].y < gs->ball_y-16)
      gs->team2[pos].vy += 300 * dt;
    else if (gs->team2[pos].y > gs->ball_y-16)
      gs->team2[pos].vy -= 300 * dt;
  }

  if (gs->ball_team == 2 && gs->ball_player == pos) {
    gs->team2[pos].vx -= 300 * dt;
    gs->team2[pos].go_home = true;
    if (gs->team2[pos].x < 66) {
      gs->ball_team = 0;
      gs->ball_x = gs->ball_x + ((gs->team2[gs->ball_player].dir)? -8 : 8);
      gs->ball_vx = gs->team2[gs->ball_player].vx * 2;
      gs->ball_vy = gs->team2[gs->ball_player].vy * ((gs->team2[gs->ball_player].y > 100)? -2 : 2);
    }
  }

  else if (!gs->team2[pos].go_home) {
    gs->team2[pos].x = clampf(gs->team2[pos].x, 440, 458);
    gs->team2[pos].y = clampf(gs->team2[pos].y, 44, 154);
  } else {
    if (gs->team2[pos].x < 430) gs->team2[pos].vx += 300 * dt;
    else gs->team2[pos].go_home = false;
  }
}

void ai_forwardl2(GameState* gs, unsigned pos, float dt) {
  if (gs->ball_x < 246 && gs->ball_team != 2) {
    if (gs->team2[pos].x < gs->ball_x)
      gs->team2[pos].vx += 300 * dt;
    else if (gs->team2[pos].x > gs->ball_x)
      gs->team2[pos].vx -= 300 * dt;
    if (gs->ball_y-16 < 100) {
      if (gs->team2[pos].y < gs->ball_y-16)
        gs->team2[pos].vy += 300 * dt;
      else if (gs->team2[pos].y > gs->ball_y-16)
        gs->team2[pos].vy -= 300 * dt;
    }
  }

  if (gs->ball_team == 2 && gs->ball_player == pos) {
    gs->team2[pos].vx -= 300 * dt;
    gs->team2[pos].go_home = true;
    if (gs->team2[pos].x < 66) {
      gs->ball_team = 0;
      gs->ball_x = gs->ball_x + ((gs->team2[gs->ball_player].dir)? -8 : 8);
      gs->ball_vx = gs->team2[gs->ball_player].vx * 2;
      gs->ball_vy = gs->team2[gs->ball_player].vy * ((gs->team2[gs->ball_player].y > 100)? -2 : 2);
    }
  }

  else if (!gs->team2[pos].go_home) {
    gs->team2[pos].x = clampf(gs->team2[pos].x, 58, 246);
    gs->team2[pos].y = clampf(gs->team2[pos].y, 22, 100);
  } else {
    if (gs->team2[pos].x > 246) gs->team2[pos].vx -= 300 * dt;
    else gs->team2[pos].go_home = false;
  }
}

void ai_forwardr2(GameState* gs, unsigned pos, float dt) {
  if (gs->ball_x < 246 && gs->ball_team != 2) {
    if (gs->team2[pos].x < gs->ball_x)
      gs->team2[pos].vx += 300 * dt;
    else if (gs->team2[pos].x > gs->ball_x)
      gs->team2[pos].vx -= 300 * dt;
    if (gs->ball_y-16 > 100) {
      if (gs->team2[pos].y < gs->ball_y-16)
        gs->team2[pos].vy += 300 * dt;
      else if (gs->team2[pos].y > gs->ball_y-16)
        gs->team2[pos].vy -= 300 * dt;
    }
  }

  if (gs->ball_team == 2 && gs->ball_player == pos) {
    gs->team2[pos].vx -= 300 * dt;
    gs->team2[pos].go_home = true;
    if (gs->team2[pos].x < 66) {
      gs->ball_team = 0;
      gs->ball_x = gs->ball_x + ((gs->team2[gs->ball_player].dir)? -8 : 8);
      gs->ball_vx = gs->team2[gs->ball_player].vx * 2;
      gs->ball_vy = gs->team2[gs->ball_player].vy * ((gs->team2[gs->ball_player].y > 100)? -2 : 2);
    }
  }

  else if (!gs->team2[pos].go_home) {
    gs->team2[pos].x = clampf(gs->team2[pos].x, 58, 246);
    gs->team2[pos].y = clampf(gs->team2[pos].y, 100, 188);
  } else {
    if (gs->team2[pos].x > 246) gs->team2[pos].vx -= 300 * dt;
    else gs->team2[pos].go_home = false;
  }
}

void ai_centerl2(GameState* gs, unsigned pos, float dt) {
  if (gs->ball_x > 246 && gs->ball_team != 2) {
    if (gs->team2[pos].x < gs->ball_x)
      gs->team2[pos].vx += 300 * dt;
    else if (gs->team2[pos].x > gs->ball_x)
      gs->team2[pos].vx -= 300 * dt;
    if (gs->ball_y-16 < 100) {
      if (gs->team2[pos].y < gs->ball_y-16)
        gs->team2[pos].vy += 300 * dt;
      else if (gs->team2[pos].y > gs->ball_y-16)
        gs->team2[pos].vy -= 300 * dt;
    }
  }

  if (gs->ball_team == 2 && gs->ball_player == pos) {
    gs->team2[pos].vx -= 300 * dt;
    gs->team2[pos].go_home = true;
    if (gs->team2[pos].x < 66) {
      gs->ball_team = 0;
      gs->ball_x = gs->ball_x + ((gs->team2[gs->ball_player].dir)? -8 : 8);
      gs->ball_vx = gs->team2[gs->ball_player].vx * 2;
      gs->ball_vy = gs->team2[gs->ball_player].vy * ((gs->team2[gs->ball_player].y > 100)? -2 : 2);
    }
  }

  else if (!gs->team2[pos].go_home) {
    gs->team2[pos].x = clampf(gs->team2[pos].x, 246, 458);
    gs->team2[pos].y = clampf(gs->team2[pos].y, 22, 100);
  } else {
    if (gs->team2[pos].x < 246) gs->team2[pos].vx += 300 * dt;
    else gs->team2[pos].go_home = false;
  }
}

void ai_centerr2(GameState* gs, unsigned pos, float dt) {
  if (gs->ball_x > 246 && gs->ball_team != 2) {
    if (gs->team2[pos].x < gs->ball_x)
      gs->team2[pos].vx += 300 * dt;
    else if (gs->team2[pos].x > gs->ball_x)
      gs->team2[pos].vx -= 300 * dt;
    if (gs->ball_y-16 > 100) {
      if (gs->team2[pos].y < gs->ball_y-16)
        gs->team2[pos].vy += 300 * dt;
      else if (gs->team2[pos].y > gs->ball_y-16)
        gs->team2[pos].vy -= 300 * dt;
    }
  }

  if (gs->ball_team == 2 && gs->ball_player == pos) {
    gs->team2[pos].vx -= 300 * dt;
    gs->team2[pos].go_home = true;
    if (gs->team2[pos].x < 66) {
      gs->ball_team = 0;
      gs->ball_x = gs->ball_x + ((gs->team2[gs->ball_player].dir)? -8 : 8);
      gs->ball_vx = gs->team2[gs->ball_player].vx * 2;
      gs->ball_vy = gs->team2[gs->ball_player].vy * ((gs->team2[gs->ball_player].y > 100)? -2 : 2);
    }
  }

  else if (!gs->team2[pos].go_home) {
    gs->team2[pos].x = clampf(gs->team2[pos].x, 246, 458);
    gs->team2[pos].y = clampf(gs->team2[pos].y, 100, 188);
  } else {
    if (gs->team2[pos].x < 246) gs->team2[pos].vx += 300 * dt;
    else gs->team2[pos].go_home = false;
  }
}

