#ifndef ROXLU_HELIX_DRAWER_H
#define ROXLU_HELIX_DRAWER_H

#include <vector>
#include <roxlu/Roxlu.h>
#include "Helix.h"
#include "HelixShaders.h"

class HelixDrawer {
 public:
  HelixDrawer(Helix& helix);
  ~HelixDrawer();
  void setup(int w, int h);
  void update();
  void draw(const float* pm, const float* vm, const float* nm);
  void reset();
  void toggleDebug();

  void drawFullscreen();
 private:
  void shadedDraw(const float* pm, const float* vm, const float* nm);
  void debugDraw(const float* pm, const float* vm, const float* nm);

 public:
  int width;
  int height;
  bool is_debug;
  Helix& helix;
  DebugDrawer dd;

 private:

  // render passes
  HelixDebugShader shader_debug;
  HelixCubeMapShader shader_cubemap;
  GLuint vao;
  GLuint vbo;
  int bytes_allocated;

  // shader passes
  HelixLightRaysShader shader_rays;
  HelixFullscreenShader shader_fullscreen;
  GLuint fs_vao;                    /* full screen, VAO, for shader passes */
  GLuint fs_vbo;                    /* full screen, VBO, for shaded passes */
  GLuint fbo;                       /* fbo, used to render the scene into */
  GLuint rbo;                       /* depth attachment render buffer */
  GLuint tex_shaded;                /* RTT: shaded scene (with cube map) */
  GLuint tex_mask;                  /* RTT: masked scene (for rays pass) */
};
#endif
