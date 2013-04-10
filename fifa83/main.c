#include "init.h"
#include "game.h"

int main(int argc, char** argv) {
  context_init();
  
  game_loop();
  
  context_close();

  return 0;
}