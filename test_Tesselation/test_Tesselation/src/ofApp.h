#pragma once

#include "ofMain.h"
#include "ofxFontStash.h"
#include "ofxGui.h"
#include "triangulator.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofTrueTypeFont font;

		ofShader shader;

		ofEasyCam cam;

		string text;

		ofMesh finalMesh;
		ofMesh originalMesh;

		ofParameter<int> meshIndex;

		ofxPanel gui;

};
