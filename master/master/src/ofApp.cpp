#include "ofApp.h"

#define NUM_NAMES 10

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowPosition(ofGetScreenWidth() + 10, 30);

	font = new ofTrueTypeFont();

	ofSetLogLevel(OF_LOG_WARNING);

	font->load("fonts/BebasNeue/BebasNeue.otf", 50, true, true, true);

	//font->setLetterSpacing(10.0f);

	light.setPosition(0, 0, 0);

	cam.lookAt(ofVec3f(0, 0, 0));

	//img.load("images/Village split image.png");

	gui.setup("GUI", "settings/settings.xml");
	gui.add(distortFactor.set("Distort", 500.0, 0.0, 1000.0));
	gui.add(transitionDuration.set("Duration", 20.0, 1.0, 30.0));
	gui.add(lineWidth.set("Line Width", 3.0, 1.0, 10.0));
	gui.loadFromFile("settings/settings.xml");

	//ofSetDataPathRoot("../Resources/data/");

	post.init(ofGetWidth(), ofGetHeight());

	post.createPass<HorizontalTiltShifPass>()->setEnabled(true);
	post.createPass<GodRaysPass>()->setEnabled(true);

	//tiltShiftHoriPass->setEnabled(true);
	//tiltShiftHoriPass->setH(0.005);

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

	ofxNestedFileLoader loader;
	vector<string> imageNames = loader.load("images/finalPNGS");

	//vector<string> names;
	//for (int i = 0; i < imageNames.size(; i++) {
	//	names.push_back("A");
	//	//names.push_back("VILLAGE");
	//	//names.push_back("MEDIA");
	//}

	//texts.resize(imageNames.size() * NUM_NAMES);
	images.resize(imageNames.size());

	vector<ofVec2f> shardSizes;
	for (int i = 0; i < images.size(); i++) {
		shardSizes.push_back(ofVec2f(10, 10));
	}

	shardSizes[0] = ofVec2f(10, 10); // Village White
	shardSizes[1] = ofVec2f(1, 200); // Spring Summer 2017
	shardSizes[2] = ofVec2f(200, 1); // Press Open Day Tue 1st & Wed 2nd Niovmeber 9am-6pm 2016
	shardSizes[3] = ofVec2f(1, 1); // 140 Old St London EC1V 9BJ
	shardSizes[4] = ofVec2f(5, 5); // RSVP hello@wearevillage.com
	shardSizes[5] = ofVec2f(10, 10); // Village Gradient

	for (int i = 0; i < imageNames.size(); i++) {
		float theta = ofRandom(0.0, 180.0);
		float phi = ofRandom(0.0, 360.0);
		images[i].load(imageNames[i]);
		Textsplosion tempText;
		tempText.setFont(font);
		tempText.setCam(&cam);
		tempText.setColorGradient(ofRandom(0, 127), ofRandom(127, 255), ofRandom(127, 255), ofRandom(0, 127), ofRandom(127, 255), ofRandom(127, 255));
		tempText.setShaders(&shaders);
		tempText.setImg(&(images[i]));
		tempText.setShardSize(shardSizes[i]);
		tempText.setText("A");
		tempText.setViewPositionSpherical(150.0, theta, phi);
		tempText.setCenter(ofVec3f(0, 0, 0));
		texts.push_back(tempText);
	}

	for (int i = 0; i < NUM_NAMES; i++) {
		float theta = ofRandom(0.0, 180.0);
		float phi = ofRandom(0.0, 360.0);
		Textsplosion tempText;
		tempText.setFont(font);
		tempText.setCam(&cam);
		tempText.setColorGradient(ofRandom(0, 127), ofRandom(127, 255), ofRandom(127, 255), ofRandom(0, 127), ofRandom(127, 255), ofRandom(127, 255));
		tempText.setShaders(&shaders);
		tempText.setImg(&(images[i%imageNames.size()]));
		tempText.setShardSize(ofRandom(1, 10), ofRandom(1, 10));
		tempText.setText("A");
		tempText.setViewPositionSpherical(150.0, theta, phi);
		tempText.setCenter(ofVec3f(0, 0, 0));
		texts.push_back(tempText);
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
	else if(animating) {
		goToNextText();
	}

	for (int i = 0; i < texts.size(); i++) {
		texts[i].setDistortFactor(distortFactor);
		texts[i].setLineWidth(lineWidth);
		texts[i].update();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	//cam.begin();
	post.begin(cam);

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
	//ofEnableDepthTest();
	for (int i = 0; i < texts.size(); i++) {
		texts[i].draw();
	}
	//texts[textIndex].draw();
	//ofDisableDepthTest();
	
	post.end();
	cam.begin();
	for (int i = 0; i < texts.size(); i++) {
		if (texts[i].getBrightnessModifier() > 0.4) {
			texts[i].draw();
		}
	}
	cam.end();
	//cam.end();

	if (showGui) {
		ofPushStyle();
		ofSetColor(255);
		gui.draw();
		ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight() - 10);
		ofPopStyle();
	}

}

void ofApp::goToNextText() {
	cout << "Going!" << endl;
	texts[textIndex].fadeOut(transitionDuration);
	textIndex++;
	textIndex %= images.size();
	float distance = 125;
	cameraPosTarget = texts[textIndex].getViewPosition() * distance;
	camUpVectorTarget = texts[textIndex].getUpVector();
	texts[textIndex].fadeIn(transitionDuration);
	initTime = ofGetElapsedTimef();

	camUpVectorTarget = texts[textIndex].getUpVector();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ' ') {
		goToNextText();
	}
	if (key == 'a') {
		animating = !animating;
	}
	if (key == 'f') {
		ofToggleFullscreen();
	}
	if (key == 'g') {
		if (showGui) {
			ofHideCursor();
		}
		else {
			ofShowCursor();
		}
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
