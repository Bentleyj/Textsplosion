//
//  Textsplosion.cpp
//  test_Text
//
//  Created by James Bentley on 08/07/2016.
//
//

#include "Textsplosion.h"

Textsplosion::Textsplosion() {
    noise = 0;
    isSelected = false;
}

void Textsplosion::update() {
    //noise += 0.001;
}

void Textsplosion::draw() {
    ofPushMatrix();
    
    float dist = (ofVec3f(0, 0, 0) - cam->getPosition()).length();
    
    ofRectangle rect = font->getStringBoundingBox(text, 0, 0);
    
    vector<ofTTFCharacter> characters = font->getStringAsPoints(text);
    
    mesh.clear();
    
    for(int j = 0; j < characters.size(); j++) {
        vector<ofPolyline> lines = characters[j].getOutline();
        
        ofRectangle lineRect = lines[0].getBoundingBox();
        
        vector<ofPoint> points = lines[0].getVertices();
        
        for(int i = 0; i < points.size(); i++) {
            mesh.addVertex(ofVec3f(points[i].x - rect.width/2, -points[i].y - lineRect.getHeight() + rect.height/2, 0));
            ofVec3f pos = ofVec3f(points[i].x - rect.width/2, -points[i].y - lineRect.getHeight() + rect.height/2);
            mesh.addColor(color);
            mesh.addVertex(ofVec3f(points[(i+1)%points.size()].x - rect.width/2, -points[(i+1)%points.size()].y - lineRect.getHeight() + rect.height/2, 0));
            mesh.addColor(color);
        }
    }
    
    float offset = 0;
    for(int i = 0; i < mesh.getNumVertices(); i++) {
        ofVec3f vertex = mesh.getVertex(i);
        if(i%2 == 0) offset = ofMap(ofNoise(noise + i*0.1), 0, 1, 500, -500);
        vertex.z += offset;
        float distanceToTarget = (cam->getPosition() - ofVec3f(0, 0, 0)).length();
        float distanceToObject = distanceToTarget - offset;
        float scale = distanceToObject / distanceToTarget;
        vertex.x *= scale;
        vertex.y *= scale;
        mesh.setVertex(i, vertex);
        float dist = (vertex - ofVec3f(0, 0, 0)).length();
        float camDist = ( cam->getPosition() - ofVec3f(0, 0, 0) ).length();
        float percent = ofMap(dist, 0, camDist, 1.0, 0.0, true);
        ofColor col = color;
        col.lerp(ofColor(220, 220, 220), percent);
        if(isSelected) col = ofColor(255);
        mesh.setColor(i, col);
    }
    
    ofSetLineWidth(3);
    
//    ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(0, 0, 100));

    ofRotate(theta, axis.x, axis.y, axis.z);
    
    mesh.setMode(OF_PRIMITIVE_LINES);
    mesh.draw();
//    ofSetColor(255, 0, 0);
//    ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(0, 0, 100));

    ofPopMatrix();
}

void Textsplosion::setViewPosition(ofVec3f _pos) {
    viewPosition = _pos.normalize();
    ofMatrix3x3 R;
    ofVec3f upVectorTemp = ofVec3f(0.0, 1.0, 0.0);
    ofVec3f initialVector = ofVec3f(0.0, 0.0, 1.0);
    cout<<"initial vector: "<<initialVector.x<<", "<<initialVector.y<<", "<<initialVector.z<<endl;

    ofVec3f newVector = _pos.normalize();
    cout<<"new vector: "<<newVector.x<<", "<<newVector.y<<", "<<newVector.z<<endl;

    ofMatrix3x3 I;
    I.set(1, 0, 0, 0, 1, 0, 0, 0, 1);
    
    ofVec3f v = initialVector.cross(newVector);
    cout<<"initial vector: "<<initialVector.x<<", "<<initialVector.y<<", "<<initialVector.z<<endl;
    cout<<"new vector: "<<newVector.x<<", "<<newVector.y<<", "<<newVector.z<<endl;
    cout<<"v: "<<v.x<<", "<<v.y<<", "<<v.z<<endl;
    float s = v.length();
    float c = initialVector.dot(newVector);
    
    ofMatrix3x3 skewSymV;
    skewSymV.set(0, -v.z, v.y, v.z, 0, -v.x, -v.y, v.x, 0);
    
    R = I + skewSymV + skewSymV * skewSymV * (1 - c) / (s * s);
    
    if (initialVector == newVector) {
        R = I;
    }
    
    cout<<"R: "<<endl<<R.a<<" "<<R.b<<" "<<R.c<<endl<<R.d<<" "<<R.e<<" "<<R.f<<endl<<R.g<<" "<<R.h<<" "<<R.i<<endl;

    axis = ofVec3f(R.h - R.f, R.c - R.g, R.d - R.b);
    axis = axis.normalize();
    cout<<"axis: "<<axis.x<<", "<<axis.y<<", "<<axis.z<<endl;
    
    float trace = R.a + R.e + R.i;
    
    theta = acos((trace - 1) / 2);
    theta *= 180 / PI;
    cout<<"theta: "<<theta<<endl;
    
    rotation = ofQuaternion(theta, axis);
    
    upVector = rotation * upVectorTemp;
    cout<<"upVector: "<<upVector.x<<", "<<upVector.y<<", "<<upVector.z<<endl;
    
//    rotation = ofQuaternion(theta, axis);

}

void Textsplosion::setText(string _text) {
    text = _text;
};
