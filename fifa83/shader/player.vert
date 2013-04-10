#version 130

attribute vec2 coord;
attribute vec2 st;

uniform vec2 pos;
uniform int frame;
uniform int dir;
uniform vec3 color;

varying vec2 frag_st;
varying vec3 frag_color;

void main() {

  vec2 c = coord;
  if (dir == 1)
    c.x = 16-c.x;

  gl_Position = gl_ModelViewProjectionMatrix * vec4(c + pos, pos.y, 1.0);
  
  frag_st = vec2(frame * 0.25, 0) + st;
  frag_color = color;
}