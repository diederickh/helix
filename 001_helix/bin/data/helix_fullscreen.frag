#version 120

uniform sampler2D u_tex;
varying vec2 v_tex;

void main() {
  gl_FragColor.a = 1.0;
  gl_FragColor.rgb = texture2D(u_tex, v_tex).rgb;
}
