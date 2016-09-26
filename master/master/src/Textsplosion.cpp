//
//  Textsplosion.cpp
//  test_Text
//
//  Created by James Bentley on 08/07/2016.
//
//

#include "Textsplosion.h"

Textsplosion::Textsplosion() {
	//Set everything up by default (real simple)
    isSelected = false;
}

void Textsplosion::update() {
	// Update the noise
}

void Textsplosion::draw() {
	// We'll draw our display mesh each frame
    ofPushMatrix();
        
    ofSetLineWidth(1);
    
//    ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(0, 0, 100));
	//ofTranslate(textCenter);

    ofRotate(rotationTheta, rotationAxis.x, rotationAxis.y, rotationAxis.z);
	ofSetColor(color);

	(*shaders)[1].begin();
	(*shaders)[1].setUniform1f("timeVal", ofGetElapsedTimef() * 0.1);
	(*shaders)[1].setUniform1f("distortAmount", 1000.0);
	(*shaders)[1].setUniform3f("camPosition", cam->getPosition());
	(*shaders)[1].setUniform1i("isSelected", isSelected);
	backgroundMesh.setMode(OF_PRIMITIVE_TRIANGLES);
	backgroundMesh.draw();
	(*shaders)[1].end();


	ofSetColor(255, 255, 255);
	(*shaders)[0].begin();
	(*shaders)[0].setUniform1f("timeVal", ofGetElapsedTimef() * 0.01);
	(*shaders)[0].setUniform1f("distortAmount", 500.0);
	(*shaders)[0].setUniform3f("camPosition", cam->getPosition());
	(*shaders)[0].setUniform1i("isSelected", isSelected);
	mesh.setMode(OF_PRIMITIVE_LINES);
	mesh.draw();
	(*shaders)[0].end();


    ofPopMatrix();
}

void Textsplosion::setViewPosition(ofVec3f _pos) {
	//Given a position in space, calculate the up vector and the rotation.
	//
    viewPosition = _pos.normalize();
    ofMatrix3x3 R;
    ofVec3f upVectorTemp = ofVec3f(0.0, 1.0, 0.0);
    ofVec3f initialVector = ofVec3f(0.0, 0.0, 1.0);
    //cout<<"initial vector: "<<initialVector.x<<", "<<initialVector.y<<", "<<initialVector.z<<endl;

    ofVec3f newVector = _pos.normalize();
    //cout<<"new vector: "<<newVector.x<<", "<<newVector.y<<", "<<newVector.z<<endl;

    ofMatrix3x3 I;
    I.set(1, 0, 0, 0, 1, 0, 0, 0, 1);
    
    ofVec3f v = initialVector.cross(newVector);
    //cout<<"initial vector: "<<initialVector.x<<", "<<initialVector.y<<", "<<initialVector.z<<endl;
    //cout<<"new vector: "<<newVector.x<<", "<<newVector.y<<", "<<newVector.z<<endl;
    //cout<<"v: "<<v.x<<", "<<v.y<<", "<<v.z<<endl;
    float s = v.length();
    float c = initialVector.dot(newVector);
    
    ofMatrix3x3 skewSymV;
    skewSymV.set(0, -v.z, v.y, v.z, 0, -v.x, -v.y, v.x, 0);
    
    R = I + skewSymV + skewSymV * skewSymV * (1 - c) / (s * s);
    
    if (initialVector == newVector) {
        R = I;
    }
    
    //cout<<"R: "<<endl<<R.a<<" "<<R.b<<" "<<R.c<<endl<<R.d<<" "<<R.e<<" "<<R.f<<endl<<R.g<<" "<<R.h<<" "<<R.i<<endl;

    rotationAxis = ofVec3f(R.h - R.f, R.c - R.g, R.d - R.b);
	rotationAxis = rotationAxis.normalize();
    //cout<<"axis: "<<axis.x<<", "<<axis.y<<", "<<axis.z<<endl;
    
    float trace = R.a + R.e + R.i;
    
	rotationTheta = acos((trace - 1) / 2);
	rotationTheta *= 180 / PI;
    //cout<<"rotationTheta: "<< rotationTheta <<endl;
    
    ofQuaternion rotation = ofQuaternion(rotationTheta, rotationAxis);
    
    upVector = rotation * upVectorTemp;
    //cout<<"upVector: "<<upVector.x<<", "<<upVector.y<<", "<<upVector.z<<endl;
}

void Textsplosion::setText(string _text) {
    text = _text;

	// Find the string bounding box for our font about the particular text we want to write.
	ofRectangle rect = font->getStringBoundingBox(text, 0, 0);
	ofRectangle boundingBox = rect;
	boundingBox.y = -boundingBox.y;
	boundingBox.y -= boundingBox.height;
	boundingBox.x -= boundingBox.width / 2;

	boundingBox.width += 10;
	boundingBox.height += 10;

	boundingBox.x -= 5;
	boundingBox.y -= 5;

	backgroundMesh.addVertex(boundingBox.getTopLeft());
	backgroundMesh.addColor(color);
	backgroundMesh.addVertex(boundingBox.getBottomLeft());
	backgroundMesh.addColor(color);
	backgroundMesh.addVertex(boundingBox.getTopRight());
	backgroundMesh.addColor(color);
	backgroundMesh.addVertex(boundingBox.getBottomRight());
	backgroundMesh.addColor(color);
	backgroundMesh.addVertex(boundingBox.getTopLeft());
	backgroundMesh.addColor(color);
	backgroundMesh.addVertex(boundingBox.getBottomRight());
	backgroundMesh.addColor(color);

	backgroundMesh.addIndex(0);
	backgroundMesh.addIndex(1);
	backgroundMesh.addIndex(3);
	backgroundMesh.addIndex(4);
	backgroundMesh.addIndex(5);
	backgroundMesh.addIndex(2);

	// Get the text as a vector of ofTTFCharacters, these characters have a whole bunch of info about the text.
	vector<ofTTFCharacter> characters = font->getStringAsPoints(text);

	// Go through all the characters
	for (int j = 0; j < characters.size(); j++) {
		// Get the outline of each character
		vector<ofPolyline> lines = characters[j].getOutline();

		// Get the bounding box of the first outline (this should always be the largest outline)
		ofRectangle lineRect = lines[0].getBoundingBox();

		for (int k = 0; k < lines.size(); k++) {

			// Get the vertices of all the points in each of the lines
			vector<ofPoint> points = lines[k].getVertices();

			//Go through all the points and add them to the meshes
			for (int i = 0; i < points.size(); i++) {
				mesh.addVertex(ofVec3f(points[i].x - rect.width / 2, -points[i].y, 0));
				mesh.addColor(ofColor(255, 255, 255));
				mesh.addVertex(ofVec3f(points[(i + 1) % points.size()].x - rect.width / 2, -points[(i + 1) % points.size()].y, 0));
				mesh.addColor(ofColor(255, 255, 255));
			}
		}

		mesh = mesh;
	}
};
