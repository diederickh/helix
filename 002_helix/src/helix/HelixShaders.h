#ifndef ROXLU_HELIX_SHADERS_H
#define ROXLU_HELIX_SHADERS_H

#include <math.h>
#include <roxlu/Roxlu.h>
#include "ofMain.h"


// BASIC SHADER HELPER
class HelixShader {
 public:
  HelixShader();
  virtual ~HelixShader();
  virtual bool setup() = 0;
  virtual void use();
  virtual GLuint createShader(std::string vertFile, std::string fragFile);
  virtual void setMatrices(const float* pm, const float* vm, const float* nm);
  GLuint createTexture(int w, int h);
 public:
  GLuint prog;
  GLuint u_pm;
  GLuint u_vm;
  GLuint u_nm;

};

// DEBUG SHADER WHICH DRAWS NORMALS
class HelixDebugShader : public HelixShader {
 public:
  HelixDebugShader();
  bool setup();
  void use();
 public:
  GLint u_mode;
  int mode;
};

#endif
