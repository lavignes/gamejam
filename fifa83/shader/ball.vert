#version 130

attribute vec2 coord;
attribute vec2 st;

uniform vec2 pos;
uniform int frame;

varying vec2 frag_st;

void main() {

  gl_Position = gl_ModelViewProjectionMatrix * vec4(coord + pos, pos.y -8, 1.0);
  frag_st = vec2(frame * 0.125, 0) + st;
}