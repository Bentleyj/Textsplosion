#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	lighting.load("shaders/lighting");

	cam.lookAt(ofVec3f(0, 0, 0));
	cam.setPosition(ofVec3f(0, 0, 300));

	gui.setup("gui", "settings/settings.xml");
	gui.add(lightPos.set("lightPos", ofPoint(0, 0, 0), ofPoint(-200, -200, -200), ofPoint(200, 200, 200)));
	gui.add(diffuse.set("diffuse", ofVec4f(0.5, 0.5, 0.5, 0.5), ofVec4f(0, 0, 0, 0), ofVec4f(1, 1, 1, 1)));

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	glCullFace(GL_BACK);

	ofEnableDepthTest();
	cam.begin();
	lighting.begin();
	lighting.setUniform4f("diffuse", diffuse);
	lighting.setUniform3f("lightSourcePosition", lightPos);
	ofSetColor(255);
	ofDrawSphere(0, 0, 0, 50);
	lighting.end();
	ofDrawAxis(100);
	ofSetColor(255);
	ofDrawLine(lightPos, ofVec3f(0, 0, 0));
	ofDrawSphere(lightPos, 10);
	cam.end();

	ofDisableDepthTest();

	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
