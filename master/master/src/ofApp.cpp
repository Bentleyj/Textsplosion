#include "ofApp.h"

#define NUM_NAMES 10

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowPosition(ofGetScreenWidth() + 10, 30);

	font = new ofTrueTypeFont();

	font->load("fonts/BebasNeue/BebasNeue.otf", 50, true, true, true);

	//font->setLetterSpacing(10.0f);

	light.setPosition(0, 0, 0);

	cam.lookAt(ofVec3f(0, 0, 0));

	img.load("images/Charlotte+Rampling+style+hippie.jpg");

	gui.setup("GUI", "settings/settings.xml");
	gui.add(distortFactor.set("Distort", 500.0, 0.0, 1000.0));
	gui.add(transitionDuration.set("Duration", 20.0, 1.0, 30.0));
	gui.add(lineWidth.set("Line Width", 3.0, 1.0, 10.0));
	gui.loadFromFile("settings/settings.xml");

	//ofSetDataPathRoot("../Resources/data/");

	post.init(ofGetWidth(), ofGetHeight());

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
	//ofBackgroundGradient(ofColor(10), ofColor(0), OF_GRADIENT_CIRCULAR);

	textNoise.load("shaders/textNoise.vert", "shaders/textNoise.frag");
	backgroundNoise.load("shaders/backgroundNoise.vert", "shaders/backgroundNoise.frag");

	shaders.push_back(textNoise);
	shaders.push_back(backgroundNoise);

	vector<string> names;
	for (int i = 0; i < NUM_NAMES; i++) {
		names.push_back("WELCOME");
		names.push_back("TO");
		names.push_back("VILLAGE");
	    names.push_back("TV");
	}

	texts.resize(names.size());

	for (int i = 0; i < names.size(); i++) {
		texts[i].setFont(font);
		texts[i].setCam(&cam);
		texts[i].setColorGradient(ofRandom(127, 255), ofRandom(127, 255), ofRandom(127, 255), ofRandom(127, 255), ofRandom(127, 255), ofRandom(127, 255));
		texts[i].setShaders(&shaders);
		texts[i].setImg(&img);
		texts[i].setText(names[i]);
		float theta = ofRandom(0.0, 180.0);
		float phi = ofRandom(0.0, 360.0);
		texts[i].setViewPositionSpherical(150.0, theta, phi);
		texts[i].setCenter(ofVec3f(0, 0, 0));
	}

	ofEnableAntiAliasing();

	initTime = 0.0f;
}

//--------------------------------------------------------------
void ofApp::update() {
	float now = ofGetElapsedTimef();
	float endTime = initTime + transitionDuration;

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
		texts[i].setDistortFactor(distortFactor);
		texts[i].setLineWidth(lineWidth);
		texts[i].update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	cam.begin();


	//float highestPercentage = -1.0;
	//float lowestPercentage = 1.0;
	//int highestPercentageIndex;
	//for (int i = 0; i < texts.size(); i++) {
	//	ofVec3f currentViewPos = cam.getPosition().normalize();
	//	ofVec3f textViewPos = texts[i].getViewPosition().normalize();
	//	float diff = (currentViewPos - textViewPos).length();
	//	float percent = ofMap(diff, 0.0, 2.0, 1.0, 0.0, true);
	//	if (percent > highestPercentage) {
	//		highestPercentage = percent;
	//		highestPercentageIndex = i;
	//	}
	//	else if (percent < lowestPercentage) {
	//		lowestPercentage = percent;
	//	}
	//}

	//float distance = (cam.getPosition() - ofVec3f(0, 0, 0)).length();

	for (int i = 0; i < texts.size(); i++) {
		texts[i].draw();
	}
	for (int i = 0; i < texts.size(); i++) {
		if (texts[i].getBrightnessModifier() > 0.01) {
			texts[i].draw();
		}
	}
	
	cam.end();

	if (animating) {
		noise += 0.01;
	}

	if (showGui) {
		ofPushStyle();
		ofSetColor(255);
		gui.draw();
		ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight() - 10);
		ofPopStyle();
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ' ') {
		texts[textIndex].fadeOut(transitionDuration);
		textIndex++;
		textIndex %= texts.size();
		float distance = (cam.getPosition() - ofVec3f(0, 0, 0)).length();
		cameraPosTarget = texts[textIndex].getViewPosition() * distance;
		camUpVectorTarget = texts[textIndex].getUpVector();
		texts[textIndex].fadeIn(transitionDuration);
		initTime = ofGetElapsedTimef();

		camUpVectorTarget = texts[textIndex].getUpVector();
	}
	if (key == 'a') {
		animating = !animating;
	}
	if (key == 'f') {
		ofToggleFullscreen();
	}
	if (key == 'g') {
		showGui = !showGui;
	}
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
