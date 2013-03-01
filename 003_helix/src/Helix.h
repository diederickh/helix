#ifndef ROXLU_HELIX_H
#define ROXLU_HELIX_H

#include <iostream>
#include <vector>
#include <deque>
#include "ofMain.h"


class Helix {
 public:
  Helix();
  ~Helix();
  void create();
  void generateVertices(std::deque<ofVec3f>& spine, ofMesh& vertices);
  void draw();
 private:
  void generateVerticesForTube(ofVec3f& ta, ofVec3f& tb);
  void generateVerticesForBasePairs(std::deque<ofVec3f>& sa, std::deque<ofVec3f>& sb);
 public:
  std::deque<ofVec3f> points_a;
  std::deque<ofVec3f> points_b;
  ofVec3f head_a;
  ofVec3f head_b;
  int num_iterations;
  float radius;
  float grow_size;
  float ribbon_thickness;
  float ribbon_height;
  float tube_radius;
  int tube_resolution;
  float angle_increment;
  ofMesh mesh_ribbon_right;
  ofMesh mesh_ribbon_left;
  std::vector<ofMesh> mesh_tubes;
  unsigned int tube_interval;



};

#endif 
