#ifndef ROXLU_PARTICLES_H
#define ROXLU_PARTICLES_H

#include <roxlu/Roxlu.h>
#include "ofMain.h"

struct ParticleVertex {
  Vec3 position;
  float agep;
  float inv_mass;
};

struct Particle {
  Particle(float m = 1.0f, float lifespan = 1000.0f);
  void update(float dt);
  void addForce(Vec3 f);

  Vec3 velocity;
  Vec3 forces;
  Vec3 position;
  float inv_mass;
  float age;
  float agep;
  float lifespan;
};

class Particles {
 public:
  Particles();
  ~Particles();
  void addForce(Vec3 f);
  void addParticle(Particle p);
  void update(float dt);
 public:
  std::vector<Particle> particles;
};

#endif
