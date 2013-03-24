#version 120

varying vec2 frag_uv;
uniform sampler2D texture;

void main() {

  // Get fragment color
  gl_FragColor = texture2D(texture, vec2(frag_uv.x, 1.0 - frag_uv.y));
}