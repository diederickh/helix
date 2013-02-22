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

bool HelixShader::createShader(std::string vert, std::string frag) {
  std::string vs = File::getFileContents(ofToDataPath(vert),false);
  std::string fs = File::getFileContents(ofToDataPath(frag),false);
  const char* vss = vs.c_str();
  const char* fss = fs.c_str();
  
  prog = rx_create_shader(vss, fss);
  glBindAttribLocation(prog, 0, "a_pos");
  glBindAttribLocation(prog, 1, "a_norm");
  glBindAttribLocation(prog, 2, "a_tex");

  glLinkProgram(prog);
  glUseProgram(prog);

  u_pm = glGetUniformLocation(prog, "pm");
  u_vm = glGetUniformLocation(prog, "vm");
  u_nm = glGetUniformLocation(prog, "nm");

  return true;
}

void HelixShader::use() {
  glUseProgram(prog);
}

void HelixShader::setMatrices(const float* pm, const float* vm, const float* nm) {
  glUniformMatrix4fv(u_pm, 1, GL_FALSE, pm);
  glUniformMatrix4fv(u_vm, 1, GL_FALSE, vm);
  glUniformMatrix3fv(u_nm, 1, GL_FALSE, nm);
}

HelixDebugShader::HelixDebugShader()
  :HelixShader()
{
}


// DEBUG SHADER
bool HelixDebugShader::setup() {
  createShader("helix_debug.vert", "helix_debug.frag");
  return true;
}


// CUBEMAP SHADER
HelixCubeMapShader::HelixCubeMapShader()
  :HelixShader()
  ,cubemap("helix_cubemap")
{
}

bool HelixCubeMapShader::setup() {
  if(!cubemap.create()) {
    RX_ERROR(("Cannot create cubemap"));
    return false;
  }

  createShader("helix_cubemap.vert", "helix_cubemap.frag");

  use();

  u_cube_tex = glGetUniformLocation(prog, "u_cube_tex");
  if(u_cube_tex == -1) {
    RX_ERROR(("Cannot find cube texture uniform"));
    return false;
  }

  return true;
}

void HelixCubeMapShader::bind(int unit) {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap.tex);
  glUniform1i(u_cube_tex, unit);
  
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
  createShader("helix_rays.vert", "helix_rays.frag");
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
  createShader("helix_fullscreen.vert", "helix_fullscreen.frag");
  use();
  u_tex = glGetUniformLocation(prog, "u_tex");
  return true;
}
