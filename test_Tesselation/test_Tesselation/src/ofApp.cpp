#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	font.loadFont("fonts/Plain-Light.ttf", 50, true, false, true);

	cam.lookAt(ofVec3f(0, 0, 0));

	shader.load("shaders/backgroundNoise");

	text = "THE QUICK BORWN FOX JUMPED OVER THE LAZY DOG";

	gui.setup("settings", "settings/settings.xml");
	gui.add(meshIndex.set("Mesh Index", 0, 0, 20));

	vector<ofTTFCharacter> characters = font.getStringAsPoints(text);

	for (int i = 0; i < characters.size(); i++) {
		ofMesh letterMesh = characters[i].getTessellation();
		Triangulator::generateTriangulation(&letterMesh, &finalMesh);
	}

	for (int i = 0; i < finalMesh.getNumVertices(); i++) {
		ofPoint newVertex;
		newVertex = finalMesh.getVertex(i);
		newVertex.y = newVertex.y * -1;
		finalMesh.setVertex(i, newVertex);
	}

	finalMesh.setMode(OF_PRIMITIVE_TRIANGLES);
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
		shader.begin();
		shader.setUniform3f("camPosition", cam.getPosition());
		shader.setUniform1f("brightnessModifier", 1.0f);
		//mesh.setMode(OF_PRIMITIVE_TRIANGLES);
		ofSetColor(255, 0, 0);
		finalMesh.draw();
		ofSetColor(0, 0, 255);
		originalMesh.draw();
		shader.end();
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
