#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
  helix_radius.setup("helix radius", helix.radius, 0.01f, 30.0f);
  helix_tube_radius.setup("tube radius", helix.tube_radius, 0.01f, 10.0f);
  helix_num_iterations.setup("num iterations", helix.num_iterations, 1,500);
  helix_grow_size.setup("grow size", helix.grow_size, 0.01, 2.0);
  helix_angle_increment.setup("angle increment", helix.angle_increment, 0, PI);
  helix_ribbon_height.setup("ribbon height", helix.ribbon_height, 0.0, 50.0);
  helix_tube_resolution.setup("tube resolution", helix.tube_resolution, 1, 50);
  helix_tube_interval.setup("tube interval", helix.tube_interval, 1, 20);

  gui.setup("settings");
  gui.add(&helix_radius);
  gui.add(&helix_tube_radius); 
  gui.add(&helix_num_iterations);
  gui.add(&helix_grow_size);
  gui.add(&helix_angle_increment);
  gui.add(&helix_ribbon_height);
  gui.add(&helix_tube_resolution);
  gui.add(&helix_tube_interval);
  gui.loadFromFile("settings.xml");

  ofBackground(33);
  cam.setDistance(50.0f);
  cam.boom(25);
}

//--------------------------------------------------------------
void testApp::update() {
  helix.radius = helix_radius;
  helix.tube_radius = helix_tube_radius;
  helix.num_iterations = helix_num_iterations;
  helix.grow_size = helix_grow_size;
  helix.angle_increment = helix_angle_increment;
  helix.ribbon_height = helix_ribbon_height;
  helix.tube_interval = helix_tube_interval;
  helix.tube_resolution = helix_tube_resolution;

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
