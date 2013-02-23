#include "HelixDrawer.h"

HelixDrawer::HelixDrawer(Helix& helix, ParticlesDrawer& ps)
  :helix(helix) 
  ,ps(ps)
  ,vao(0)
  ,vbo(0)
  ,bytes_allocated(0)
  ,is_debug(false)
  ,width(0)
  ,height(0)
  ,fs_vao(0)
  ,fs_vbo(0)
  ,fbo(0)
  ,rbo(0)
  ,tex_shaded(0)
  ,tex_mask(0)
{
}

HelixDrawer::~HelixDrawer() {
  vao = 0;

  if(vbo) {
    glDeleteBuffers(1,&vbo);
    vbo = 0;
  }

  bytes_allocated = 0;
  fs_vao = 0; // @todo delete
  fs_vbo = 0; // @todo delete
  fbo = 0; // @todo delete
  rbo = 0; // @todo delete
  tex_shaded = 0;
  tex_mask = 0;
}


void HelixDrawer::setup(int w, int h) {
  width = w;
  height = h;

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE);

  helix.setup();

  shader_debug.setup();
  shader_cubemap.setup();
  shader_rays.setup();
  shader_fullscreen.setup();

  // Render pass VBO + VAO
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(HelixVertex), (GLvoid*)0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(HelixVertex), (GLvoid*)12);

  // Shader pass VBO + VAO
  glGenVertexArrays(1, &fs_vao);
  glBindVertexArray(fs_vao);
  
  glGenBuffers(1, &fs_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, fs_vbo);
  
  GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f, 0.0f,  // bottom left
    1.0f, -1.0f, 1.0f, 0.0f, // bottom right
    1.0f, 1.0f, 1.0f, 1.0f, // top right

    -1.0f, -1.0f, 0.0f, 0.0f,  // bottom left
    1.0f, 1.0f, 1.0f, 1.0f, // top right
    -1.0f, 1.0f, 0.0f, 1.0f  // top left
  };
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0); // pos
  glEnableVertexAttribArray(2); // tex
  glVertexAttribPointer(0, 2, GL_FLOAT ,GL_FALSE, sizeof(float) * 4, (GLvoid*)0); // pos
  glVertexAttribPointer(2, 2, GL_FLOAT ,GL_FALSE, sizeof(float) * 4, (GLvoid*)8); // tex

  // FBO
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

  // Depth buffer
  glGenRenderbuffers(1, &rbo);
  glBindRenderbuffer(GL_RENDERBUFFER,rbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
  glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

  // Masked scene (black/white/occluded)
  glGenTextures(1, &tex_mask);
  glBindTexture(GL_TEXTURE_2D, tex_mask);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_mask, 0);

  // Shaded scene
  glGenTextures(1, &tex_shaded);
  glBindTexture(GL_TEXTURE_2D, tex_shaded);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, tex_shaded, 0);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
  if(!is_debug) {
    shadedDraw(pm, vm, nm);
  }
  else {
    debugDraw(pm, vm, nm);
  }
}

void HelixDrawer::shadedDraw(const float* pm, const float* vm, const float* nm) {
  // RTT
  {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glDisable(GL_BLEND);

    GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, buffers);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(vao);

    shader_cubemap.use();
    shader_cubemap.bind(0);
    shader_cubemap.setMatrices(pm, vm, nm);

    for(int i = 0; i < helix.side_elements.size(); ++i) {
      glDrawArrays(GL_TRIANGLE_STRIP, helix.side_elements[i], helix.num_side_vertices);
    }

    for(int i = 0; i < helix.tube_elements.size(); ++i) {
      glDrawArrays(GL_TRIANGLE_STRIP, helix.tube_elements[i], helix.num_tube_vertices);
    }

    ps.draw(pm, vm, nm);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDrawBuffer(GL_BACK_LEFT);

  glDepthMask(GL_FALSE);
  {

    // HELIX
    glDisable(GL_BLEND);
    shader_fullscreen.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_shaded);
    glUniform1i(shader_fullscreen.u_tex, 0);
    drawFullscreen();

    // LIGHT RAYS
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    shader_rays.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_shaded);
    glUniform1i(shader_rays.u_tex, 0);
    drawFullscreen();

    // HELIX
    // glDisable(GL_BLEND);
    //glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
    glBlendFunc(GL_SRC_COLOR, GL_ONE);
    glBlendFunc(GL_DST_COLOR, GL_ONE);
    //glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
    shader_fullscreen.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_shaded);
    glUniform1i(shader_fullscreen.u_tex, 0);
    drawFullscreen();

  }

  glDisable(GL_BLEND);
  glDepthMask(GL_TRUE);

}

void HelixDrawer::drawFullscreen() {
  glBindVertexArray(fs_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}

void HelixDrawer::debugDraw(const float* pm, const float* vm, const float* nm) {
  Vec4 col(1.0, 0.0, 0.0, 1.0);
  glPointSize(5.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBindVertexArray(vao);

  shader_debug.use();
  shader_debug.setMatrices(pm, vm, nm);

  for(int i = 0; i < helix.side_elements.size(); ++i) {
    glDrawArrays(GL_TRIANGLE_STRIP, helix.side_elements[i], helix.num_side_vertices);
  }

  for(int i = 0; i < helix.tube_elements.size(); ++i) {
    glDrawArrays(GL_TRIANGLE_STRIP, helix.tube_elements[i], helix.num_tube_vertices);
  }
  
#if 1
  // THE HEADS
  dd.begin(GL_POINTS);
  {
    dd.addVertex(Vec3(0.0f, 1.0f, 0.0f), col);
    dd.addVertex(helix.a.position, col);
    dd.addVertex(helix.b.position, col);
  }
  dd.end();
#endif

#if 0
  // DRAW SPLINE OF STRING A  
  dd.begin(GL_LINE_STRIP);
  for(int i = 0; i < helix.spine_a.size()-1; ++i) {
    col.y = float(i)/helix.spine_a.size();
    dd.addVertex(helix.spine_a[i], col);
  }
  dd.end();
#endif

#if 0 
  // DRAW SPLINE OF STRING B
  dd.begin(GL_LINE_STRIP);
  for(int i = 0; i < helix.spine_b.size()-1; ++i) {
    col.y = float(i)/helix.spine_b.size();
    dd.addVertex(helix.spine_b[i], col);
  }
  dd.end();
#endif    


#if 0
  // PAIRS
  col.set(1.0, 1.0, 0.0, 1.0);
  dd.begin(GL_LINES);
  for(int i = 0; i < helix.spine_a.size(); ++i) {
    if(i % 5 == 0) {
      dd.addVertex(helix.spine_a[i], col);
      dd.addVertex(helix.spine_b[i], col);
    }
  }
  dd.end();

  col.set(1.0, 0.0, 0.0, 1.0);
  dd.begin(GL_POINTS);
  for(int i = 0; i < helix.pairs.size(); ++i) {
    dd.addVertex(helix.pairs[i], col);
  }
  dd.end();
#endif


#if 1
  // MESH
  for(int i = 0; i < helix.side_elements.size(); ++i) {
    int start = helix.side_elements[i];
    dd.begin(GL_LINES);
    for(int j = 0; j < helix.num_side_vertices; ++j) {
      dd.addVertex(helix.vertices[start + j].position, col);
    }
    dd.end();
  }
#endif


#if 1
  // NORMALS
  col.set(0.0f, 1.0f, 1.0f, 1.0f);
  dd.begin(GL_LINES);
  for(int i = 0; i < helix.vertices.size(); ++i) {
    HelixVertex& hv = helix.vertices[i];
    dd.addVertex(hv.position, col);
    dd.addVertex(hv.position + (hv.normal * 0.3) , col);
  }
  dd.end();
#endif

  dd.draw(pm, vm);
}

void HelixDrawer::reset() {
  helix.reset();
}

void HelixDrawer::toggleDebug() {
  is_debug = !is_debug;
}
