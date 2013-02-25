#version 120

uniform float u_weight[10];
uniform float u_pix_offset[10];
uniform float u_width;
uniform sampler2D u_tex;
varying vec2 v_tex;

void main() {
  float dx = 1.0 / u_width;
  vec4 sum = texture2D(u_tex, v_tex) * u_weight[0];
  for(int i = 1; i < 10; ++i) {
    sum += texture2D(u_tex, v_tex + vec2(u_pix_offset[i] * dx, 0.0) ) * u_weight[i];
    sum += texture2D(u_tex, v_tex - vec2(u_pix_offset[i] * dx, 0.0) ) * u_weight[i] ;
  }
  gl_FragColor = sum;
}
