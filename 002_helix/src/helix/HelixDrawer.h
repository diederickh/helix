#ifndef ROXLU_HELIX_DRAWER_H
#define ROXLU_HELIX_DRAWER_H

#include <vector>
#include <roxlu/Roxlu.h>
#include "Helix.h"
#include "HelixShaders.h"

class HelixDrawer {
 public:
  HelixDrawer(Helix& helix, int w, int h);
  ~HelixDrawer();
  void setup(int w, int h);
  void update();
  void draw(const float* pm, const float* vm, const float* nm);
  void reset();

 public:
  int width;
  int height;
  bool is_debug;
  Helix& helix;

 public: 
  HelixDebugShader shader_debug;
  GLuint vao;
  GLuint vbo;
  int bytes_allocated;
  bool draw_lines;

};
#endif
