#version 120

varying vec2 frag_uv;
uniform sampler2D texture;

void main() {

  gl_FragColor = texture2D(texture, vec2(frag_uv.x, 1.0 - frag_uv.y));
  //gl_FragColor = vec4(0,0,0,1);
}