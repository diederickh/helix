#include "helix/ParticlesDrawer.h"

ParticlesDrawer::ParticlesDrawer(Particles& ps)
  :ps(ps)
  ,ps_bytes_allocated(0)
  ,ps_vao(0)
  ,ps_vbo(0)
  ,ps_prog(0)
  ,ps_u_tex(0)
{
}

ParticlesDrawer::~ParticlesDrawer() {
}

void ParticlesDrawer::setup() {

  // point sprite shader.
  ps_prog = rx_create_shader(rx_get_file_contents(ofToDataPath("helix_particles.vert")).c_str(), 
                             rx_get_file_contents(ofToDataPath("helix_particles.frag")).c_str());

  glBindAttribLocation(ps_prog, 0, "a_pos");
  glBindAttribLocation(ps_prog, 1, "a_agep");
  glBindAttribLocation(ps_prog, 2, "a_inv_mass");
  glLinkProgram(ps_prog);

  glGenVertexArrays(1, &ps_vao);
  glBindVertexArray(ps_vao);
   
  glGenBuffers(1, &ps_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, ps_vbo);

  glEnableVertexAttribArray(0); // pos
  glEnableVertexAttribArray(1); // age percentage
  glEnableVertexAttribArray(2); // inv mass
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (GLvoid*)0);
  glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (GLvoid*)12);
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (GLvoid*)16);

  glUseProgram(ps_prog);
  ps_u_tex = glGetUniformLocation(ps_prog, "u_tex");
  ps_u_vm = glGetUniformLocation(ps_prog, "vm");
  ps_u_pm = glGetUniformLocation(ps_prog, "pm");


  glEnable(GL_POINT_SPRITE);
  glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
  glEnable(GL_PROGRAM_POINT_SIZE);

  ///RX_VERBOSE(("%d %d %d", ps_u_tex, ps_u_vm, ps_u_pm));
  Image img;
  img.load(ofToDataPath("images/orb.png"));

  glGenTextures(1, &ps_tex);
  glBindTexture(GL_TEXTURE_2D, ps_tex);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getWidth(), img.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.getPixels());
}

void ParticlesDrawer::update() {
  if(ps.particles.size() == 0) {
    return;
  }

  std::vector<ParticleVertex> points;
  for(std::vector<Particle>::iterator it = ps.particles.begin(); it != ps.particles.end(); ++it) {
    ParticleVertex pv;
    pv.position = (*it).position;
    pv.agep = (*it).age;
    pv.inv_mass = (*it).inv_mass;
    points.push_back(pv);
  }

  glBindBuffer(GL_ARRAY_BUFFER, ps_vbo);

  int bytes_needed = points.size() * sizeof(ParticleVertex);
  if(bytes_needed > ps_bytes_allocated) {
    while(ps_bytes_allocated < bytes_needed) {
      ps_bytes_allocated = std::max<int>(ps_bytes_allocated * 2, 1024);
    }
    glBufferData(GL_ARRAY_BUFFER, ps_bytes_allocated, NULL, GL_DYNAMIC_DRAW);
  }
  glBufferSubData(GL_ARRAY_BUFFER, 0, bytes_needed, points[0].position.getPtr());
}

void ParticlesDrawer::draw(const float* pm, const float* vm, const float* nm) {
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);

  glBindVertexArray(ps_vao);
  glUseProgram(ps_prog);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, ps_tex);
  glUniform1i(ps_u_tex, 0);

  glUniformMatrix4fv(ps_u_vm, 1, GL_FALSE, vm);
  glUniformMatrix4fv(ps_u_pm, 1, GL_FALSE, pm);

  glDepthMask(GL_FALSE);
  glPointSize(10.0f);
  glDrawArrays(GL_POINTS, 0, ps.particles.size());
  glDepthMask(GL_TRUE);

  /*
  Vec4 col(1.0, 1.0f, 1.0f, 1.0f);
  dd.begin(GL_POINTS);
  for(int i = 0; i < ps.particles.size(); ++i) {
    dd.addVertex(ps.particles[i].position, col);
  }
  dd.end();
  dd.draw(pm, vm);
  */
}
