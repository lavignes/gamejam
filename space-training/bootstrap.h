#include <assert.h>
#include <stdbool.h>

#include <GL/glew.h>
#include <GL/glfw.h>

#define fail_if(x) do { if (x) assert(false); } while (false);

typedef struct GameState {

  long unsigned frames; // # of frames rendered
  double dt;            // dt of physics
  double fps;

} GameState;

void boot_init();
void boot_shutdown();