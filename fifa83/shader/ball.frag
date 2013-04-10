#version 130

varying vec2 frag_st;
uniform sampler2D tex;

void main() {

  vec4 color = texture(tex, frag_st);
  if (color.a == 0) discard;

  gl_FragColor = color;
}