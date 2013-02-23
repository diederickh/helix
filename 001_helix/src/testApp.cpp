#include "testApp.h"

testApp::testApp()
  :particles_drawer(particles)
  ,helix_drawer(helix, particles_drawer)
  ,follow(false)
  ,gui("settings", 300, 300)
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
  cam.setPosition(0.0f, 10.0f, 20.0f);
  helix_drawer.setup(ofGetWidth(), ofGetHeight());

  gui.addFloat("rays.exposure", helix_drawer.shader_rays.exposure).setMin(0.0001).setMax(0.01).setStep(0.0001);
  gui.addFloat("rays.decay", helix_drawer.shader_rays.decay).setMin(0.9).setMax(1.2).setStep(0.0001);
  gui.addFloat("rays.density", helix_drawer.shader_rays.density).setMin(0.25).setMax(0.75).setStep(0.0001);
  gui.addFloat("rays.weight", helix_drawer.shader_rays.weight).setMin(1.0f).setMax(10.0f).setStep(0.1);
  gui.addColor("helix.color", helix_drawer.shader_cubemap.color);
  gui.load();
  gui.setColor(0.3);

  float s = 10.5;
  for(int i = 0; i < 1000; ++i) {
    Particle p(rx_random(0.1, 4.0f), rx_random(300,1500));
    p.position.set(rx_random(-s,s),rx_random(-s,s), rx_random(-s,s));
    particles.addParticle(p);
  }
  particles_drawer.setup();
}

//--------------------------------------------------------------
void testApp::update(){
  gui.update();
  helix_drawer.update();

  if(follow) {
    Vec3 p = cam.getPosition();
    cam.setPosition(p.x, helix.a.position.y - 4, p.z);
  }
  
  particles.addForce(Vec3(0.0f, 2.2f, 0.0f));
  particles.update(0.016f);
  particles_drawer.update();
}

//--------------------------------------------------------------
void testApp::draw(){
  const float* pm = cam.pm().getPtr();
  const float* vm = cam.vm().getPtr();
  const float* nm = cam.nm().getPtr();

  particles_drawer.draw(pm, vm, nm);  
  helix_drawer.draw(pm, vm, nm);

  if(draw_gui) {
    fps.draw();
    gui.draw();
  }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
  if(key == 'f') {
    follow = !follow;
  }
  else if(key == 'r') {
    helix_drawer.reset();
    Vec3 p = cam.getPosition();
    cam.setPosition(p.x, 0.0f, p.z);
  }
  else if(key == 'd') {
    helix_drawer.toggleDebug();
  }
  else if(key == 'c') {
    use_cam = !use_cam;
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
