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

	void goToNextText(float distance);

	vector<Textsplosion> texts;

	ofxPanel gui;

	vector<ofVec2f> positions;

	ofParameter<float> distortFactor;
	ofParameter<float> transitionDuration;
	ofParameter<float> lineWidth;


	ofVec3f cameraPosTarget;
	ofVec3f camUpVectorTarget;

	float noiseOffset1;
	float noiseOffset2;
	float noiseOffset3;
	float noiseOffset4;

	float noise;

	int textIndex;

	bool animating = false;
	bool showGui = true;

	ofTrueTypeFont* font;

	ofShader textNoise;
	ofShader backgroundNoise;

	vector<ofImage> images;

	ofxPostProcessing post;

	float initTime;
	float duration;

	DofAltPass::Ptr tiltShiftHoriPass;

	vector<ofShader> shaders;

	ofLight light;

	ofEasyCam cam;
};
