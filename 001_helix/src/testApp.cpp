#include "testApp.h"

testApp::testApp()
  :helix_drawer(helix)
  ,follow(false)
{
}

//--------------------------------------------------------------
void testApp::setup(){
  ofSetFrameRate(60);
  ofSetVerticalSync(true);
  ofBackground(1);
  cam.setup(ofGetWidth(), ofGetHeight());
  cam.setPosition(0.0f, 10.0f, 20.0f);
  helix_drawer.setup(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void testApp::update(){
  helix_drawer.update();

  Vec3 p = cam.getPosition();
  if(follow) {
    cam.setPosition(p.x, helix.a.position.y, p.z);
  }
  else {
    cam.setPosition(p.x, 0.0f, p.z);
  }
}

//--------------------------------------------------------------
void testApp::draw(){
  const float* pm = cam.pm().getPtr();
  const float* vm = cam.vm().getPtr();
  const float* nm = cam.nm().getPtr();
  
  helix_drawer.draw(pm, vm, nm);
  fps.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
  if(key == 'f') {
    follow = !follow;
  }
  else if(key == 'r') {
    helix_drawer.reset();
  }
  else if(key == 'd') {
    helix_drawer.toggleDebug();
  }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
  cam.onMouseDragged(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
  cam.onMouseDown(x,y);
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
