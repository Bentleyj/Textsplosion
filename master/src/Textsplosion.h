//
//  Textsplosion.h
//  test_Text
//
//  This is the class that holds the text and the position that we view it from.
// Effectively all it should have is 6 things:
// 1. A string describing the name, 
// 2. A ofVec3f describing the position from which our object should be viewed
// 3. An Up-Vector describing the orientation of our text.
// 4. The ofMesh that we use to draw.
// To use ofRotate we need to rotate by some angle about an axis so we also need to store that angle and that axis. This is used when drawing the mesh.
// 5. Axis about which to rotate before drawing the mesh
// 6. Angle We should rotate about that axis
// I want to add a 7th thing which is the center of the text so we can move it in space, right now this functionality exists but I've defaulted the center to ofVec3f(0, 0, 0);
// The rest of the stuff is just for looks, we have a color, bool that tells us if our name is selected, a noise variable for movement.
// We need two references to objects in our ofApp for simplicity, we pass a pointer to the ofEasyCam and to the font (the font will eventually not be necessaryw when we use custom meshes).
//  Created by James Bentley on 08/07/2016.
//
//
#ifndef Textsplosion_h
#define Textsplosion_h

#include "ofMain.h"
#include "ofxXmlSettings.h"

class Textsplosion {
public:
    //Initializer
    Textsplosion();
    //Setters
	void setTextXml(string _text);
    void setTextPoints(string _text);
	void setTextLines(string _text);
	void setTextTris(string _text);
	void setTextBackground(string _text);

	void setViewPositionSpherical(float _r, float _theta, float _phi);
    void setUpVector(ofVec3f _upVector ) { upVector = _upVector; };
    void setCam(ofEasyCam* _cam) { cam = _cam; };
    void setFont(ofTrueTypeFont* _font) { font = _font; };
	void setImg(ofImage* _img) { img = _img; };
    void setColor1(int r, int g, int b, int a) { color1 = ofColor(r, g, b, a); };
	void setColor2(int r, int g, int b, int a) { color2 = ofColor(r, g, b, a); };
    void setColor(int r, int g, int b) { 
		setColor1(r, g, b, 255);
		setColor2(r, g, b, 255);
	};
	void setBackgroundColor1(int r, int g, int b, int a) { backgroundColor1 = ofColor(r, g, b, a); };
	void setBackgroundColor2(int r, int g, int b, int a) { backgroundColor2 = ofColor(r, g, b, a); };
	void setBackgroundColor(int r, int g, int b) {
		setBackgroundColor1(r, g, b, 255);
		setBackgroundColor2(r, g, b, 255);
	};
	void setBackgroundColor1(ofColor col) { backgroundColor1 = col; };
	void setBackgroundColor2(ofColor col) { backgroundColor2 = col; };
	void setColor1(ofColor col) { color1 = col; };
	void setColor2(ofColor col) { color2 = col; };

	void setColorGradient(int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2) { setColor1(r1, g1, b1, a1); setColor2(r2, g2, b2, a2); };
	void setColorGradient(int r1, int g1, int b1, int r2, int g2, int b2) { setColor1(r1, g1, b1, 255.0); setColor2(r2, g2, b2, 255.0); };

	void setXML(ofxXmlSettings* _xml) { xmlPoints = _xml; };
	void setLetterMeshes(map<char, ofMesh>* _map) { letterMeshes = _map; };
	void setLetterSizes(map<char, ofVec2f>* _map) { letterSizes = _map; };

	void setCenter(ofVec3f _pos) { textCenter = _pos; };
	void setShaders(vector<ofShader>* _shaders) { shaders = _shaders; };
	void setDistortFactor(float _distortFactor) { distortFactor = _distortFactor; };
	void setLineWidth(float _lineWidth) { lineWidth = _lineWidth; };
	void setShardSize(ofVec2f _size) {
		int width = 200;//(int)(img->getWidth());
		int height = 200;//(int)(img->getWidth());

		//if ((int)(_size.x) % width) {
		//	ofLogError("Textsplosion::setShardSize(): Warning, the width of your image: " + ofToString(width) + " does not divide evenly into your shard size x: " + ofToString(_size.x) + ".\n Setting anyway but expect holes.\n Options are that do divide evenly are:");
		//	for (int i = 0; i < width; i++) {
		//		if (i % width == 0) {
		//			ofLogError("Texsplosion::setShardSize(): " + ofToString(i) + "\n");
		//		}
		//	}
		//}
		//if ((int)(_size.y) % (int)(height)) {
		//	ofLogError("Warning, the width of your image: " + ofToString(height) + " does not divide evenly into your shard size x: " + ofToString(_size.y) + ".\n Setting anyway but expect holes\n Options are that do divide evenly are:");
		//	for (int i = 0; i < width; i++) {
		//		if (i%width == 0) {
		//			ofLogError("Texsplosion::setShardSize(): " + ofToString(i) + "\n");
		//		}
		//	}
		//}
		shardSize = _size;
	};
    
	void setShardSize(float _x, float _y) { setShardSize(ofVec2f(_x, _y)); };

    //Getters
    string getText() { return text; };
    ofVec3f getViewPosition() { return viewPosition; };
    ofVec3f getUpVector() { return upVector; };
	ofVec3f getCenter() { return textCenter; };
	float getBrightnessModifier() { return brightnessModifier; };
	float getDistortFactor() { return distortFactor; };
	ofVec2f getShardSize() { return shardSize; };
    
    //Functionality
    void draw();
    void update();
	void fadeIn();
	void fadeOut();
    ofVec4f ColorToUniformRange(ofColor col);
	bool isInsideOnlyFirstLine(vector<ofPolyline>* lines, ofPoint p);

private:
	//Core variables
    string text;
	ofVec3f viewPosition;
	ofVec3f upVector;
	ofVboMesh mesh;
	ofVboMesh backgroundMesh;
	ofQuaternion quat;
	ofVec2f shardSize;

	//uniforms to go to the various shaders
	vector<ofShader>* shaders;
	float distortFactor;

	ofRectangle boundingBox;
	ofRectangle charBoundingBox;
	ofVec3f textCenter;
	float rotationTheta;
	ofVec3f rotationAxis;

	//Looks stuff
    ofColor color1;
	ofColor color2;
	ofColor backgroundColor1;
	ofColor backgroundColor2;
	float brightnessModifier;
	float targetBrightness;
	float lineWidth;

	//References to outside things used for calculations
	ofEasyCam* cam;
	ofTrueTypeFont* font;
	ofImage* img;
	ofxXmlSettings* xmlPoints;
	map<char, ofMesh>* letterMeshes;
	map<char, ofVec2f>* letterSizes;
};

#endif /* textsplosion_hpp */
