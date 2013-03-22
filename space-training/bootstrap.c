#include <stdio.h>

#include "bootstrap.h"

void boot_init() {

  int error;

  fail_if(glfwInit() == false);

  printf("Running Scott's bootstrap...\n");
  // No window resize
  glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, true);
  // 400% gameboy scale, 5 bit rgb with 1 bit alpha, 8 bit z buffer
  fail_if(glfwOpenWindow(160 * 4, 144 * 4, 5, 5, 5, 1, 8, 0, GLFW_WINDOW) == false);

  glfwSetWindowTitle("Space Training");

  glfwEnable(GLFW_AUTO_POLL_EVENTS); // Automatically poll input on swap
  glfwSwapInterval(0); // 0: vsync off, 1: vsync on 

  printf("Renderer: %s\n", glGetString(GL_RENDERER));
  printf("GL Version: %s\n", glGetString(GL_VERSION));
  printf("Using GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  error = glewInit();

  if (error != GLEW_OK) {
    fprintf(stderr, "GLEW error: %s\n", glewGetErrorString(error));
  }

  printf("GL Extension Wrangler: %s\n", glewGetString(GLEW_VERSION));

  glClearColor(0.13725490196f, 0.25882352941f, 0.2f, 1.0f);
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
}

void boot_shutdown() {

  glfwTerminate();
}
