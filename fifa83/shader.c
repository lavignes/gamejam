#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>

#include "shader.h"

Shader* shader_new(ShaderType type, const char* path) {

  Shader* shader = malloc(sizeof(Shader));

  // Open the shader source
  // load into gfx card
  FILE* file = fopen(path, "rb");
  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  rewind(file);
  char* buffer = malloc(size);
  fread(buffer, size, 1, file);

  shader->id = glCreateShader(type);
  shader->type = type;
  glShaderSource(shader->id, 1, (const char**) &buffer, &size);

  free(buffer);
  fclose(file);

  // Compile shader
  printf("Compiling %s...", path);
  int compile_ok;
  glCompileShader(shader->id);
  glGetShaderiv(shader->id, GL_COMPILE_STATUS, &compile_ok);

  if (compile_ok == false) {

    printf("error!\n");
    printf("Error in compilation of %s\n", path);

    int info_size;
    glGetShaderiv(shader->id, GL_INFO_LOG_LENGTH, &info_size);

    char* info = malloc(info_size);
    glGetShaderInfoLog(shader->id, info_size, NULL, info);

    printf("%s\n", info);
    free(info);

    exit(compile_ok);
  }

  printf("ok!\n");

  return shader;
}

void shader_free(Shader* shader) {

  glDeleteShader(shader->id);
  free(shader);
}

Pipeline* pipeline_new(Shader* vert_shader, Shader* frag_shader) {

  Pipeline* pipeline = malloc(sizeof(Pipeline));

  pipeline->id = glCreateProgram();

  pipeline->vert_shader = vert_shader;
  pipeline->frag_shader = frag_shader;

  // Attach the shaders
  glAttachShader(pipeline->id, vert_shader->id);
  glAttachShader(pipeline->id, frag_shader->id);
  
  printf("Linking program...");
  int link_ok;
  glLinkProgram(pipeline->id);

  glGetProgramiv(pipeline->id, GL_LINK_STATUS, &link_ok);

  if (link_ok == false) {
    printf("error!\n");

    printf("Error in linking\n");

    int info_size;
    glGetProgramiv(pipeline->id, GL_INFO_LOG_LENGTH, &info_size);

    char* info = malloc(info_size);
    glGetProgramInfoLog(pipeline->id, info_size, NULL, info);

    printf("%s\n", info);
    free(info);

    exit(link_ok);
  }

  printf("ok!\n");

  return pipeline;
}

void pipeline_attribute(Pipeline* pipeline, const char* attr, unsigned id) {

  pipeline->attribute[id] = glGetAttribLocation(pipeline->id, attr);
}

void pipeline_uniform(Pipeline* pipeline, const char* unif, unsigned id) {

  pipeline->uniform[id] = glGetUniformLocation(pipeline->id, unif);
}

void pipeline_free(Pipeline* pipeline) {

  shader_free(pipeline->vert_shader);
  shader_free(pipeline->frag_shader);

  glDeleteProgram(pipeline->id);
  free(pipeline);
}
