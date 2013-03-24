#version 120

attribute vec2 coord;
attribute vec2 uv;

uniform vec2 pos;

varying vec2 frag_uv;

void main() {

  vec2 xy = floor(pos + coord);

  gl_Position = gl_ModelViewProjectionMatrix * vec4(xy, 1.0, 1.0);
  
  frag_uv = uv;
}