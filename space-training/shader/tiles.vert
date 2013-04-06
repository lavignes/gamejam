#version 130

attribute float id;
attribute vec2 coord;
attribute vec2 st;

uniform vec2 camera;
uniform vec2 pos;
uniform vec2 tile;

varying vec2 frag_st;

void main() {

  gl_Position = gl_ModelViewProjectionMatrix * vec4(coord + pos, 1.0, 1.0);
  
  frag_st = tile * .0625 + st;
}