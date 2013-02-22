#version 120
uniform mat4 pm;
uniform mat4 vm;
attribute vec3 a_norm;
attribute vec4 a_pos;
varying vec3 v_norm;
varying vec3 v_pos;
void main() {
     gl_Position = pm * vm * a_pos;
     v_norm = a_norm;
     v_pos = vec3(vm * a_pos);
}