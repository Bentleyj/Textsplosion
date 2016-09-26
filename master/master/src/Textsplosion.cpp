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
    noise = 0;
    isSelected = false;
}

void Textsplosion::update() {
	// Update the noise
    noise += 0.001;
}

void Textsplosion::draw() {
	// We'll draw our display mesh each frame
    ofPushMatrix();
    
    float offset = 0;

	float distanceToTarget = (cam->getPosition() - textCenter).length();

	//We could replace this with a vertex shader... Would definitely make it more scalable.. That's what I'll start right now!
  //  for(int i = 0; i < originalMesh.getNumVertices(); i++) {
  //      ofVec3f vertex = originalMesh.getVertex(i);
  //      if(i%2 == 0) offset = ofMap(ofNoise(noise + i*0.1), 0, 1, distanceToTarget * 0.9, -distanceToTarget * 0.9);
  //      vertex.z += offset;
  //      float distanceToObject = distanceToTarget - offset;
  //      float scale = distanceToObject / distanceToTarget;
  //      vertex.x *= scale;
  //      vertex.y *= scale;
  //      float dist = (vertex - textCenter).length();
  //      float camDist = ( cam->getPosition() - textCenter).length();
  //      float percent = ofMap(dist, 0, camDist, 1.0, 0.0, true);
  //      displayMesh.setVertex(i, vertex);
  //      ofColor col = color;
  //      col.lerp(ofColor(220, 220, 220), percent);
  //      if(isSelected) col = ofColor(255);
		//displayMesh.setColor(i, col);
  //  }
    
    ofSetLineWidth(1);
    
//    ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(0, 0, 100));

    ofRotate(rotationTheta, rotationAxis.x, rotationAxis.y, rotationAxis.z);
    
    displayMesh.setMode(OF_PRIMITIVE_LINES);
	displayMesh.draw();
//    ofSetColor(255, 0, 0);
//    ofDrawArrow(ofVec3f(0, 0, 0), ofVec3f(0, 0, 100));

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
    cout<<"rotationTheta: "<< rotationTheta <<endl;
    
    ofQuaternion rotation = ofQuaternion(rotationTheta, rotationAxis);
    
    upVector = rotation * upVectorTemp;
    cout<<"upVector: "<<upVector.x<<", "<<upVector.y<<", "<<upVector.z<<endl;
}

void Textsplosion::setText(string _text) {
    text = _text;

	// Find the string bounding box for our font about the particular text we want to write.
	ofRectangle rect = font->getStringBoundingBox(text, 0, 0);

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
				originalMesh.addVertex(ofVec3f(points[i].x - rect.width / 2, -points[i].y - lineRect.getHeight() + rect.height / 2, 0));
				originalMesh.addColor(color);
				originalMesh.addVertex(ofVec3f(points[(i + 1) % points.size()].x - rect.width / 2, -points[(i + 1) % points.size()].y - lineRect.getHeight() + rect.height / 2, 0));
				originalMesh.addColor(color);

				originalMesh.addVertex(ofVec3f(points[i].x - rect.width / 2, -points[i].y - lineRect.getHeight() + rect.height / 2, 0));
				originalMesh.addColor(color);
				originalMesh.addVertex(ofVec3f(points[(i + 1) % points.size()].x - rect.width / 2, -points[(i + 1) % points.size()].y - lineRect.getHeight() + rect.height / 2, 0));
				originalMesh.addColor(color);
			}
		}

		displayMesh = originalMesh;
	}
};
