#include "Helix.h"

HelixVertex::HelixVertex(){}
HelixVertex::HelixVertex(Vec3 position):position(position) {}
HelixVertex::HelixVertex(Vec3 position, Vec3 normal):position(position),normal(normal) {}

// +++++++++++++++++++++++++++++++++++++++++++++++++++

Helix::Helix() 
  :radius(4.5f)
  ,angle(0.0f)
  ,num_side_vertices(0)
  ,num_tube_vertices(0)
  ,ribbon_height(0.8f)
  ,ribbon_thickness(0.05f)
{
}

Helix::~Helix() {
  reset();
}

void Helix::setup() {
  a.position = Vec3( - (0.5 * radius), 0.0f, 0.0f);
  b.position = Vec3( + (0.5 * radius), 0.0f, 0.0f);
}

void Helix::update(float dt) {
  angle += 0.07;
  float ca = cos(angle);
  float sa = sin(angle);
  a.position.x = ca * radius;
  a.position.z = sa * radius;

  ca = cos(angle - PI);
  sa = sin(angle - PI);
  b.position.x = ca * radius;
  b.position.z = sa * radius;

  a.update(dt);
  b.update(dt);

  spine_a.push_back(a.position);
  spine_b.push_back(b.position);

  int nmax = 400;
  if(spine_a.size() > nmax) {
    // spine_a.erase(spine_a.begin());
  }
  if(spine_b.size() > nmax) {
    //spine_b.erase(spine_b.begin());
  }
}

void Helix::addForce(Vec3 f) {
  a.addForce(f);
  b.addForce(f);
}

void Helix::generateVerticesForSpine(std::deque<Vec3>& spine) {
  // GENERATE THE "BACKBONES/SIDE"
  //  float height = 0.8f;
  //  float thickness = 0.05f;
  if(spine.size() < 2) {
    return;
  }

  std::vector<Vec3> points;
  for(int i = 1; i < spine.size(); ++i) {
    Vec3& a0 = spine[i-1];
    Vec3& a2 = spine[i];
    Vec3 t = a2 - a0;
    Vec3 c(0.0f, a0.y, 0.0f); // <-- we could use one spine in the center and swirl around that
 
    Vec3 perp = a0 - c;
    perp.normalize();
    perp *= ribbon_thickness;
 
    Vec3 diry = cross(perp, t);
    diry.normalize();
    diry *= ribbon_height;
 
    points.push_back((a0 - perp) - diry);
    points.push_back((a0 + perp) - diry);
    points.push_back((a2 + perp) + diry);
    points.push_back((a2 - perp) + diry);
  }
 
  int num_slices = points.size() / 4;
  num_side_vertices = (num_slices - 1) * 2;
  
  // FRONT
  side_elements.push_back(num_elements);
  for(int i = 0; i < num_slices - 1; ++i) {
    int dx = i * 4;
    Vec3& a0 = points[dx + 0];
    Vec3& a3 = points[dx + 3];
    Vec3& a4 = points[dx + 4];

    Vec3 side_normal = cross(a4 - a0, a4 - a3).normalize();
    vertices.push_back(HelixVertex(a0, side_normal));
    vertices.push_back(HelixVertex(a3, side_normal));
    num_elements += 2;
  }

  // BACK
  side_elements.push_back(num_elements);
  for(int i = 0; i < num_slices - 1; ++i) {
    int dx = i * 4;
    Vec3& a1 = points[dx + 1];
    Vec3& a2 = points[dx + 2];
    Vec3& a5 = points[dx + 5];

    Vec3 side_normal = - cross(a5 - a1, a5 - a2).normalize();
    vertices.push_back(HelixVertex(a1, side_normal));
    vertices.push_back(HelixVertex(a2, side_normal));
    num_elements += 2;
  }

  // BOTTOM
  side_elements.push_back(num_elements);
  for(int i = 0; i < num_slices - 1; ++i) {
    int dx = i * 4;
    Vec3& a2 = points[dx + 2];
    Vec3& a3 = points[dx + 3];
    Vec3& a6 = points[dx + 6];

    Vec3 top_normal = - cross(a3 - a2, a3 - a6).normalize();
    vertices.push_back(HelixVertex(a3, top_normal));
    vertices.push_back(HelixVertex(a2, top_normal));
    num_elements += 2;
  }

  // TOP
  side_elements.push_back(num_elements);
  for(int i = 0; i < num_slices - 1; ++i) {
    int dx = i * 4;
    Vec3& a0 = points[dx + 0];
    Vec3& a1 = points[dx + 1];
    Vec3& a5 = points[dx + 5];

    Vec3 top_normal = - cross(a0 - a1, a0 - a5).normalize();
    vertices.push_back(HelixVertex(a0, top_normal));
    vertices.push_back(HelixVertex(a1, top_normal));
    num_elements += 2;
  }
}


void Helix::generateVerticesForTube(Vec3& ta, Vec3& tb) {
  std::vector<Vec3> points_a;
  std::vector<Vec3> points_b;
  std::vector<Vec3> normals_a;
  std::vector<Vec3> normals_b;

  Vec3 up(0.0f, 1.0f, 0.0f);
  Vec3 dir = (tb - ta).getNormalized();
  Vec3 right = cross(dir, up);
  Vec3 right_start = right;
  float r = 0.2;
  int num_segments = 50;
  float angle_per_segment = TWO_PI / num_segments;

  right.normalize();
  num_tube_vertices = (num_segments * 2) + 2;
  tube_elements.push_back(num_elements);

  for(int i = 0; i < num_segments; ++i) {
    right.rotate(angle_per_segment, dir);
    points_a.push_back(ta + (right * r));
    normals_a.push_back(right);
    points_b.push_back(tb + (right * r));
    normals_b.push_back(right);
  }

  for(int i = 0; i < num_segments; ++i) {
    vertices.push_back(HelixVertex(points_a[i], normals_a[i]));
    vertices.push_back(HelixVertex(points_b[i], normals_b[i]));
    num_elements += 2;
  }

  vertices.push_back(HelixVertex(points_a[0], normals_a[0]));
  vertices.push_back(HelixVertex(points_b[0], normals_b[0]));
  num_elements += 2;
}

void Helix::generateVerticesForBasePairs(std::deque<Vec3>& sa, std::deque<Vec3>& sb) {
  if(sa.size() != sb.size()) {
    return;
  }
  
  for(int i = 0; i < sa.size(); ++i){
    if(i % 7 == 0) {
      generateVerticesForTube(sa[i], sb[i]);
    }
  }
}

void Helix::generateVertices() {
  vertices.clear();
  side_elements.clear();
  tube_elements.clear();
  num_elements = 0;
  
  generateVerticesForSpine(spine_a);
  generateVerticesForSpine(spine_b);
  generateVerticesForBasePairs(spine_a, spine_b);
}

// -----








