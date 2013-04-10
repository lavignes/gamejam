#version 130

attribute vec2 coord;
attribute vec2 st;

uniform vec2 pos;

varying vec2 frag_st;

void main() {

  gl_Position = gl_ModelViewProjectionMatrix * vec4(pos + coord, 1.0, 1.0);
  frag_st = st;
}