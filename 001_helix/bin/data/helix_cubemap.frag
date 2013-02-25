#version 120

uniform samplerCube u_cube_tex;
uniform mat3 nm;
uniform vec4 u_col;
uniform float u_luma_threshold;

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
  vec3 shading = ndotl * u_col.rgb * u_col.a; // vec3(0.2, 0.4, 0.9);

  vec3 shaded =  shading * 0.8 + col * 0.8;
  vec4 shaded_col = vec4(shaded.r, shaded.g, shaded.b, 1.0); 
  gl_FragData[1] = shaded_col;

  
  float luma = 0.2126 * shaded_col.r + 0.7152 * shaded_col.g + 0.0722 * shaded_col.b;
  luma = clamp(luma - u_luma_threshold, 0.0, 1.0);
  luma = luma * (1.0 / (1.0 - u_luma_threshold));
  luma *= 1.3;
  gl_FragData[0] = vec4(luma, luma, luma, 1.0); // this are the bright spots for the bloom

}
