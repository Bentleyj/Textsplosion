//
//  Textsplosion.h
//  test_Text
//
//  This is the class that holds the text and the position that we view it from.
// Effectively all it should have is 7 things:
// 1. A string describing the name, 
// 2. A ofVec3f describing the position from which our object should be viewed
// 3. An Up-Vector describing the orientation of our text.
// 4. The ofMesh object that we use to draw our text.
// 5. The ofMesh that we start with, we'll save this so we can offset it from this and our mesh doesn't explode into oblivion.
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

class Textsplosion {
public:
    //Initializer
    Textsplosion();
    //Setters
    void setText(string _text);
    void setViewPosition(ofVec3f _pos);
    void setUpVector(ofVec3f _upVector ) { upVector = _upVector; };
    void setCam(ofEasyCam* _cam) { cam = _cam; };
    void setFont(ofTrueTypeFont* _font) { font = _font; };
    void setColor(int r, int g, int b, int a) { color = ofColor(r, g, b, a); };
    void setColor(int r, int g, int b) { setColor(r, g, b, 255); };
    void setIsSelected(bool _selected) { isSelected = _selected; };
	void setCenter(ofVec3f _pos) { textCenter = _pos; };

    //Getters
    string getText() { return text; };
    ofVec3f getViewPosition() { return viewPosition; };
    ofVec3f getUpVector() { return upVector; };
    
    //Functionality
    void draw();
    void update();
private:
	//Core variables
    string text;
	ofVec3f viewPosition;
	ofVec3f upVector;
	ofMesh originalMesh;
	ofMesh displayMesh;
	ofVec3f textCenter;
	float rotationTheta;
	ofVec3f rotationAxis;

	//Looks stuff
    ofColor color;
	bool isSelected;
	float noise;


	//References to outside things used for calculations
	ofEasyCam* cam;
	ofTrueTypeFont* font;
};

#endif /* textsplosion_hpp */
