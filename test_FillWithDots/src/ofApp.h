#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

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

		bool isInsideOnlyFirstLine(vector<ofPolyline>* lines, ofPoint p);
		bool isInsideAnyLines(vector<ofPolyline>* lines, ofPoint p);

		void saveCharacter(char c);

		string characters;

		void savePoints();

		ofTrueTypeFont* font;

		string fontName;

		string text;

		ofxXmlSettings positions;

		ofMesh mesh;
		ofMesh controlMesh;
		
};