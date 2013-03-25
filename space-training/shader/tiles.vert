#version 120

attribute float id;
attribute vec2 coord;
attribute vec2 uv;

uniform vec2 camera;
uniform vec2 pos;
uniform vec2 tile;

varying vec2 frag_uv;

void main() {

  gl_Position = gl_ModelViewProjectionMatrix * vec4(coord + pos, 1.0, 1.0);
  
  frag_uv = vec2(tile.x*.0625+uv.x, tile.y*.0625+uv.y);
}