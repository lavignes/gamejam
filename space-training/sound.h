#ifndef SOUND_H
#define SOUND_H

#include <stddef.h>

typedef struct Sound {

  size_t size;
  char* data;

  unsigned buffer;

} Sound;

Sound* sound_load(const char* path);

#endif