#include "ofApp.h"

#define NUM_NAMES 50

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowPosition(ofGetScreenWidth() + 10, 10);

	font = new ofTrueTypeFont();

	font->load("fonts/maxter_board_st/Maxter Board St.ttf", 50, true, true, true);

	light.setPosition(0, 0, 0);

	cam.lookAt(ofVec3f(0, 0, 0));

	cam.disableMouseMiddleButton();

	gui.setup("settings.xml");

	//ofSetDataPathRoot("../Resources/data/");

	post.init(ofGetWidth(), ofGetHeight());
	//post.createPass

	tiltShiftHoriPass = post.createPass<HorizontalTiltShifPass>();
	tiltShiftHoriPass->setEnabled(true);
	tiltShiftHoriPass->setH(0.005);

	noiseOffset1 = 0;
	noiseOffset2 = 1000;
	noiseOffset3 = 2000;
	noiseOffset4 = 3000;

	noise = 0;

	textIndex = 0;

	ofSetLogLevel(OF_LOG_ERROR);

	cameraPosTarget = ofVec3f(0, 0, 600);
	camUpVectorTarget = ofVec3f(0, 1, 0);

	ofBackground(ofColor(0));


	textNoise.load("shaders/textNoise.vert", "shaders/textNoise.frag");
	backgroundNoise.load("shaders/backgroundNoise.vert", "shaders/backgroundNoise.frag");


	shaders.push_back(textNoise);
	shaders.push_back(backgroundNoise);

	vector<string> names;
	for (int i = 0; i < NUM_NAMES; i++) {
		names.push_back("A QUICK");
		names.push_back("BROWN FOX");
		names.push_back("JUMPED OVER");
		names.push_back("THE LAZY");
	    names.push_back("DOG");
	}

	texts.resize(names.size());

	for (int i = 0; i < names.size(); i++) {
		texts[i].setFont(font);
		texts[i].setCam(&cam);
		texts[i].setColor(ofRandom(127, 255), ofRandom(127, 255), ofRandom(127, 255));
		texts[i].setShaders(&shaders);
		texts[i].setText(names[i]);
		float theta = ofRandom(0.0, 180.0);
		float phi = ofRandom(0.0, 360.0);
		texts[i].setViewPositionSpherical(150.0, theta, phi);
		texts[i].setCenter(ofVec3f(0, 0, 0));
	}

	ofEnableAntiAliasing();

	duration = 20.0f;
	initTime = 0.0f;
}

//--------------------------------------------------------------
void ofApp::update() {
	float now = ofGetElapsedTimef();
	float endTime = initTime + duration;

	if (now < endTime) {
		auto easingMethod = &ofxeasing::quart::easeIn;
		float newX = ofxeasing::map(now, initTime, endTime, cam.getPosition().x, cameraPosTarget.x, easingMethod);
		float newY = ofxeasing::map(now, initTime, endTime, cam.getPosition().y, cameraPosTarget.y, easingMethod);
		float newZ = ofxeasing::map(now, initTime, endTime, cam.getPosition().z, cameraPosTarget.z, easingMethod);

		cam.setPosition(newX, newY, newZ);

		float newUpX = ofxeasing::map(now, initTime, endTime, cam.getUpDir().x, camUpVectorTarget.x, easingMethod);
		float newUpY = ofxeasing::map(now, initTime, endTime, cam.getUpDir().y, camUpVectorTarget.y, easingMethod);
		float newUpZ = ofxeasing::map(now, initTime, endTime, cam.getUpDir().z, camUpVectorTarget.z, easingMethod);

		cam.lookAt(ofVec3f(0, 0, 0), ofVec3f(newUpX, newUpY, newUpZ));
	}

	for (int i = 0; i < texts.size(); i++) {
		texts[i].update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	cam.begin();
	//post.begin(cam);

	float highestPercentage = -1.0;
	float lowestPercentage = 1.0;
	int highestPercentageIndex;
	for (int i = 0; i < texts.size(); i++) {
		ofVec3f currentViewPos = cam.getPosition().normalize();
		ofVec3f textViewPos = texts[i].getViewPosition().normalize();
		float diff = (currentViewPos - textViewPos).length();
		float percent = ofMap(diff, 0.0, 2.0, 1.0, 0.0, true);
		if (percent > highestPercentage) {
			highestPercentage = percent;
			highestPercentageIndex = i;
		}
		else if (percent < lowestPercentage) {
			lowestPercentage = percent;
		}
	}

	//drawSquares();

	//textNoise.begin();

	//we want to pass in some varrying values to animate our type / color 

	float distance = (cam.getPosition() - ofVec3f(0, 0, 0)).length();

	//ofDrawSphere(0, 0, 0, 10);

	//ofDrawAxis(100);

	for (int i = 0; i < texts.size(); i++) {
		texts[i].draw();
	}
	for (int i = 0; i < texts.size(); i++) {
		if(texts[i].getBrightnessModifier() > 0.001)
			texts[i].draw();
	}

	//post.end();

	//textNoise.end();

	//    ofDrawAxis(10);

	cam.end();

	if (animating) {
		noise += 0.01;
	}

	gui.draw();

	ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, 10);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ' ') {
		texts[textIndex].fadeOut(20.0f);
		textIndex++;
		textIndex %= texts.size();
		float distance = (cam.getPosition() - ofVec3f(0, 0, 0)).length();
		cameraPosTarget = texts[textIndex].getViewPosition() * distance;
		camUpVectorTarget = texts[textIndex].getUpVector();
		texts[textIndex].fadeIn(20.0f);
		initTime = ofGetElapsedTimef();

		camUpVectorTarget = texts[textIndex].getUpVector();
	}
	if (key == 'a') {
		animating = !animating;
	}
	if (key == 'f') {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
ofVec3f ofApp::drawText(string text, ofVec3f viewPositionNorm) {
	ofPushMatrix();
	float theta = atan(viewPositionNorm.y / viewPositionNorm.x);
	float phi = atan(sqrt(viewPositionNorm.x * viewPositionNorm.x + viewPositionNorm.y * viewPositionNorm.y) / (viewPositionNorm.z));
	ofRotateX(theta * 180 / PI);
	ofRotateY(phi * 180 / PI);
	ofSetColor(0, 255, 255);
	float dist = (ofVec3f(0, 0, 0) - cam.getPosition()).length();

	ofRectangle rect = font->getStringBoundingBox(text, 0, 0);

	vector<ofTTFCharacter> characters = font->getStringAsPoints(text);

	ofMesh mesh;

	for (int j = 0; j < characters.size(); j++) {
		vector<ofPolyline> lines = characters[j].getOutline();

		ofRectangle lineRect = lines[0].getBoundingBox();

		vector<ofPoint> points = lines[0].getVertices();

		for (int i = 0; i < points.size(); i++) {
			ofColor col = ofColor(0, 255, 255/*ofMap(i, 0, points.size(), 0, 255), 0, 0*/);
			mesh.addVertex(ofVec3f(points[i].x - rect.width / 2, -points[i].y - lineRect.getHeight() + rect.height / 2, 0));
			mesh.addColor(col);
			mesh.addVertex(ofVec3f(points[(i + 1) % points.size()].x - rect.width / 2, -points[(i + 1) % points.size()].y - lineRect.getHeight() + rect.height / 2, 0));
			mesh.addColor(col);
		}
	}

	float zOffset = ofMap(ofNoise(noise), 0, 1, 50, -50);
	for (int i = 0; i < mesh.getNumVertices(); i++) {
		ofVec3f vertex = mesh.getVertex(i);
		if (i % 2 == 0) zOffset = ofMap(ofNoise(noise + i), 0, 1, 500, -500);
		vertex.z += zOffset;
		float distanceToTarget = (cam.getPosition() - ofVec3f(0, 0, 0)).length();
		float distanceToObject = distanceToTarget - zOffset;
		float scale = distanceToObject / distanceToTarget;
		vertex.x *= scale;
		vertex.y *= scale;
		mesh.setVertex(i, vertex);
	}

	ofSetLineWidth(3);

	mesh.setMode(OF_PRIMITIVE_LINES);
	mesh.draw();
	ofPopMatrix();

	return viewPositionNorm;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	//cameraPosTarget = cam.getPosition();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	post.init(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//--------------------------------------------------------------
void ofApp::drawSquares() {
	float distanceToTarget = (cam.getPosition() - ofVec3f(0, 0, 0)).length();

	float minDist = 100;
	float maxDist = -100;

	ofSetColor(0, 0, 255);
	float zOffset = ofMap(ofNoise(noise + noiseOffset1), 0, 1.0, minDist, maxDist);
	float distanceToObject = distanceToTarget - zOffset;
	float scale = distanceToObject / distanceToTarget;
	ofDrawBox(12.5*scale, 12.5*scale, zOffset, 25 * scale, 25 * scale, 0);

	zOffset = ofMap(ofNoise(noise + noiseOffset2), 0, 1.0, minDist, maxDist);
	distanceToObject = distanceToTarget - zOffset;
	scale = distanceToObject / distanceToTarget;
	ofDrawBox(-12.5*scale, 12.5*scale, zOffset, 25 * scale, 25 * scale, 0);

	zOffset = ofMap(ofNoise(noise + noiseOffset3), 0, 1.0, minDist, maxDist);
	distanceToObject = distanceToTarget - zOffset;
	scale = distanceToObject / distanceToTarget;
	ofDrawBox(-12.5*scale, -12.5*scale, zOffset, 25 * scale, 25 * scale, 0);

	zOffset = ofMap(ofNoise(noise + noiseOffset4), 0, 1.0, minDist, maxDist);
	distanceToObject = distanceToTarget - zOffset;
	scale = distanceToObject / distanceToTarget;
	ofDrawBox(12.5*scale, -12.5*scale, zOffset, 25 * scale, 25 * scale, 0);

	ofRotate(90, 1, 0, 0);
	ofSetColor(0, 255, 0);
	zOffset = ofMap(ofNoise(noise + noiseOffset1), 0, 1.0, minDist, maxDist);
	distanceToObject = distanceToTarget - zOffset;
	scale = distanceToObject / distanceToTarget;
	ofDrawBox(12.5*scale, 12.5*scale, zOffset, 25 * scale, 25 * scale, 0);

	zOffset = ofMap(ofNoise(noise + noiseOffset2), 0, 1.0, minDist, maxDist);
	distanceToObject = distanceToTarget - zOffset;
	scale = distanceToObject / distanceToTarget;
	ofDrawBox(-12.5*scale, 12.5*scale, zOffset, 25 * scale, 25 * scale, 0);

	zOffset = ofMap(ofNoise(noise + noiseOffset3), 0, 1.0, minDist, maxDist);
	distanceToObject = distanceToTarget - zOffset;
	scale = distanceToObject / distanceToTarget;
	ofDrawBox(-12.5*scale, -12.5*scale, zOffset, 25 * scale, 25 * scale, 0);

	zOffset = ofMap(ofNoise(noise + noiseOffset4), 0, 1.0, minDist, maxDist);
	distanceToObject = distanceToTarget - zOffset;
	scale = distanceToObject / distanceToTarget;
	ofDrawBox(12.5*scale, -12.5*scale, zOffset, 25 * scale, 25 * scale, 0);

	ofRotate(90, 0, 1, 0);

	ofSetColor(255, 0, 0);
	zOffset = ofMap(ofNoise(noise + noiseOffset1), 0, 1.0, minDist, maxDist);
	distanceToObject = distanceToTarget - zOffset;
	scale = distanceToObject / distanceToTarget;
	ofDrawBox(12.5*scale, 12.5*scale, zOffset, 25 * scale, 25 * scale, 0);

	zOffset = ofMap(ofNoise(noise + noiseOffset2), 0, 1.0, minDist, maxDist);
	distanceToObject = distanceToTarget - zOffset;
	scale = distanceToObject / distanceToTarget;
	ofDrawBox(-12.5*scale, 12.5*scale, zOffset, 25 * scale, 25 * scale, 0);

	zOffset = ofMap(ofNoise(noise + noiseOffset3), 0, 1.0, minDist, maxDist);
	distanceToObject = distanceToTarget - zOffset;
	scale = distanceToObject / distanceToTarget;
	ofDrawBox(-12.5*scale, -12.5*scale, zOffset, 25 * scale, 25 * scale, 0);

	zOffset = ofMap(ofNoise(noise + noiseOffset4), 0, 1.0, minDist, maxDist);
	distanceToObject = distanceToTarget - zOffset;
	scale = distanceToObject / distanceToTarget;
	ofDrawBox(12.5*scale, -12.5*scale, zOffset, 25 * scale, 25 * scale, 0);
}
