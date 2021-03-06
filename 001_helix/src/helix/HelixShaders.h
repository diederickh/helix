#ifndef ROXLU_HELIX_SHADERS_H
#define ROXLU_HELIX_SHADERS_H

#include <math.h>
#include <roxlu/Roxlu.h>
#include "ofMain.h"
#include "CubeMap.h"

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
};

// APPLY CUBEMAP REFLECTION + SOME LIGHT RAY PRE-PROCESSING
class HelixCubeMapShader : public HelixShader {
 public:
  HelixCubeMapShader();
  bool setup();
  void bind(int unit = 0);
 public:
  CubeMap cubemap;
  GLint u_cube_tex;
  GLint u_col;
  GLint u_luma_threshold;
  float color[4];
  float luma_threshold;
};

// APPLY LIGHT RAYS
class HelixLightRaysShader : public HelixShader {
 public:
  HelixLightRaysShader();
  bool setup();
  void use();
 public:
  float exposure;
  float decay;
  float density;
  float weight;

  GLint u_tex; /* the texture from which we sample in the shader, this gets 'smudged' */
  GLint u_exposure;
  GLint u_decay;
  GLint u_density;
  GLint u_weight;
};


// FULL SCREEN TEXTURE
class HelixFullscreenShader : public HelixShader {
 public:
  HelixFullscreenShader();
  bool setup();
 public:
  GLint u_tex;
};

// TWO PASS BLUR SHADER
class HelixBlurShader : public HelixShader {
 public:
  HelixBlurShader(int w, int h);
  bool setup();
  void usePass0Program();
  void usePass1Program();
  void setPass0TextureUnit(GLuint unit);
  void setPass1TextureUnit(GLuint unit);
  float gauss(float x, float sigma2);
  void setupBlur(GLuint prog, float amount);
 public:
  GLuint prog_blur0;
  GLuint prog_blur1;
  GLuint blur0_tex; /* texture for blur 0 */
  GLuint blur1_tex; /* texture for blur 1 */
  GLint u_blur0_tex; /* uniform index for blur 0 texture */
  GLint u_blur1_tex; /* uniform index for blur 1 texture */
    
  int w;
  int h;
};

#endif
