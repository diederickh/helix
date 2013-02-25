#version 120

uniform float u_weight[10];
uniform float u_pix_offset[10];
uniform float u_height;
uniform sampler2D u_tex;
varying vec2 v_tex;

void main() {
  float dy = 1.0 / u_height;
  vec4 sum = texture2D(u_tex, v_tex) * u_weight[0];
  for(int i = 1; i < 10; ++i) {
    sum += texture2D(u_tex, v_tex + vec2(0.0, u_pix_offset[i] * dy) ) * u_weight[i];
    sum += texture2D(u_tex, v_tex - vec2(0.0, u_pix_offset[i] * dy) ) * u_weight[i] ;
  }
  gl_FragColor = sum;
}
