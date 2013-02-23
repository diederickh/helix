#version 120

varying float v_agep;

uniform sampler2D u_tex;
void main() {
  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
  //  gl_FragColor.rgb = texture2D(u_tex, gl_PointCoord).rgb  * v_agep;
  gl_FragColor.rgb = vec3(1.0, 1.0, 1.0) * texture2D(u_tex, gl_PointCoord).rgb;
}
