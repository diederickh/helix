#include "HelixShaders.h"

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

HelixDebugShader::HelixDebugShader()
  :HelixShader()
  ,u_mode(0)
  ,mode(0)
{
}

bool HelixDebugShader::setup() {
  prog = createShader("helix_debug.vert", "helix_debug.frag");
  glUseProgram(prog);
  u_mode = glGetUniformLocation(prog, "u_mode");
  return true;
}

void HelixDebugShader::use() {
  glUseProgram(prog);
  glUniform1i(u_mode, mode);
}


