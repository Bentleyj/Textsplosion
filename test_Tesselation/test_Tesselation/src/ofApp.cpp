#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	font.loadFont("fonts/Plain-Light.ttf", 50, true, false, true);

	cam.lookAt(ofVec3f(0, 0, 0));

	text = "THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG";


}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	vector<ofTTFCharacter> characters = font.getStringAsPoints(text);
	for (int i = 0; i < characters.size(); i++) {
		ofMesh mesh = characters[i].getTessellation();
		cam.begin();
		ofSetColor(255);
		font.drawString(text, 0, 0);
		mesh.setMode(OF_PRIMITIVE_TRIANGLES);
		ofSetColor(255, 0, 0);
		mesh.draw();
		ofSetLineWidth(10);
		mesh.setMode(OF_PRIMITIVE_POINTS);
		ofSetColor(0, 0, 255);
		mesh.draw();
		cam.end();
	}

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
