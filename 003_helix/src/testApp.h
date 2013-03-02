#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Helix.h"

class testApp : public ofBaseApp{
 public:
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
  ofxFloatSlider helix_radius;
  ofxFloatSlider helix_tube_radius;
  ofxIntSlider helix_num_iterations;
  ofxFloatSlider helix_grow_size;
  ofxFloatSlider helix_angle_increment;
  ofxFloatSlider helix_ribbon_height;
  ofxFloatSlider helix_ribbon_thickness;
  ofxIntSlider helix_tube_resolution;
  ofxIntSlider helix_tube_interval;
    
  ofxPanel gui;
  
  ofEasyCam cam;
  Helix helix;
};
