#pragma once

#include "ofMain.h"
#include "helix/Helix.h"
#include "helix/HelixDrawer.h"

#include <roxlu/Roxlu.h>

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
  FPS fps;
  EasyCam cam;
  Helix helix;
  HelixDrawer helix_drawer;
  bool follow;
};
