#version 120

uniform samplerCube u_cube_tex;
uniform mat3 nm;

varying vec3 v_eye_norm;
varying vec3 v_eye_pos;
varying vec3 v_pos;

void main() {
  vec3 norm = normalize(v_eye_norm);
  vec3 cube_norm = vec3(0.0, 0.0, 0.0);
  vec3 world_dir = normalize(v_eye_pos);
  vec3 reflect_dir = nm * reflect(v_eye_pos, norm);
  vec3 col = textureCube(u_cube_tex, reflect_dir).rgb;

  vec3 light_pos = vec3(0,0.2, 50);
  vec3 light_dir = normalize(light_pos - v_eye_pos);
  float ndotl = max(dot(norm, light_dir), 0.0);
  vec3 shading = ndotl * vec3(0.2, 0.6, 0.9);


  vec3 shaded =  shading * 0.8 + col * 0.9;
  gl_FragData[1] = vec4(shaded.r, shaded.g, shaded.b, 1.0);
  gl_FragData[0] = vec4(1.0, 1.0, 1.0, 1.0);

}
