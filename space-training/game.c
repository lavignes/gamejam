#include <stdio.h>

#include "bootstrap.h"

int main(int argc, char** argv) {

  boot_init();

  GameState gs;

  // Game loop
  while (glfwGetWindowParam(GLFW_OPENED) == true) {

    if (gs.frames > 60) {

      gs.fps = gs.frames / (glfwGetTime() - gs.dt);
      printf("%.1lf fps\n", gs.fps);
      gs.dt = glfwGetTime();
      gs.frames = 0;
    }
    gs.frames++;

    // Clear screen
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glfwSwapBuffers();
  }
  
  boot_shutdown();

  return 0;
}