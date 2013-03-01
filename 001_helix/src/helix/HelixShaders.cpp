#include "HelixShaders.h"

// BASIC SHADER
HelixShader::HelixShader()
  :prog(0)
  ,u_pm(0)
  ,u_vm(0)
{
}

HelixShader::~HelixShader() {
  if(prog) {
    glDeleteShader(prog);
  }
  u_pm = 0;
  u_vm = 0;
}

GLuint HelixShader::createShader(std::string vert, std::string frag) {
  std::string vs = File::getFileContents(ofToDataPath(vert),false);
  std::string fs = File::getFileContents(ofToDataPath(frag),false);
  const char* vss = vs.c_str();
  const char* fss = fs.c_str();
  
  GLuint shader_prog = rx_create_shader(vss, fss);
  glBindAttribLocation(shader_prog, 0, "a_pos");
  glBindAttribLocation(shader_prog, 1, "a_norm");
  glBindAttribLocation(shader_prog, 2, "a_tex");

  glLinkProgram(shader_prog);
  glUseProgram(shader_prog);

  u_pm = glGetUniformLocation(shader_prog, "pm");
  u_vm = glGetUniformLocation(shader_prog, "vm");
  u_nm = glGetUniformLocation(shader_prog, "nm");

  return shader_prog;
}

void HelixShader::use() {
  glUseProgram(prog);
}

void HelixShader::setMatrices(const float* pm, const float* vm, const float* nm) {
  glUniformMatrix4fv(u_pm, 1, GL_FALSE, pm);
  glUniformMatrix4fv(u_vm, 1, GL_FALSE, vm);
  glUniformMatrix3fv(u_nm, 1, GL_FALSE, nm);
}

GLuint HelixShader::createTexture(int w, int h) {
  GLuint tex_id = 0;
  glGenTextures(1, &tex_id);
  glBindTexture(GL_TEXTURE_2D, tex_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  return tex_id;
}

// ---

HelixDebugShader::HelixDebugShader()
  :HelixShader()
{
}


// DEBUG SHADER
bool HelixDebugShader::setup() {
  prog = createShader("helix_debug.vert", "helix_debug.frag");
  return true;
}


// CUBEMAP SHADER
HelixCubeMapShader::HelixCubeMapShader()
  :HelixShader()
  ,cubemap("helix_cubemap")
  ,u_col(0)
  ,u_cube_tex(0)
  ,u_luma_threshold(0)
  ,luma_threshold(0.4f)
{

}

bool HelixCubeMapShader::setup() {
  if(!cubemap.create()) {
    RX_ERROR(("Cannot create cubemap"));
    return false;
  }

  prog = createShader("helix_cubemap.vert", "helix_cubemap.frag");

  use();

  u_cube_tex = glGetUniformLocation(prog, "u_cube_tex");
  if(u_cube_tex == -1) {
    RX_ERROR(("Cannot find cube texture uniform"));
    return false;
  }

  u_col = glGetUniformLocation(prog, "u_col");
  u_luma_threshold = glGetUniformLocation(prog, "u_luma_threshold");


  return true;
}

void HelixCubeMapShader::bind(int unit) {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.tex);
  glUniform1i(u_cube_tex, unit);
  glUniform4fv(u_col, 1, color);
  glUniform1f(u_luma_threshold, luma_threshold);  
}

// LIGHT RAYS
HelixLightRaysShader::HelixLightRaysShader()
  :HelixShader()
  ,u_tex(0)
  ,u_exposure(0)
  ,u_decay(0)
  ,u_weight(0)
  ,u_density(0)
  ,exposure(0.0034f)
  ,decay(1.0f)
  ,density(0.34f)
  ,weight(5.65f) // 5.65
{
}

bool HelixLightRaysShader::setup() {
  prog = createShader("helix_rays.vert", "helix_rays.frag");
  use();
  
  u_tex = glGetUniformLocation(prog, "u_tex");
  u_exposure = glGetUniformLocation(prog, "u_exposure");
  u_decay = glGetUniformLocation(prog, "u_decay");
  u_weight = glGetUniformLocation(prog, "u_weight");
  u_density = glGetUniformLocation(prog, "u_density");

  return true;
}

void HelixLightRaysShader::use() {
  glUseProgram(prog);
  glUniform1f(u_exposure, exposure);
  glUniform1f(u_decay, decay);
  glUniform1f(u_weight, weight);
  glUniform1f(u_density, density);

}

// FULLSCREEN
HelixFullscreenShader::HelixFullscreenShader()
  :HelixShader()
  ,u_tex(0)
{
}

bool HelixFullscreenShader::setup() { 
  prog = createShader("helix_fullscreen.vert", "helix_fullscreen.frag");
  use();
  u_tex = glGetUniformLocation(prog, "u_tex");
  return true;
}

// BLUR SHADER
HelixBlurShader::HelixBlurShader(int w, int h)
  :w(w)
  ,h(h)
  ,prog_blur0(0)
  ,prog_blur1(0)
  ,blur0_tex(0)
  ,blur1_tex(0)
  ,u_blur0_tex(0)
  ,u_blur1_tex(0)
  ,HelixShader()
{
}

float HelixBlurShader::gauss(float x, float sigma2) {
  double coeff = 1.0 / (2.0 * PI * sigma2);
  double expon = -(x * x) / (2.0 * sigma2);
  return (float) (coeff * exp(expon));
}

bool HelixBlurShader::setup() {
  prog_blur0 = createShader("helix_blur.vert", "helix_blur0.frag");
  prog_blur1 = createShader("helix_blur.vert", "helix_blur1.frag");
  blur0_tex = createTexture(w, h);
  blur1_tex = createTexture(w, h);
  
  printf("blur0_tex: %d, w: %d, h: %d\n", blur0_tex, w, h);
  printf("blur1_tex: %d, w: %d, h: %d\n", blur1_tex, w, h);

  // SETUP BLUR PASS 0 (vertical)
  glUseProgram(prog_blur0);
  GLint u_blur0_height = glGetUniformLocation(prog_blur0, "u_height");
  glUniform1f(u_blur0_height, h);
  u_blur0_tex = glGetUniformLocation(prog_blur0, "u_tex");
  
  setupBlur(prog_blur0, 18.0f);
 
  // SETUP BLUR PASS 1
  glUseProgram(prog_blur1);
  GLint u_blur0_width = glGetUniformLocation(prog_blur1, "u_width");
  glUniform1f(u_blur0_width, w);
  u_blur1_tex = glGetUniformLocation(prog_blur1, "u_tex");

  setupBlur(prog_blur1, 18.0f);

  printf("blur0 tex uniform: %d\n", u_blur0_tex);
  printf("blur1 tex uniform: %d\n", u_blur1_tex);
  return true;
}

void HelixBlurShader::setupBlur(GLuint blurprog, float amount) {
  const int num_weights = 10;
  char weight_uniform_name[50];
  char offset_uniform_name[50];
  float weights[num_weights];
  float sum;
  float sigma2 = amount;

  weights[0] = gauss(0.0f, sigma2);
  sum = weights[0];
  for(int i = 1; i < num_weights; ++i) {
    weights[i] = gauss(i, sigma2);
    sum += 2 * weights[i];
  }

  glUseProgram(blurprog);
  for(int i = 0; i < num_weights; ++i) {
    sprintf(weight_uniform_name, "u_weight[%d]", i);
    GLint weight_uniform_loc = glGetUniformLocation(blurprog, weight_uniform_name);
    float val = weights[i] / sum;
    glUniform1f(weight_uniform_loc, val);

    sprintf(offset_uniform_name, "u_pix_offset[%d]", i);
    GLint offset_uniform_loc = glGetUniformLocation(blurprog, offset_uniform_name);
    glUniform1f(offset_uniform_loc, 2.5f * i);

    //printf("> weight: %s = %d, offset: %s, %d\n", weight_uniform_name, weight_uniform_loc, offset_uniform_name, offset_uniform_loc);
  }
}

void HelixBlurShader::usePass0Program() {
  glUseProgram(prog_blur0);
}

void HelixBlurShader::usePass1Program() {
  glUseProgram(prog_blur1);
}

void HelixBlurShader::setPass0TextureUnit(GLuint unit) { 
  usePass0Program(); // just to be sure.
  glUniform1i(u_blur0_tex, unit);
}

void HelixBlurShader::setPass1TextureUnit(GLuint unit) { 
  usePass1Program(); // just to be sure.
  glUniform1i(u_blur1_tex, unit);
}


