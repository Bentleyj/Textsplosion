#include "ofApp.h"

#define NUM_NAMES 0

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowPosition(ofGetScreenWidth() + 10, 30);

	font = new ofTrueTypeFont();

	ofSetLogLevel(OF_LOG_WARNING);

	font->load("fonts/BebasNeue/BebasNeue.otf", 50, true, true, true);

	//font->setLetterSpacing(10.0f);

	cam.lookAt(ofVec3f(0, 0, 0));

	//img.load("images/Village split image.png");


	gui.setup("GUI", "settings/settings.xml");
	gui.add(distortFactor.set("Distort", 500.0, 0.0, 1000.0));
	gui.add(transitionDuration.set("Duration", 20.0, 1.0, 30.0));
	gui.add(lineWidth.set("Line Width", 3.0, 1.0, 10.0));
	gui.add(zoomOutDist.set("zoomOutDist", 500.0, 100.0, 1000.0));
	gui.add(tracker.set("Tracker", 0.0, 0.0, 1.01));
	gui.add(animating.set("animating", false));
	gui.add(fadingUp.set("fadingUp", false));
	gui.add(fadeAmount.set("fadeAmount", 1.0, 0.0, 1.0));

	//ofSetDataPathRoot("../Resources/data/");

	post.init(ofGetWidth(), ofGetHeight());

	//post.createPass<HorizontalTiltShifPass>()->setEnabled(true);
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

	textNoise.load("shaders/textNoise");
	backgroundNoise.load("shaders/backgroundNoise");
	fadeShader.load("shaders/fade");

	shaders.push_back(textNoise);
	shaders.push_back(backgroundNoise);

	ofxNestedFileLoader loader;
	vector<string> imageNames = loader.load("images/SquarePNGs");

	//vector<string> names;
	//for (int i = 0; i < imageNames.size(; i++) {
	//	names.push_back("A");
	//	//names.push_back("VILLAGE");
	//	//names.push_back("MEDIA");
	//}

	//texts.resize(imageNames.size() * NUM_NAMES);
	images.resize(imageNames.size());
	viewDistances.resize(imageNames.size());

	viewDistancesGroup.setName("View Distances");

	for (int i = 0; i < viewDistances.size(); i++) {
		viewDistancesGroup.add(viewDistances[i].set("View Distance " + ofToString(i), 125.0, 10.0, 200.0));
	}

	gui.add(viewDistancesGroup);
	gui.loadFromFile("settings/settings.xml");


	vector<ofVec2f> shardSizes;

	for (int i = 0; i < images.size(); i++) {
		shardSizes.push_back(ofVec2f(10, 10));
	}

	shardSizes[0] = ofVec2f(10, 10); // Village White
	shardSizes[1] = ofVec2f(1, 200); // Spring Summer 2017
	shardSizes[2] = ofVec2f(200, 1); // Press Open Day Tue 1st & Wed 2nd Niovmeber 9am-6pm 2016
	shardSizes[3] = ofVec2f(1, 1); // 140 Old St London EC1V 9BJ
	shardSizes[4] = ofVec2f(5, 5); // RSVP hello@wearevillage.com
	//shardSizes[5] = ofVec2f(10, 10); // Village Gradient

	for (int i = 0; i < imageNames.size()-1; i++) {
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
		tempText.setText("TESTING");
		tempText.setViewPositionSpherical(150.0, theta, phi);
		tempText.setCenter(ofVec3f(0, 0, 0));
		texts.push_back(tempText);
	}

	//for (int i = 0; i < NUM_NAMES; i++) {
	//	float theta = ofRandom(0.0, 180.0);
	//	float phi = ofRandom(0.0, 360.0);
	//	Textsplosion tempText;
	//	tempText.setFont(font);
	//	tempText.setCam(&cam);
	//	tempText.setColorGradient(ofRandom(0, 127), ofRandom(127, 255), ofRandom(127, 255), ofRandom(0, 127), ofRandom(127, 255), ofRandom(127, 255));
	//	tempText.setShaders(&shaders);
	//	tempText.setImg(&(images[i%(imageNames.size()-1)]));
	//	tempText.setShardSize(ofRandom(1, 10), ofRandom(1, 10));
	//	tempText.setText("A");
	//	tempText.setViewPositionSpherical(150.0, theta, phi);
	//	tempText.setCenter(ofVec3f(0, 0, 0));
	//	texts.push_back(tempText);
	//}

	ofEnableAntiAliasing();

}

//--------------------------------------------------------------
void ofApp::update() {
	//float now = ofGetElapsedTimef();
	//float endTime = initTime + transitionDuration;
	if (animating) {
		tracker = ofLerp(tracker, 1.0, 0.05);
		//auto easingMethod = &ofxeasing::linear::easeIn;
		float rate = tracker;
		//tracker = ofLerp(tracker, 1.2, rate);
		float newX = ofLerp(cameraPosOld.x, cameraPosTarget.x, rate);//ofxeasing::map(now, initTime, endTime, cam.getPosition().x, cameraPosTarget.x, easingMethod);
		float newY = ofLerp(cameraPosOld.y, cameraPosTarget.y, rate); //ofxeasing::map(now, initTime, endTime, cam.getPosition().y, cameraPosTarget.y, easingMethod);
		float newZ = ofLerp(cameraPosOld.z, cameraPosTarget.z, rate); //ofxeasing::map(now, initTime, endTime, cam.getPosition().z, cameraPosTarget.z, easingMethod);

		cam.setPosition(newX, newY, newZ);

		float newUpX = ofLerp(camUpVectorOld.x, camUpVectorTarget.x, rate);//ofxeasing::map(now, initTime, endTime, cam.getUpDir().x, camUpVectorTarget.x, easingMethod);
		float newUpY = ofLerp(camUpVectorOld.y, camUpVectorTarget.y, rate);  //ofxeasing::map(now, initTime, endTime, cam.getUpDir().y, camUpVectorTarget.y, easingMethod);
		float newUpZ = ofLerp(camUpVectorOld.z, camUpVectorTarget.z, rate);  //ofxeasing::map(now, initTime, endTime, cam.getUpDir().z, camUpVectorTarget.z, easingMethod);

		cam.lookAt(ofVec3f(0, 0, 0), ofVec3f(newUpX, newUpY, newUpZ));
	}
	else {
		float rate = 0.01;
		tracker = ofLerp(tracker, 1.0, rate);
		cameraPosOld = cam.getPosition();
		camUpVectorOld = cam.getUpDir();

		cameraPosTarget.rotate(1.0, ofVec3f(1, 1, 1));
		camUpVectorTarget.rotate(1.0, ofVec3f(1, 1, 1));

		float newX = ofLerp(cameraPosOld.x, cameraPosTarget.x, rate);//ofxeasing::map(now, initTime, endTime, cam.getPosition().x, cameraPosTarget.x, easingMethod);
		float newY = ofLerp(cameraPosOld.y, cameraPosTarget.y, rate); //ofxeasing::map(now, initTime, endTime, cam.getPosition().y, cameraPosTarget.y, easingMethod);
		float newZ = ofLerp(cameraPosOld.z, cameraPosTarget.z, rate); //ofxeasing::map(now, initTime, endTime, cam.getPosition().z, cameraPosTarget.z, easingMethod);

		float newUpX = ofLerp(camUpVectorOld.x, camUpVectorTarget.x, rate);//ofxeasing::map(now, initTime, endTime, cam.getUpDir().x, camUpVectorTarget.x, easingMethod);
		float newUpY = ofLerp(camUpVectorOld.y, camUpVectorTarget.y, rate);  //ofxeasing::map(now, initTime, endTime, cam.getUpDir().y, camUpVectorTarget.y, easingMethod);
		float newUpZ = ofLerp(camUpVectorOld.z, camUpVectorTarget.z, rate);

		cam.setPosition(newX, newY, newZ);

		cam.lookAt(ofVec3f(0, 0, 0), ofVec3f(newUpX, newUpY, newUpZ));
	}
	//}
	//else {
	//	goToNextText();
	//}
	/*else if (drifting) {
		auto easingMethod = &ofxeasing::linear::easeOut;
		float slowFactor = 10.0f;
		float newX = ofxeasing::map(now, initTime, endTime*slowFactor, cam.getPosition().x, cameraPosTarget.x, easingMethod);
		float newY = ofxeasing::map(now, initTime, endTime*slowFactor, cam.getPosition().y, cameraPosTarget.y, easingMethod);
		float newZ = ofxeasing::map(now, initTime, endTime*slowFactor, cam.getPosition().z, cameraPosTarget.z, easingMethod);

		cam.setPosition(newX, newY, newZ);

		float newUpX = ofxeasing::map(now, initTime, endTime*slowFactor, cam.getUpDir().x, camUpVectorTarget.x, easingMethod);
		float newUpY = ofxeasing::map(now, initTime, endTime*slowFactor, cam.getUpDir().y, camUpVectorTarget.y, easingMethod);
		float newUpZ = ofxeasing::map(now, initTime, endTime*slowFactor, cam.getUpDir().z, camUpVectorTarget.z, easingMethod);

		cam.lookAt(ofVec3f(0, 0, 0), ofVec3f(newUpX, newUpY, newUpZ));
	}*/

	//if (!(now < endTime)) {
	////	if (going && now < endTime + 0.1) {
	////		drift();
	////	}
	////	else {
	//		goToNextText();
	//	//}
	//}

	for (int i = 0; i < texts.size(); i++) {
		texts[i].setDistortFactor(distortFactor);
		texts[i].setLineWidth(lineWidth);
		texts[i].update();
		texts[i].setViewDistance(viewDistances[i]);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	//cam.begin();
	//buffer.begin();
	post.begin(cam);

	for (int i = 0; i < texts.size(); i++) {
		texts[i].draw();
	}
	//
	post.end();

	cam.begin();
	for (int i = 0; i < texts.size(); i++) {
		if (texts[i].getBrightnessModifier() > 0.4) {
			texts[i].draw();
		}
	}
	cam.end();

	//fadeShader.begin();
	//ofSetColor(255, 0, 0);
	//fadeShader.setUniform1f("u_Amount", fadeAmount);
	//fadeShader.setUniformTexture("u_Tex", buffer.getTexture(), 0);
	//buffer.draw(0, 0);
	//ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	//fadeShader.end();

	//buffer.end();

	//buffer.draw(0, 0);

	//fadeShader.begin();
	//fadeShader.setUniformTexture("texOut", images[1].getTextureReference(), 0);
	//fadeShader.setUniformTexture("texIn", images[0].getTextureReference(), 1);
	//fadeShader.setUniform1f("u_Amount", fadeAmount);
	//cout << fadeAmount << endl;
	//ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	//buffer.draw(0, 0);
	//images[0].draw(0, 0, ofGetWidth(), ofGetHeight());
	//            if( displayImgs[currImg].width < displayImgs[(currImg+1)%2].width)
	//                displayImgs[currImg].draw(0, 0, WIDTH, HEIGHT);
	//            else
	//                displayImgs[(currImg+1)%2].draw(0, 0, WIDTH, HEIGHT);

	//fadeShader.end();

	if (showGui) {
		ofPushStyle();
		ofSetColor(255);
		gui.draw();
		ofDrawBitmapString(ofToString(ofGetFrameRate()), 10, ofGetHeight() - 10);
		ofDrawBitmapString("Width: " + ofToString(ofGetWindowWidth()), 10, ofGetHeight() - 20);
		ofDrawBitmapString("Height: " + ofToString(ofGetWindowHeight()), 10, ofGetHeight() - 30);

		ofPopStyle();
	}

}

void ofApp::goToNextText() {
	//float distance = 125;
	cameraPosOld = cam.getPosition();//texts[textIndex].getViewPosition() * distance;
	camUpVectorOld = cam.getUpDir();//texts[textIndex].getUpVector();
	texts[textIndex].fadeOut(transitionDuration);
	textIndex++;
	textIndex %= texts.size();
	cameraPosTarget = texts[textIndex].getViewPosition() * texts[textIndex].getViewDistance();
	camUpVectorTarget = texts[textIndex].getUpVector();
	texts[textIndex].fadeIn(transitionDuration);

	tracker = 0.0;

	camUpVectorTarget = texts[textIndex].getUpVector();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ' ') {
		goToNextText();
	}
	if (key == 'a') {
		if (animating) {
			cameraPosTarget = cameraPosTarget.getNormalized() * zoomOutDist;
			cameraPosOld = cameraPosOld.getNormalized() * zoomOutDist;
		}
		animating = !animating;
	}
	if (key == 'f') {
		ofToggleFullscreen();
	}
	if (key == 's') {
		fadingUp = !fadingUp;
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
