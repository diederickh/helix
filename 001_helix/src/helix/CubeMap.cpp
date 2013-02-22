#include "CubeMap.h"

CubeMap::CubeMap(std::string basename) 
  :basename(basename)
  ,tex(0)
{
}

CubeMap::~CubeMap() {
}

bool CubeMap::create() {
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

  createTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, "pos_x");
  createTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "neg_x");
  createTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "pos_y");
  createTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "neg_y");
  createTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "pos_z");
  createTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "neg_z");

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  return true;
}

bool CubeMap::createTexture(GLenum target, std::string sub) {
  std::string filepath = ofToDataPath("images/" +basename +"_" +sub +".jpg");

  Image img;
  if(!img.load(filepath)) {
    RX_ERROR(("Cannot load: %s", filepath.c_str()));
    return false;
  }

  glTexImage2D(target, 0, GL_RGBA, img.getWidth(), img.getHeight(), 
               0, GL_RGB, GL_UNSIGNED_BYTE, img.getPixels());

  return true;
}

