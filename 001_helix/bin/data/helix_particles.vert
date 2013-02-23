#version 120

uniform mat4 vm;
uniform mat4 pm;
attribute vec4 a_pos;
attribute float a_agep;
attribute float a_inv_mass;
varying float v_agep;

void main() {
  gl_Position = pm * vm * a_pos;
  gl_PointSize = 2.4f * sin(a_agep * 0.028) * a_inv_mass;
  v_agep = a_agep;
}
