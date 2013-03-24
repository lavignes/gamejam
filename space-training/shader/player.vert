#version 120

attribute vec2 coord;
attribute vec2 uv;

uniform vec2 camera;
uniform vec2 pos;
uniform bool dir;

varying vec2 frag_uv;

void main() {

  vec2 c = coord;
  if (dir)
    c.x = 32-c.x;

  // I floor the position to keep the position "pixel" perfect
  gl_Position = gl_ModelViewProjectionMatrix * vec4((pos + c - camera), pos.y/180.0, 1.0);
  
  frag_uv = uv;
}