#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	plexusEffect.load("shaders/plexus");

	for (int i = 0; i < 1000; i++) {
		seedLocs.push_back(ofVec2f(ofRandom(ofGetWidth()), ofRandom(ofGetHeight())));
	}

	noise = 0.0;
}

//--------------------------------------------------------------
void ofApp::update(){
	seedLocs[0] = ofVec2f(ofGetMouseX(), ofGetHeight() - ofGetMouseY());
	for (int i = 0; i < seedLocs.size(); i++) {
		float val = 1.0 - ofNoise(noise + i) * 2.0;
		seedLocs[i].x += val * 10.0;//1.0 - ofNoise(noise + i)*2.0;
		val = 1.0 - ofNoise(noise + i + 1000) * 2.0;
		seedLocs[i].y += val * 10.0;//1.0 - ofNoise(noise + i + 1000)*2.0;// *100;
	}
	noise += 0.01;

}

//--------------------------------------------------------------
void ofApp::draw(){
	plexusEffect.begin();
	plexusEffect.setUniform2f("u_Resolution", ofGetWidth(), ofGetHeight());
	plexusEffect.setUniform2fv("u_Points", (float *)&seedLocs[0], seedLocs.size());
	plexusEffect.setUniform2f("u_PointA", ofGetMouseX(), ofGetHeight() - ofGetMouseY());
	//plexusEffect.setUniform2f("u_PointB", 0, 0);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	plexusEffect.end();

	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight() - 10.0);
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
