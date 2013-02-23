#include "Particles.h"

Particle::Particle(float m, float lifespan)
  :lifespan(lifespan)
{
  if(m <= 0.0001f) {
    inv_mass = 0.0f;
  }
  else {
    inv_mass = 1.0f / m;
  }
}

void Particle::update(float dt) {
  agep = age/lifespan;  
  velocity += ((forces * inv_mass) * dt);
  position += velocity * dt;
  velocity *= 0.99f;
  forces = 0;
  age++;
}

void Particle::addForce(Vec3 f) {
  forces += f;
}


// -----------

Particles::Particles() {
}

Particles::~Particles() {
}


void Particles::addParticle(Particle p) {
  particles.push_back(p);
}

void Particles::addForce(Vec3 f) {
  for(std::vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it) {
    (*it).addForce(f);
  }
}

void Particles::update(float dt) {
  for(std::vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it) {
    (*it).update(dt);
  }
}
