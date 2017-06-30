//
//  Textsplosion.cpp
//  test_Text
//
//  Created by James Bentley on 08/07/2016.
//
//

#include "Textsplosion.h"
//#include "ofxEasing.h"
#include "triangulator.h"

Textsplosion::Textsplosion() {
	//Set everything up by default (real simple)
	//fadeStartTime = 0.0;
	//fadeEndTime = 0.0;
	brightnessModifier = 0.3;
	distortFactor - 0.0;
	lineWidth = 1.0;
	shardSize = ofVec2f(1, 1);
}

void Textsplosion::update() {
	// Update the noise
	float now = ofGetElapsedTimef();

	//if (now < fadeEndTime) {
		//auto easingMethod = &ofxeasing::quart::easeIn;
        brightnessModifier = ofLerp(brightnessModifier, targetBrightness, 0.1);//ofxeasing::map(now, fadeStartTime, fadeEndTime, brightnessModifier, targetBrightness, easingMethod);
	//}
}

void Textsplosion::draw() {
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

	//ofDrawAxis(100); // use this for debugging

    //This is our code for drawing imageswhich we don't really want to use, what we really want to be able to switch between these seemlessly either by combining the shaders or adding some logic to the textsplosion.
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
    
    // This is the shader you use for drawing text. I want to combine the image and text shaders in to 1 but haven't gotten around to it yet. Not even sure if it's a great idea.
	(*shaders)[0].begin();
    // This tells the shader what time it is and the scalar multiplied by ofGetElapsedTimef() can be used to control the speed of the noise.
	(*shaders)[0].setUniform1f("timeVal", ofGetElapsedTimef() * 0.01);
    // This tells the shader what the maximum distortion factor should be, ie the furthest from the origin that the shards are allowed to wobble
	(*shaders)[0].setUniform1f("distortAmount", distortFactor);
    // Need to tell the shader about the position of the camera
	(*shaders)[0].setUniform3f("camPosition", cam->getPosition());
    // Also tell the shader about how bright our text is. This is used for highlighing, a value of 0 goes to black and value of 1 goes to white, default is set in the constructor
    // Of Textsplosion objects to be 0.3
	(*shaders)[0].setUniform1f("brightnessModifier", brightnessModifier);
    // Set the two colors for the background shards, ***I'm going to write a helper method that converts these now and will delete this text when I've done it.*** Did it!! See below! it's called ColorToUniformRange
	(*shaders)[0].setUniform4f("col1", ColorToUniformRange(backgroundColor1));
	(*shaders)[0].setUniform4f("col2", ColorToUniformRange(backgroundColor1));
	mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	mesh.draw();
	(*shaders)[0].end();

    ofPopMatrix();
}

// This method takes an ofColor and converts all it values to a ofVec4f with values between 0 and 1 so we can pass it in to the shader and don't have the rescale it per vertex or per pixel.
ofVec4f Textsplosion::ColorToUniformRange(ofColor col) {
    return ofVec4f(col.r / 255.0, col.g / 255.0, col.b / 255.0, col.a / 255.0);
}

//This fades in the Text by setting it's targetBrightness to 1.0
void Textsplosion::fadeIn() {
	targetBrightness = 1.0;
}

//This fades out the text by setting its target brightness to the default which is currently 0.3
void Textsplosion::fadeOut() {
	targetBrightness = 0.3;
}

/* 
 This is the most important method in the WHOLE class, basically you set a view position for the object based on spherical coordinates, so you move the amera to a random point on the sphere and this calculates all the important stuff:
 viewPosition -> Where the camera needs to sit in Cartesian coordinates (simple conversion from spherical to cartesian)
 quat -> the quternion representing the rotation that needs to happen in order to point the text at the correct viewPosition
 upVector -> the upwards facing vector for the text or image after we've rotated with the quaternion
 */

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
