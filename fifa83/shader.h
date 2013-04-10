#ifndef SHADER_H

#include <stddef.h>

#include <GL/gl.h>

typedef enum ShaderType {

  SHADER_VERTEX = GL_VERTEX_SHADER,
  SHADER_FRAGMENT = GL_FRAGMENT_SHADER

} ShaderType;

typedef struct Shader {

  unsigned id;
  ShaderType type;

} Shader;

typedef struct Pipeline {

  unsigned id;
  Shader* vert_shader;
  Shader* frag_shader;
  unsigned attribute[8];
  unsigned uniform[8];

} Pipeline;

// Allocates and compiles a shader
Shader* shader_new(ShaderType type, const char* path);

void shader_free(Shader* shader);

// Links shaders into shader program
// Shaders are owned by the pipeline
Pipeline* pipeline_new(Shader* vert_shader, Shader* frag_shader);

// locate an attribute and add it to an attribute table with id
void pipeline_attribute(Pipeline* pipeline, const char* attr, unsigned id);

// locate a uniform and add it to a uniform table with id
void pipeline_uniform(Pipeline* pipeline, const char* unif, unsigned id);

// This frees the shaders used by the pipeline
void pipeline_free(Pipeline* pipeline);

#endif /* GBC_SHADER_H */
