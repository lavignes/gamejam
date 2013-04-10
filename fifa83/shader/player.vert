#version 130

attribute vec2 coord;
attribute vec2 st;

uniform vec2 pos;
uniform int frame;
uniform int dir;

varying vec2 frag_st;

void main() {

  vec2 c = coord;
  if (dir == 1)
    c.x = 16-c.x;

  gl_Position = gl_ModelViewProjectionMatrix * vec4(c + pos, 1.0, 1.0);
  
  frag_st = vec2(frame * 0.25, 0) + st;
}