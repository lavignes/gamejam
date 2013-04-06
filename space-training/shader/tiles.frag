#version 130

varying vec2 frag_st;
uniform sampler2D tex;

void main() {

  // Get fragment color
  gl_FragColor = texture(tex, frag_st);
}