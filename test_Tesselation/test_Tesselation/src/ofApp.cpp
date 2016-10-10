#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	font.loadFont("fonts/Plain-Light.ttf", 50, true, false, true);

	cam.lookAt(ofVec3f(0, 0, 0));

	shader.load("shaders/textNoise");

	text = "T";

	gui.setup("settings", "settings/settings.xml");
	gui.add(meshIndex.set("Mesh Index", 0, 0, 20));

	vector<ofTTFCharacter> characters = font.getStringAsPoints(text);
	ofMesh mesh;// = characters[0].getTessellation();

	mesh.addVertex(ofPoint(0, 6));
	mesh.addVertex(ofPoint(0, 0));
	mesh.addVertex(ofPoint(3, 0));
	mesh.addVertex(ofPoint(4, 1));
	mesh.addVertex(ofPoint(6, 1));
	mesh.addVertex(ofPoint(8, 0));
	mesh.addVertex(ofPoint(12, 0));
	mesh.addVertex(ofPoint(13, 2));
	mesh.addVertex(ofPoint(8, 2));
	mesh.addVertex(ofPoint(8, 4));
	mesh.addVertex(ofPoint(11, 4));
	mesh.addVertex(ofPoint(11, 6));
	mesh.addVertex(ofPoint(6, 6));
	mesh.addVertex(ofPoint(4, 3));
	mesh.addVertex(ofPoint(2, 6));

	originalMesh = mesh;

	Vector2fVector points = mesh.getVertices();

	Vector2fVector result;

	Triangulate::Process(points, result);

	for (int i = 0; i < result.size(); i++) {
		finalMesh.addVertex(result[i]);
	}

	finalMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
	originalMesh.setMode(OF_PRIMITIVE_LINE_LOOP);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){


		cam.begin();
		ofSetColor(255);
		font.drawString(text, 0, 0);
		//shader.begin();
		shader.setUniform3f("camPosition", cam.getPosition());
		//mesh.setMode(OF_PRIMITIVE_TRIANGLES);
		ofSetColor(255, 0, 0);
		finalMesh.draw();
		ofSetColor(0, 0, 255);
		originalMesh.draw();
		//shader.end();
		//mesh.setMode(OF_PRIMITIVE_POINTS);
		ofSetColor(0, 0, 255);
		//finalMesh.draw();
		ofSetColor(0, 255, 0);
		//ofDrawSphere(pos, 5);

		cam.end();
	//}

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
