#pragma once

#include "ofMain.h"
#include "ofxFontStash.h"
#include "ofxGui.h"
#include "Textsplosion.h"
#include "ofxPostProcessing.h"
#include "ofxEasing.h"
#include "ofxNestedFileLoader.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void goToNextText();

	vector<Textsplosion> texts;

	ofxPanel gui;

	vector<ofVec2f> positions;

	ofParameter<float> distortFactor;
	ofParameter<float> transitionDuration;
	ofParameter<float> lineWidth;
	ofParameter<float> zoomOutDist;
	ofParameter<float> tracker;
	ofParameter<float> fadeAmount;
	ofParameter<bool> animating = false;
	ofParameter<bool> fadingUp = false;
	bool showGui = true;

	ofVec3f cameraPosTarget;
	ofVec3f camUpVectorTarget;
	ofVec3f cameraPosOld;
	ofVec3f camUpVectorOld;

	float noiseOffset1;
	float noiseOffset2;
	float noiseOffset3;
	float noiseOffset4;

	float noise;


	int textIndex;

	ofTrueTypeFont* font;

	ofShader textNoise;
	ofShader backgroundNoise;

	ofFbo buffer;

	vector<ofImage> images;

	ofxPostProcessing post;

	DofAltPass::Ptr tiltShiftHoriPass;

	vector<ofShader> shaders;

	ofShader fadeShader;

	ofEasyCam cam;
};
