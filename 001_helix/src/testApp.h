#pragma once

#include "ofMain.h"
#include "helix/Helix.h"
#include "helix/HelixDrawer.h"
#include "helix/Particles.h"
#include "helix/ParticlesDrawer.h"

#include <roxlu/Roxlu.h>
#include <buttons/Panel.h>


class testApp : public ofBaseApp{
 public:
  testApp();
  void setup();
  void update();
  void draw();
		
  void keyPressed(int key);
  void keyReleased(int key);
  void mouseMoved(int x, int y);
  void mouseDragged(int x, int y, int button);
  void mousePressed(int x, int y, int button);
  void mouseReleased(int x, int y, int button);
  void windowResized(int w, int h);
  void dragEvent(ofDragInfo dragInfo);
  void gotMessage(ofMessage msg);
 public:
  buttons::Panel gui;
  FPS fps;
  EasyCam cam;
  Particles particles;
  ParticlesDrawer particles_drawer;
  Helix helix;
  HelixDrawer helix_drawer;
  bool follow;
  bool use_cam;
  bool draw_gui;
};
