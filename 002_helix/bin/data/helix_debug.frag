#version 120

uniform int u_mode;  /* 0 = draw normals, 1 = draw solid color */
varying vec3 v_norm;
varying vec3 v_pos;

void main() {
  gl_FragColor.a = 1.0;
  if(u_mode == 0) {
    gl_FragColor.rgb = 0.5 + 0.5 * v_norm;
  }
  else if(u_mode == 1) {
    gl_FragColor.rgb = vec3(0.0, 0.0, 0.0);
  }
}
