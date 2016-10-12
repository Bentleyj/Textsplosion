//
//  Textsplosion.cpp
//  test_Text
//
//  Created by James Bentley on 08/07/2016.
//
//

#include "Textsplosion.h"
#include "ofxEasing.h"
#include "triangulator.h"

Textsplosion::Textsplosion() {
	//Set everything up by default (real simple)
	fadeStartTime = 0.0;
	fadeEndTime = 0.0;
	brightnessModifier = 0.3;
	distortFactor - 0.0;
	lineWidth = 1.0;
	shardSize = ofVec2f(1, 1);
}

void Textsplosion::update() {
	// Update the noise
	float now = ofGetElapsedTimef();

	if (now < fadeEndTime) {
		auto easingMethod = &ofxeasing::quart::easeIn;
		brightnessModifier = ofxeasing::map(now, fadeStartTime, fadeEndTime, brightnessModifier, targetBrightness, easingMethod);
	}
}

void Textsplosion::draw() {
	// We'll draw our display mesh each frame
    ofPushMatrix();

    ofSetLineWidth(lineWidth);

	float angle;
	ofVec3f axis;
	quat.getRotate(angle, axis);
	ofRotate(angle, axis.x, axis.y, axis.z);
	ofRotate(-90.0, 1.0, 0.0, 0.0);

	//ofDrawAxis(100);

	//(*shaders)[1].begin();
	//(*shaders)[1].setUniform1f("timeVal", 0.01);
	//(*shaders)[1].setUniform1f("distortAmount", distortFactor);
	//(*shaders)[1].setUniform3f("camPosition", cam->getPosition());
	//(*shaders)[1].setUniform1f("brightnessModifier", brightnessModifier);
	//(*shaders)[1].setUniformTexture("texture0", *img, 0);
	//img->bind();
	//backgroundMesh.setMode(OF_PRIMITIVE_TRIANGLES);
	//backgroundMesh.draw();
	//img->unbind();
	//(*shaders)[1].end();

	//ofSetColor(color1);
	(*shaders)[0].begin();
	(*shaders)[0].setUniform1f("timeVal", ofGetElapsedTimef() * 0.01);
	(*shaders)[0].setUniform1f("distortAmount", distortFactor);
	(*shaders)[0].setUniform3f("camPosition", cam->getPosition());
	(*shaders)[0].setUniform1f("brightnessModifier", brightnessModifier);
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	mesh.draw();
	(*shaders)[0].end();

    ofPopMatrix();
}

void Textsplosion::fadeIn(float duration) {
	fadeStartTime = ofGetElapsedTimef();
	fadeEndTime = fadeStartTime + duration;
	targetBrightness = 1.0;
}

void Textsplosion::fadeOut(float duration) {
	fadeStartTime = ofGetElapsedTimef();
	fadeEndTime = fadeStartTime +duration;
	targetBrightness = 0.2;
}

void Textsplosion::setViewPositionSpherical(float _r, float _theta, float _phi)
{
	//We set the view position using spherical coordinates
	float r = _r;
	float theta = _theta;
	float phi = _phi;

	//convert from spherical to cartesian coordinates
	float xpos = r * sin(theta * PI / 180.0) * sin(phi * PI / 180.0);
	float ypos = r * cos(theta * PI / 180.0);
	float zpos = r * sin(theta * PI / 180.0) * cos(phi * PI / 180.0);

	viewPosition = ofVec3f(xpos, ypos, zpos);

	viewPosition.normalize();

	float thetaRad = theta * PI / 180.0;
	float phiRad = phi * PI / 180.0;

	ofVec3f originalForward = ofVec3f(0.0, 1.0, 0.0);

	if (viewPosition == originalForward || viewPosition == originalForward * -1) {
		viewPosition = ofVec3f(1.0, 1.0, 1.0);
		viewPosition.normalize();
	}

	quat.makeRotate(originalForward, viewPosition);

	ofVec3f upVectorTemp = ofVec3f(0.0, 0.0, -1.0);

	upVector = quat * upVectorTemp;
}

void Textsplosion::setText(string _text) {
    text = _text;

	// Find the string bounding box for our font about the particular text we want to write.
	ofRectangle rect = font->getStringBoundingBox(text, 0, 0);
	//ofRectangle boundingBox = rect;
	//boundingBox.y = -boundingBox.y;
	//boundingBox.y -= boundingBox.height;
	//boundingBox.x -= boundingBox.width / 2;

	//boundingBox.width += 10;
	//boundingBox.height += 10;

	//boundingBox.x -= 5;
	//boundingBox.y -= 5;

	//backgroundMesh.addVertex(boundingBox.getTopLeft());
	//backgroundMesh.addVertex(boundingBox.getBottomLeft());
	//backgroundMesh.addVertex(boundingBox.getTopRight());
	//backgroundMesh.addVertex(boundingBox.getBottomRight());
	//backgroundMesh.addVertex(boundingBox.getTopLeft());
	//backgroundMesh.addVertex(boundingBox.getBottomRight());

	ofRectangle square = ofRectangle(-100, -100, 200, 200);

	int w = 200;//img->getWidth();
	int h = 200;//img->getHeight();//w * img->getWidth() / img->getHeight();

	//backgroundMesh.addVertex(ofVec3f(-100, -100, 0));
	//backgroundMesh.addVertex(ofVec3f(100, 100, 0));

	//backgroundMesh.addIndex(0);
	//backgroundMesh.addIndex(1);

	// Every bit is chosen
	int xStep = shardSize.x;
	int yStep = shardSize.y;
	for (int y = 0; y < h; y += yStep) {
		for (int x = 0; x < w; x += xStep) {
			backgroundMesh.addVertex(ofVec3f((x - w / 2), (y - h / 2), 0));
			//backgroundMesh.addColor(ofColor(255, 0, 0));
			backgroundMesh.addTexCoord(ofVec3f(x * (img->getWidth() / w), img->getHeight() - y * (img->getHeight() / h)));
			backgroundMesh.addVertex(ofVec3f((x + xStep - w / 2), (y - h / 2), 0));
			//backgroundMesh.addColor(ofColor(255, 0, 0));
			backgroundMesh.addTexCoord(ofVec3f((x + xStep) * (img->getWidth() / w), img->getHeight() -  y * (img->getHeight() / h)));
			backgroundMesh.addVertex(ofVec3f((x - w / 2), (y + yStep - h / 2), 0));
			//backgroundMesh.addColor(ofColor(255, 0, 0));
			backgroundMesh.addTexCoord(ofVec3f(x * (img->getWidth() / w), img->getHeight() - (y + yStep) * (img->getHeight() / h)));
		}
	}

	for (int y = h; y > 0; y -= yStep) {
		for (int x = w; x > 0; x -= xStep) {
			backgroundMesh.addVertex(ofVec3f((x - w / 2), (y - h / 2), 0));
			//backgroundMesh.addColor(ofColor(0, 0, 255));
			backgroundMesh.addTexCoord(ofVec3f(x * (img->getWidth() / w), img->getHeight() - y * (img->getHeight() / h)));
			backgroundMesh.addVertex(ofVec3f((x - xStep - w / 2), (y - h / 2), 0));
			//backgroundMesh.addColor(ofColor(0, 0, 255));
			backgroundMesh.addTexCoord(ofVec3f((x - xStep) * (img->getWidth() / w), img->getHeight() - y * (img->getHeight() / h)));
			backgroundMesh.addVertex(ofVec3f((x - w / 2), (y - yStep - h / 2), 0));
			//backgroundMesh.addColor(ofColor(0, 0, 255));
			backgroundMesh.addTexCoord(ofVec3f(x * (img->getWidth() / w), img->getHeight() - (y - yStep) * (img->getHeight() / h)));
		}
	}
	//Heres a random version

	// Get the text as a vector of ofTTFCharacters, these characters have a whole bunch of info about the text.
	vector<ofTTFCharacter> characters = font->getStringAsPoints(text);
	//characters[0].draw();
	// Go through all the characters
	for (int j = 0; j < characters.size(); j++) {
		// Get the outline of each character
		ofMesh inputMesh = characters[j].getTessellation();
		//ofTessellator tess;


		//for (int i = 0; i < tessMesh.getNumVertices(); i++) {
		//	mesh.addVertex(tessMesh.getVertex(i));
		//	mesh.addColor(ofColor(255, 255, 255));
		//}

		for (int i = 0; i < inputMesh.getNumVertices(); i++) {
			ofPoint vertex = inputMesh.getVertex(i);
			inputMesh.setVertex(i, ofPoint(vertex.x - rect.width / 2, -vertex.y - rect.height / 2, 0));
			float amount = (vertex.x) / (rect.getWidth());
			amount = (amount > 1.0) ? 1.0 : amount;
			amount = (amount < 0.0) ? 0.0 : amount;

			inputMesh.addColor(color1.getLerped(color2, amount));
		}

		Triangulator::generateTriangulation(&inputMesh, &mesh);

		//for (int k = 0; k < lines.size(); k++) {

		//	// Get the vertices of all the points in each of the lines
		//	vector<ofPoint> points = lines[k].getVertices();

		//	// Go through all the points and add them to the meshes
		//	for (int i = 0; i < points.size(); i++) {
		//		mesh.addVertex(ofVec3f(points[i].x - rect.width / 2, -points[i].y - rect.height / 2, 0));
		//		mesh.addColor(color1.getLerped(color2, points[i].x / (rect.getWidth() + 10)));
		//		mesh.addVertex(ofVec3f(points[(i + 1) % points.size()].x - rect.width / 2, -points[(i + 1) % points.size()].y - rect.height / 2, 0));
		//		int index2 = ((i + 1) > points.size() - 1) ? points.size() - 1 : i + 1;
		//		mesh.addColor(color1.getLerped(color2, points[index2].x / (rect.getWidth() + 10)));
		//	}
		//}
	}
};
