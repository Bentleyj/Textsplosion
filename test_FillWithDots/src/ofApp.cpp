#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	font = new ofTrueTypeFont();

	font->load("fonts/AlteHaasGroteskBold.ttf", 200, true, true, true);

	mesh.setMode(OF_PRIMITIVE_POINTS);
	ofBackground(0);

	glPointSize(4);
	cout << ofGetElapsedTimef() << endl;
	positions.loadFile("positions.xml");
	cout << ofGetElapsedTimef() << endl;
	cout << positions.tagExists("Letters")<<endl;
	positions.pushTag("Letters");
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofPushMatrix();
	ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
	mesh.draw();
	ofPopMatrix();
}

// Use this for numbers with holes like a, b, d, e, g, o, p, q, A, B, D, O, P, Q, R
bool ofApp::isInsideOnlyFirstLine(vector<ofPolyline>* lines, ofPoint p) {
	int numLines = lines->size();
	ofPolyline firstLine = (*lines)[0];
	if (firstLine.inside(p)) {
		for (int i = 1; i < numLines; i++) {
			if ((*lines)[i].inside(p)) {
				return false;
			}
		}
		return true;
	}
	return false;
}

// Use this for letters with multiple parts like i and j
bool ofApp::isInsideAnyLines(vector<ofPolyline>* lines, ofPoint p) {
	int numLines = lines->size();
	for (int i = 0; i < numLines; i++) {
		if ((*lines)[i].inside(p)) {
			return true;
		}
	}
	return false;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_RETURN) {
		savePoints();
		cout << "Done Saving :" + text << endl;
	}
	else {

		mesh.clear();

		text = ofToString((char)key);

		// Find the string bounding box for our font about the particular text we want to write.
		ofRectangle boundingBox = font->getStringBoundingBox(text, 0, 0);

		vector<ofTTFCharacter> characters = font->getStringAsPoints(text);

		// Go through all the characters
		for (int j = 0; j < characters.size(); j++) {
			// Get the outline of each character

			vector<ofPolyline> lines = characters[j].getOutline();
			ofRectangle charBoundingBox = boundingBox;
			charBoundingBox.x = charBoundingBox.x - boundingBox.width / 2;
			charBoundingBox.y = -charBoundingBox.y - boundingBox.height / 2;
			charBoundingBox.y -= charBoundingBox.height;

			vector<ofPolyline> newLines;
			newLines.resize(lines.size());

			for (int i = 0; i < lines.size(); i++) {
				vector<ofPoint> points = lines[i].getVertices();
				for (int j = 0; j < points.size(); j++) {
					newLines[i].addVertex(ofVec3f(points[j].x - boundingBox.width / 2, -points[j].y - boundingBox.height / 2, 0));
				}
			}

			int numLines = newLines.size();

			for (int i = 0; i < 5000; i++) {
				ofPoint testPoint = ofPoint(ofRandom(charBoundingBox.x, charBoundingBox.x + charBoundingBox.width), ofRandom(charBoundingBox.y, charBoundingBox.y + charBoundingBox.height));
				while (!isInsideOnlyFirstLine(&newLines, testPoint)) {
					testPoint = ofPoint(ofRandom(charBoundingBox.x, charBoundingBox.x + charBoundingBox.width), ofRandom(charBoundingBox.y, charBoundingBox.y + charBoundingBox.height));
				}
				testPoint.y = charBoundingBox.y - testPoint.y + charBoundingBox.height / 2;
				mesh.addVertex(testPoint);
				mesh.addColor(ofColor(255));
			}
		}
	}
}

void ofApp::savePoints() {
	if (!positions.tagExists(text))
		positions.addTag(text);
	positions.pushTag(text);
	for (int i = 0; i < mesh.getNumVertices(); i++) {
		positions.addTag("point");
		positions.pushTag("point", i);
		positions.addValue("X", mesh.getVertex(i).x);
		positions.addValue("Y", mesh.getVertex(i).y);
		positions.popTag();
	}
	positions.popTag();
	positions.saveFile("positions.xml");
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
