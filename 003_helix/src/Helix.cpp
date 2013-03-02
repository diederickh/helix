#include "Helix.h"

Helix::Helix() 
  :num_iterations(50)
  ,head_a(0,0,0)
  ,head_b(0,0,0)
  ,grow_size(1.0f)
  ,angle_increment(20 * DEG_TO_RAD)
  ,ribbon_thickness(0.05f)
  ,ribbon_height(0.8f)
  ,tube_radius(0.2)
  ,tube_resolution(10)
  ,tube_interval(2)
  ,radius(4.5f)
{
  mesh_ribbon_left.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
  mesh_ribbon_right.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
}

Helix::~Helix() {
}

void Helix::create() {
  points_a.clear();
  points_b.clear();

  head_a.set(0,0,0);
  head_b.set(0,0,0);
  float angle = 0.0f;

  for(int i = 0; i < num_iterations; ++i) {
    angle += angle_increment;

    float ca = cos(angle);
    float sa = sin(angle);
    head_a.x = ca * radius;
    head_a.z = sa * radius;

    ca = cos(angle - PI);
    sa = sin(angle - PI);
    head_b.x = ca * radius;
    head_b.z = sa * radius;

    head_a.y += grow_size;
    head_b.y += grow_size;

    points_a.push_back(head_a);
    points_b.push_back(head_b);
  }

  generateVertices(points_a, mesh_ribbon_left);
  generateVertices(points_b, mesh_ribbon_right);
  generateVerticesForBasePairs(points_a, points_b);
}

void Helix::draw() {

  mesh_ribbon_right.drawWireframe();
  mesh_ribbon_left.drawWireframe();

  for(int i = 0; i < mesh_tubes.size(); ++i) {
    mesh_tubes[i].drawWireframe();
  }

}

void Helix::generateVertices(std::deque<ofVec3f>& spine, ofMesh& vertices) { 
  vertices.clear();

  std::vector<ofVec3f> points;
  for(int i = 1; i < spine.size(); ++i) {
    ofVec3f& a0 = spine[i-1];
    ofVec3f& a2 = spine[i];
    ofVec3f t = a2 - a0;
    ofVec3f c(0.0f, a0.y, 0.0f); // <-- we could use one spine in the center and swirl around that
 
    ofVec3f perp = a0 - c;
    perp.normalize();
    perp *= ribbon_thickness;
 
    ofVec3f diry = perp.getCrossed(t); //cross(perp, t);
    diry.normalize();
    diry *= ribbon_height;
 
    points.push_back((a0 - perp) - diry);
    points.push_back((a0 + perp) - diry);
    points.push_back((a2 + perp) + diry);
    points.push_back((a2 - perp) + diry);
  }
 
  int num_slices = points.size() / 4;
  
  for(int i = 0; i < num_slices - 1; ++i) {
    int dx = i * 4;
    ofVec3f& a0 = points[dx + 0];
    ofVec3f& a3 = points[dx + 3];
    ofVec3f& a4 = points[dx + 4];
    vertices.addVertex(a0);
    vertices.addVertex(a3);
  }

}

void Helix::generateVerticesForTube(ofVec3f& ta, ofVec3f& tb) {
  std::vector<ofVec3f> points_a;
  std::vector<ofVec3f> points_b;

  ofMesh vertices;
  ofVec3f up(0.0f, 1.0f, 0.0f);
  ofVec3f dir = (tb - ta).getNormalized();
  ofVec3f right = dir.getCrossed(up); 

  ofVec3f right_start = right;
  float r = tube_radius;
  int num_segments = tube_resolution;
  float angle_per_segment = TWO_PI / num_segments;

  vertices.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

  right.normalize();

  for(int i = 0; i < num_segments; ++i) {
    right.rotate(angle_per_segment * RAD_TO_DEG, dir);
    points_a.push_back(ta + (right * r));
    points_b.push_back(tb + (right * r));
  }

  for(int i = 0; i < num_segments; ++i) {
    vertices.addVertex(points_a[i]);
    vertices.addVertex(points_b[i]);
  }

  vertices.addVertex(points_a[0]);
  vertices.addVertex(points_b[0]);

  mesh_tubes.push_back(vertices);
}

void Helix::generateVerticesForBasePairs(std::deque<ofVec3f>& sa, std::deque<ofVec3f>& sb) {
  if(sa.size() != sb.size()) {
    return;
  }

  mesh_tubes.clear();

  for(int i = 0; i < sa.size(); ++i){
    if(i % tube_interval == 0) {
      generateVerticesForTube(sa[i], sb[i]);
    }
  }
}
