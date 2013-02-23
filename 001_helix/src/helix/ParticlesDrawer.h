#ifndef ROXLU_PARTICLES_DRAWER_H
#define ROXLU_PARTICLES_DRAWER_H

#include <roxlu/Roxlu.h>
#include "helix/Particles.h"
#include <stdlib.h>
#include "ofMain.h"

class ParticlesDrawer {
 public:
  ParticlesDrawer(Particles& particles);
  ~ParticlesDrawer();
  void setup();
  void update();
  void draw(const float* pm, const float* vm, const float* nm);
 private:
  Particles& ps;

  // point sprites
  GLuint ps_vao;
  GLuint ps_vbo;
  GLuint ps_prog;
  int ps_bytes_allocated;
  GLint ps_u_tex;
  GLint ps_u_vm;
  GLint ps_u_pm;
  GLuint ps_tex;

  // debug draw
  DebugDrawer dd;
};

#endif
