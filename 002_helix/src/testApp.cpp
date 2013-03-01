#include "testApp.h"

testApp::testApp()
  :helix_drawer(helix, ofGetWidth(), ofGetHeight())
  ,gui("settings", 400, 300)
  ,use_cam(false)
  ,draw_gui(true)
{
}

//--------------------------------------------------------------
void testApp::setup(){
  ofSetFrameRate(60);
  ofSetVerticalSync(true);
  ofBackground(1);
  cam.setup(ofGetWidth(), ofGetHeight());
  cam.setPosition(-5.0f, 10.0f, 20.0f);
  helix_drawer.setup(ofGetWidth(), ofGetHeight());

  gui.addFloat("helix.radius", helix.radius).setMin(1.0f).setMax(8.0f).setStep(0.001);
  gui.addFloat("helix.ribbon_thickness", helix.ribbon_thickness).setMin(0.001).setMax(1.0f).setStep(0.001);
  gui.addFloat("helix.ribbon_height", helix.ribbon_height).setMin(0.001).setMax(5.0f).setStep(0.001);
  gui.addFloat("helix.angle_increment", helix.angle_increment).setMin(0.001).setMax(0.2f).setStep(0.001);
  gui.addFloat("helix.grow_force", helix.grow_force).setMin(0.001).setMax(10.0f).setStep(0.001);
  gui.addFloat("helix.tube_radius", helix.tube_radius).setMin(0.001).setMax(5.0f).setStep(0.001);
  gui.addInt("helix.tube_resolution", helix.tube_resolution).setMin(3).setMax(50);
  gui.addInt("helix.tube_interval", helix.tube_interval).setMin(3).setMax(50);
  gui.addInt("helix.num_iterations", helix.num_iterations).setMin(3).setMax(500);
  gui.addBool("draw lines", helix_drawer.draw_lines);
  gui.setColor(0.3);
  gui.load();
}

//--------------------------------------------------------------
void testApp::update(){
  gui.update();
  helix_drawer.update();

}

//--------------------------------------------------------------
void testApp::draw(){
  const float* pm = cam.pm().getPtr();
  const float* vm = cam.vm().getPtr();
  const float* nm = cam.nm().getPtr();

  helix_drawer.draw(pm, vm, nm);

  if(draw_gui) {
    fps.draw();
    gui.draw();
  }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
  if(key == 'r') {
    helix_drawer.reset();
    Vec3 p = cam.getPosition();
    cam.setPosition(p.x, 0.0f, p.z);
  }
  else if(key == 'l') {
    gui.load();
  }
  else if(key == 's') {
    gui.save();
  }
  else if(key == 'g') {
    draw_gui = !draw_gui;
  }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
  gui.onMouseMoved(x,y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
  if(use_cam) {
    cam.onMouseDragged(x,y);
  }
  gui.onMouseMoved(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
  if(use_cam) {
    cam.onMouseDown(x,y);
  }
  gui.onMouseDown(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
  gui.onMouseUp(x,y);
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
