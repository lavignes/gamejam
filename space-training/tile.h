#ifndef TILE_H
#define TILE_H

#include "bootstrap.h"

typedef enum TileType {

  TILE_NONE,
  TILE_BIGSTAR,
  TILE_STAR,
  TILE_CAUTION

} TileType;

void tileset_load(GameState* gs, const char* path);

#endif