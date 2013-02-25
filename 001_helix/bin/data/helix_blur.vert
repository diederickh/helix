#version 120

attribute vec4 a_pos;
attribute vec2 a_tex;
varying vec2 v_tex;

void main() {
  gl_Position = a_pos;
  v_tex = a_tex;
}
