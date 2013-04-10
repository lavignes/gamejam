#include "init.h"

void context_init(int* argc, char** argv) {

  glfwInit();

  // OpenGL version 3
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);

  glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, true);

  // Create window 
  // 200% NES resolution
  glfwOpenWindow(256 * 2, 240 * 2, 5, 5, 5, 1, 8, 0, GLFW_WINDOW);

  glfwSetWindowTitle("FIFA '83");

  // Automatically update input on vblank
  glfwEnable(GLFW_AUTO_POLL_EVENTS);
  glfwSwapInterval(1); // vsync on

  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  printf("GL Version: %s\n", glGetString(GL_VERSION));
  printf("Using GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glewInit();

  printf("GL Extension Wrangler: %s\n", glewGetString(GLEW_VERSION));

  alutInit(argc, argv);
  glEnable(GL_SCISSOR_TEST); // Enable geometry culling

  // transparency
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // z-buffer
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Set up perspective
  glOrtho(0, 256, 240, 0, -1.0, 1.0);
  glDepthRange(-1.0, 1.0);
  glViewport(0, 0, 256 * 2, 240 * 2);
}

void context_close() {

  // close glfw
  glfwTerminate();
  alutExit();
}