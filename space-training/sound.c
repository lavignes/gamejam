#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <AL/al.h>
#include <AL/alut.h>
#include <vorbis/vorbisfile.h>

#include "sound.h"

Sound* sound_load(const char* path) {

  int endian = 0; // 0 for little

  int stream;
  int format;
  int freq;
  int chunk_size;
  int buffer_size = 32768;
  char buffer[buffer_size];

  Sound* sound = malloc(sizeof(Sound));
  sound->size = 0;
  sound->data = NULL;

  FILE* file = fopen(path, "rb");

  vorbis_info* header;
  OggVorbis_File ogg;

  ov_open(file, &ogg, NULL, 0);
  header = ov_info(&ogg, -1);

  if (header->channels == 1)
    format = AL_FORMAT_MONO16;
  else
    format = AL_FORMAT_STEREO16;

  freq = header->rate;

  do {

    chunk_size = ov_read(&ogg, buffer, buffer_size, endian, 2, 1, &stream);
    sound->data = realloc(sound->data, (sound->size + chunk_size));

    // copy to end of sound buffer
    memcpy(sound->data + sound->size, buffer, chunk_size);
    sound->size += chunk_size;

  } while (chunk_size > 0);

  ov_clear(&ogg);

  alGenBuffers(1, &sound->buffer);
  alBufferData(sound->buffer, format, sound->data, sound->size, freq);

  return sound;
}