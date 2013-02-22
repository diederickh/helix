#version 120

varying vec3 v_norm;
varying vec3 v_pos;

void main() {
  gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
  gl_FragColor.rgb = 0.5 + 0.5 * v_norm;
}
