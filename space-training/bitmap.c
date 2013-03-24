#include <stdio.h>
#include <stdlib.h>

#include <GL/glfw.h>

#include "bitmap.h"

#ifndef GL_UNSIGNED_SHORT_1_5_5_5_REV
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#endif

unsigned bitmap_load(const char* path) {

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

  unsigned short* pixel;

  // twiddle red and blue channels A1B5G5R5 -> A1R5G5B5
  for (int i = 0; i < size/2; ++i) {
    pixel = binary_data + i;
    *pixel = (((*pixel & 0x7c06) >> 10) | (*pixel & 0x83e0) | ((*pixel & 0x1f) << 10));
  }

  unsigned texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
       GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, binary_data);

  free(binary_data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  return texture;
}
