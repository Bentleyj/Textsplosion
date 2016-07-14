//
//  Textsplosion.h
//  test_Text
//
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
    void setRotation(float x, float y, float z);
    void setColor(int r, int g, int b, int a) { color = ofColor(r, g, b, a); };
    void setColor(int r, int g, int b) { setColor(r, g, b, 255); };
    void setIsSelected(bool _selected) { isSelected = _selected; };

    //Getters
    string getText() { return text; };
    ofVec3f getViewPosition() { return viewPosition; };
    ofVec3f getUpVector() { return upVector; };
    //Functionality
    void draw();
    void update();
private:
    string text;
    ofColor color;
    ofVec3f viewPosition;
    ofVec3f upVector;
    ofQuaternion rotation;
    bool isSelected;
    float theta;
    ofVec3f axis;
    ofEasyCam* cam;
    ofTrueTypeFont* font;
    float noise;
    ofMesh mesh;
};

#endif /* textsplosion_hpp */
