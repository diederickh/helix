#include "HelixDrawer.h"

HelixDrawer::HelixDrawer(Helix& helix, int w, int h)
  :helix(helix) 
  ,vao(0)
  ,vbo(0)
  ,bytes_allocated(0)
  ,is_debug(false)
  ,width(0)
  ,height(0)
  ,draw_lines(false)
{
}

HelixDrawer::~HelixDrawer() {
  vao = 0;

  if(vbo) {
    glDeleteBuffers(1,&vbo);
    vbo = 0;
  }
  bytes_allocated = 0;
}


void HelixDrawer::setup(int w, int h) {
  width = w;
  height = h;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);

  helix.setup();

  shader_debug.setup();

  // Render pass VBO + VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HelixVertex), (GLvoid*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(HelixVertex), (GLvoid*)12);
}

void HelixDrawer::update() {
  helix.addForce(Vec3(0.0f, 5.16f, 0.0f));
  helix.update(0.016f);
  
  helix.generateVertices();

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  int needed = helix.vertices.size() * sizeof(HelixVertex);
  if(needed > bytes_allocated) {
    while(bytes_allocated < needed) {
      bytes_allocated = std::max<int>(bytes_allocated * 2, 64 * 1024);
    }
  
    glBufferData(GL_ARRAY_BUFFER, bytes_allocated, NULL, GL_DYNAMIC_DRAW);

  }

  glBufferSubData(GL_ARRAY_BUFFER, 0, needed, helix.vertices[0].position.getPtr());
}

void HelixDrawer::draw(const float* pm, const float* vm, const float* nm) {

  Vec4 col(1.0, 0.0, 0.0, 1.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBindVertexArray(vao);

  shader_debug.mode = 0;
  shader_debug.use();
  shader_debug.setMatrices(pm, vm, nm);
  
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  for(int i = 0; i < helix.side_elements.size(); ++i) {
    glDrawArrays(GL_TRIANGLE_STRIP, helix.side_elements[i], helix.num_side_vertices);
  }

  for(int i = 0; i < helix.tube_elements.size(); ++i) {
    glDrawArrays(GL_TRIANGLE_STRIP, helix.tube_elements[i], helix.num_tube_vertices);
  }

  if(draw_lines) {
    glLineWidth(1.0f);
    shader_debug.mode = 1;
    shader_debug.use();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(int i = 0; i < helix.side_elements.size(); ++i) {
      glDrawArrays(GL_TRIANGLE_STRIP, helix.side_elements[i], helix.num_side_vertices);
    }

    for(int i = 0; i < helix.tube_elements.size(); ++i) {
      glDrawArrays(GL_TRIANGLE_STRIP, helix.tube_elements[i], helix.num_tube_vertices);
    }
  }

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void HelixDrawer::reset() {
  helix.reset();
}

