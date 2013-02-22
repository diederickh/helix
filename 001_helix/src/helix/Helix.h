
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

#include <roxlu/Roxlu.h>


struct Particle {
  Particle();
  void addForce(Vec3 f);
  void update(float dt);

  Vec3 position;
  Vec3 forces;
  Vec3 velocity;
};

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
  void generateVerticesForSpine(std::vector<Vec3>& spine);
  void generateVerticesForBasePairs(std::vector<Vec3>& sa, std::vector<Vec3>& sb);
  void generateVerticesForTube(Vec3& ta, Vec3& tbo);
 public:

  int num_elements;                               /* used internally to keep track of the current number of added elements */
  float angle;                                    /* the current angle or the two top points of the helix */
  float radius;                                   /* radius of the helix */
  Particle a;                                     /* particle that is used to fetch the position of the head */
  Particle b;                                     /* "" */
  std::vector<Vec3> spine_a;                      /* we keep track of the particle positions at every update */
  std::vector<Vec3> spine_b;                      /* "" */
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
