#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
  
  tube_radius.setup("tube.radius",helix.tube_radius, 0.01f, 10.0f);
  gui.setup("settings");
  gui.add(&tube_radius); 

  ofBackground(33);
  cam.setDistance(50.0f);

}

//--------------------------------------------------------------
void testApp::update(){
  printf("%f\n", helix.tube_radius);
  helix.create();
  
}

//--------------------------------------------------------------
void testApp::draw(){
  cam.begin();
  helix.draw();
  cam.end();
  gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
