//
//  Explosion.cpp
//  master
//
//  Created by James Bentley on 12/20/16.
//
//

#include "Explosion.h"

Explosion::Explosion() {
    brightnessModifier = 0.3;
    distortFactor = 0.0;
    lineWidth = 1.0;
    //shardSize = ofVec2f(1, 1);
}

void Explosion::update() {
    brightnessModifier = ofLerp(brightnessModifier, targetBrightness, 0.1);//ofxeasing::map(now, fadeStartTime, fadeEndTime, brightnessModifier, targetBrightness, easingMethod);
}

void Explosion::draw() {
    // We'll draw our display mesh each frame
    ofPushMatrix();
    
    // Set our line width in case we are drawing with lines, we aren't anymore but I think it did look pretty cool so may go back to it
    ofSetLineWidth(lineWidth);
    
    // annoyingly, ofRotate only takes an axis and some angles so we need to get the rotation from the quaternion we calculated when we initialized the view position and angle. Really stupid, ofRotate should have a quaternion overload.
    // Maybe I will write this at some point.
    float angle;
    ofVec3f axis;
    //Get those euler angles!
    quat.getRotate(angle, axis);
    // rotate by our saved rotation
    ofRotate(angle, axis.x, axis.y, axis.z);
    // I don't know why I need to do this but I do, need to investigate further, if you comment it out all the angles are off.
    ofRotate(-90.0, 1.0, 0.0, 0.0);
}

//This fades in the Text by setting it's targetBrightness to 1.0
void Explosion::fadeIn() {
    targetBrightness = 1.0;
}

//This fades out the text by setting its target brightness to the default which is currently 0.3
void Explosion::fadeOut() {
    targetBrightness = 0.3;
}

ofVec4f ColorToUniformRange(ofColor col) {
    return ofVec4f(0, 0, 0, 0);
}
