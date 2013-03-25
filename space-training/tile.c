#include <stdio.h>
#include <stdlib.h>

#include "tile.h"

// Binary to tile
static const char TileTable[] = {

  [0x8000] = TILE_NONE,    // 0x000000
  [0xFFFF] = TILE_BIGSTAR, // 0xffffff
  [0xD294] = TILE_STAR,    // 0xa1a1a1
  [0xFFE0] = TILE_CAUTION, // 0xfcff00
};

void tileset_load(GameState* gs, const char* path) {

  FILE* file = fopen(path, "rb");

  int offset;
  int width;
  int height;
  int size;
  unsigned short* binary_data;

  fseek(file, 0xa, SEEK_SET);
  fread(&offset, 4, 1, file);

  fseek(file, 0x12, SEEK_SET);
  fread(&width, 4, 1, file);
  fread(&height, 4, 1, file);

  fseek(file, 0x22, SEEK_SET);
  fread(&size, 4, 1, file);

  binary_data = malloc(size);
  fseek(file, offset, SEEK_SET);
  fread(binary_data, 2, size/2, file);

  fclose(file);

  printf("0x%hx\n", binary_data[0]);

  int i, j;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < 256; i++) {
      gs->tilemap[i][j] = TileTable[binary_data[i+(255-j)*width]];
    }
  }

  free(binary_data);
}
