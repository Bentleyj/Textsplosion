//
//  Explosion.h
//  master
//
//  Created by James Bentley on 12/20/16.
//
//

#ifndef Explosion_hpp
#define Explosion_hpp

#include "ofMain.h"

class Explosion {
public:
    //Initializer
    Explosion();
    
    //Setters
    // void setText(string _text);
    void setViewPositionSpherical(float _r, float _theta, float _phi);
    void setUpVector(ofVec3f _upVector ) { upVector = _upVector; };
    void setCam(ofEasyCam* _cam) { cam = _cam; };
    // void setFont(ofTrueTypeFont* _font) { font = _font; };
    // void setImg(ofImage* _img) { img = _img; };
    // void setColor1(int r, int g, int b, int a) { color1 = ofColor(r, g, b, a); };
    // void setColor2(int r, int g, int b, int a) { color2 = ofColor(r, g, b, a); };
    /*
    void setColor(int r, int g, int b) {
        setColor1(r, g, b, 255);
        setColor2(r, g, b, 255);
    };
    */
    //void setBackgroundColor1(int r, int g, int b, int a) { backgroundColor1 = ofColor(r, g, b, a); };
    //void setBackgroundColor2(int r, int g, int b, int a) { backgroundColor2 = ofColor(r, g, b, a); };
    /*void setBackgroundColor(int r, int g, int b) {
        setBackgroundColor1(r, g, b, 255);
        setBackgroundColor2(r, g, b, 255);
    };
     */
    // void setBackgroundColor1(ofColor col) { backgroundColor1 = col; };
    // void setBackgroundColor2(ofColor col) { backgroundColor2 = col; };
    // void setColor1(ofColor col) { color1 = col; };
    // void setColor2(ofColor col) { color2 = col; };
    
   // void setColorGradient(int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2) { setColor1(r1, g1, b1, a1); setColor2(r2, g2, b2, a2); };
   // void setColorGradient(int r1, int g1, int b1, int r2, int g2, int b2) { setColor1(r1, g1, b1, 255.0); setColor2(r2, g2, b2, 255.0); };
    
    void setCenter(ofVec3f _pos) { textCenter = _pos; };
    void setShader(ofShader* _shader) { shader = _shader; };
    void setDistortFactor(float _distortFactor) { distortFactor = _distortFactor; };
    void setLineWidth(float _lineWidth) { lineWidth = _lineWidth; };
//    void setShardSize(ofVec2f _size) {
//        int width = 200;//(int)(img->getWidth());
//        int height = 200;//(int)(img->getWidth());
//        
//        //if ((int)(_size.x) % width) {
//        //	ofLogError("Textsplosion::setShardSize(): Warning, the width of your image: " + ofToString(width) + " does not divide evenly into your shard size x: " + ofToString(_size.x) + ".\n Setting anyway but expect holes.\n Options are that do divide evenly are:");
//        //	for (int i = 0; i < width; i++) {
//        //		if (i % width == 0) {
//        //			ofLogError("Texsplosion::setShardSize(): " + ofToString(i) + "\n");
//        //		}
//        //	}
//        //}
//        //if ((int)(_size.y) % (int)(height)) {
//        //	ofLogError("Warning, the width of your image: " + ofToString(height) + " does not divide evenly into your shard size x: " + ofToString(_size.y) + ".\n Setting anyway but expect holes\n Options are that do divide evenly are:");
//        //	for (int i = 0; i < width; i++) {
//        //		if (i%width == 0) {
//        //			ofLogError("Texsplosion::setShardSize(): " + ofToString(i) + "\n");
//        //		}
//        //	}
//        //}
//        shardSize = _size;
    //};
    
    //void setShardSize(float _x, float _y) { setShardSize(ofVec2f(_x, _y)); };
    
    //Getters
    string getText() { return text; };
    ofVec3f getViewPosition() { return viewPosition; };
    ofVec3f getUpVector() { return upVector; };
    ofVec3f getCenter() { return textCenter; };
    float getBrightnessModifier() { return brightnessModifier; };
    float getDistortFactor() { return distortFactor; };
    //ofVec2f getShardSize() { return shardSize; };
    
    //Functionality
    virtual void draw();
    virtual void update();
    virtual void fadeIn();
    virtual void fadeOut();
    
    //Utility
    ofVec4f ColorToUniformRange(ofColor col);
    
private:
    //Core variables
    string text;
    ofVec3f viewPosition;
    ofVec3f upVector;
    ofVboMesh mesh;
    ofVboMesh backgroundMesh;
    ofQuaternion quat;
    //ofVec2f shardSize;
    ofShader* shader;
    ofVec3f textCenter;
    float rotationTheta;
    ofVec3f rotationAxis;
    
    //uniforms to go to the shader
    float distortFactor;
    
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

};


#endif /* Explosion_hpp */
