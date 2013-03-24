#version 120

attribute vec2 coord;
attribute vec2 uv;

uniform vec2 camera;
uniform vec2 pos;

varying vec2 frag_uv;

void main() {

  vec2 c = coord + pos;

  gl_Position = gl_ModelViewProjectionMatrix * vec4(c, 1.0, 1.0);
  
  frag_uv = uv;
}