#ifndef ROXLU_CUBEMAP_H
#define ROXLU_CUBEMAP_H

#include <roxlu/Roxlu.h>
#include "ofMain.h"
#include <string>

class CubeMap {
 public:
  CubeMap(std::string basename);
  ~CubeMap();
  bool create();

 private:
  bool createTexture(GLenum target, std::string sub);

 public:
  GLuint tex;
  std::string basename;

};
#endif


