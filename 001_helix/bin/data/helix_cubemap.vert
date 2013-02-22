#version 120

uniform mat4 pm;
uniform mat4 vm;
uniform mat3 nm;

attribute vec3 a_norm;
attribute vec4 a_pos;

varying vec3 v_eye_norm;
varying vec3 v_eye_pos;
varying vec3 v_pos;

void main() {

  gl_Position = pm * vm * a_pos;

  v_pos = vec3(a_pos);  
  v_eye_norm = nm * a_norm; // eye coord
  v_eye_pos = nm * a_pos.xyz;  // eye coord

}
