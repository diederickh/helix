#version 120
/*
uniform sampler2D u_tex;
varying vec2 v_tex;

void main() {
  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  gl_FragColor.rgb = texture2D(u_tex, v_tex).rgb;
}
*/

uniform sampler2D u_tex;
uniform float u_exposure;
uniform float u_decay;
uniform float u_density;
uniform float u_weight;

varying vec2 v_tex;
const int NUM_SAMPLES = 150;
void main() {
  vec2 light_pos = vec2(0.5, 0.0);
  vec2 tex_coord = v_tex;
  vec2 delta_tex = v_tex - light_pos;
  delta_tex = delta_tex * (1.0 / float(NUM_SAMPLES)) * u_density;
  float illum_decay = 1.0;

  gl_FragColor = vec4(0);
  vec4 sample;
  for(int i = 0; i < NUM_SAMPLES; ++i) {
    tex_coord -= delta_tex;
    sample = texture2D(u_tex, tex_coord);
    sample *= illum_decay * u_weight;
    gl_FragColor += sample;
    illum_decay *= u_decay;
  }
  gl_FragColor *= u_exposure;
  gl_FragColor.a = 1.0;
}
