
/*

  The order of vertices for the helix sides


                  a7 +---------------------+ a6
                     |                     |
                     |                     |
                +----|---------------+     |
             a4 |    |            a5 |     |
                |    |               |     |
                |    |               |     |
                |    |               |     |
                |    |               |     |
                |    |               |     |
                |    +---------------------+
                |    a3              |   a2
                |                    |
                |                    |
                +--------------------+
               a0                   a1 

*/

#ifndef ROXLU_HELIX_H
#define ROXLU_HELIX_H

#include <deque>
#include <roxlu/Roxlu.h>
#include "helix/Particles.h"


struct HelixVertex {
  HelixVertex();
  HelixVertex(Vec3 position);
  HelixVertex(Vec3 position, Vec3 normal);

  Vec3 position; // 12 bytes
  Vec3 normal;   // 12 bytes
};

class Helix {
 public:
  Helix();
  ~Helix();
  void addForce(Vec3 f);
  void setup();
  void update(float dt);
  
  void generateVertices();
  void reset();
 private:
  void generateVerticesForSpine(std::deque<Vec3>& spine);
  void generateVerticesForBasePairs(std::deque<Vec3>& sa, std::deque<Vec3>& sb);
  void generateVerticesForTube(Vec3& ta, Vec3& tbo);
 public:

  int num_elements;                               /* used internally to keep track of the current number of added elements */
  float angle;                                    /* the current angle or the two top points of the helix */
  float angle_increment;                          /* when creating the pieces of the helix, increment the angle by this amount for each new ribbon segment */
  float radius;                                   /* radius of the helix */
  float ribbon_height;                            /* the height of the ribbon  */
  float ribbon_thickness;                         /* the thickness of the ribbon... this grows towards the 'center' of the helix */

  float tube_radius;                              /* width of the tube */
  int tube_resolution;                            /* how many rectangles/angles do you want to use for the tube */
  int tube_interval;                              /* create a tube every X-steps */
  float grow_force;                               /* how must force do we add for each iteration */
  int num_iterations;                             /* number of iterations to make; this generates the helix basically */
  
  Particle a;                                     /* particle that is used to fetch the position of the head */
  Particle b;                                     /* "" */
  std::deque<Vec3> spine_a;                       /* we keep track of the particle positions at every update */
  std::deque<Vec3> spine_b;                       /* "" */
  std::vector<HelixVertex> vertices;              /* our vertex buffer we send to the gpu every update */
  std::vector<int> side_elements;                 /* element indices for the sides of the backbone */
  std::vector<int> tube_elements;                 /* the elements which need to be drawn separately with a draw call to glDrawElements */
  int num_tube_vertices;                          /* number of vertices in one tube */
  int num_side_vertices;                          /* total number of vertices for one side of the backbone, so not: vertices.size() */  
};

inline void Helix::reset() {
  a.position = 0;
  b.position = 0;
  num_tube_vertices = 0;
  num_side_vertices = 0;
  num_elements = 0;

  spine_a.clear();
  spine_b.clear();
  vertices.clear();
  tube_elements.clear();
  side_elements.clear();
}
#endif
