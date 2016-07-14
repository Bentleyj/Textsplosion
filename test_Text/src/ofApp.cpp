#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    font = new ofTrueTypeFont();
    
    font->load("fonts/Plain-Light.ttf", 50, true, true, true);
    
    light.setPosition(0, 0, 0);
    
    cam.lookAt(ofVec3f(0, 0, 0));
    
    cam.disableMouseMiddleButton();
    
    noiseOffset1 = 0;
    noiseOffset2 = 1000;
    noiseOffset3 = 2000;
    noiseOffset4 = 3000;
    
    noise = 0;
    
    textIndex = 0;
    
    ofSetLogLevel(OF_LOG_ERROR);
        
    cameraPosTarget = ofVec3f(0, 0, 400);
    camUpVectorTarget = ofVec3f(0, 1, 0);
    
    ofBackground(ofColor(0));
    //positions[0] = drawText("EDMOND SAFRA", 45, ofVec3f(0, 1, 0));
    //positions[1] = drawText("LILLY SAFRA", 45, ofVec3f(-.03, .50, 0));
    
//    ofHideCursor();
//    positions[2] = drawText("MARTA", 35, ofVec3f(.51, .88, 0));
//    positions[3] = drawText("JAMES", 143, ofVec3f(.27, -.68, 0));
//    positions[4] = drawText("SAM", 202, ofVec3f(.50, .33, 0));
//    positions[5] = drawText("AMALIE", 216, ofVec3f(-.62, .26, 0));
//    positions[6] = drawText("MARGAUX", 80, ofVec3f(.54, -.31, 0));
    
    vector<string> names;
    names.push_back("META-LOOPS");
    names.push_back("INTERACTION");
    names.push_back("COMMERCIAL");
    names.push_back("PETE");
    names.push_back("JOEL");
    names.push_back("AMALIE");
    names.push_back("SAM");
    
    texts.resize(names.size());
    
    for(int i = 0; i < names.size(); i++) {
        texts[i].setFont(font);
        texts[i].setCam(&cam);
        texts[i].setText(names[i]);
        texts[i].setViewPosition(ofVec3f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0), 0.0));
//        texts[i].setUpVector(ofVec3f(0, 1, 0));
    }

}

//--------------------------------------------------------------
void ofApp::update(){
    float newX = ofLerp(cam.getPosition().x, cameraPosTarget.x, 0.05);
    float newY = ofLerp(cam.getPosition().y, cameraPosTarget.y, 0.05);
    float newZ = ofLerp(cam.getPosition().z, cameraPosTarget.z, 0.05);
    
    cam.setPosition(newX, newY, newZ);
    
    float newUpX = ofLerp(cam.getUpDir().x, camUpVectorTarget.x, 0.05);
    float newUpY = ofLerp(cam.getUpDir().y, camUpVectorTarget.y, 0.05);
    float newUpZ = ofLerp(cam.getUpDir().z, camUpVectorTarget.z, 0.05);

    cam.lookAt(ofVec3f(0, 0, 0), ofVec3f(newUpX, newUpY, newUpZ));
    
    for(int i = 0; i < texts.size(); i++) {
        texts[i].update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
//    ofEnableDepthTest();
    //light.enable();
    
    float highestPercentage = 0.0;
    float lowestPercentage = 1.0;
    float highestTheta;
    int highestPercentageIndex;
    for(int i = 0; i < texts.size(); i++) {
        ofVec3f currentViewPos = cam.getPosition().normalize();
        ofVec3f textViewPos = texts[i].getViewPosition().normalize();
        float diff = (currentViewPos - textViewPos).length();
        float percent = ofMap(diff, 0.0, 2.0, 1.0, 0.0, true);
        if(percent > highestPercentage) {
            highestPercentage = percent;
            highestPercentageIndex  = i;
        } else if(percent < lowestPercentage) {
            lowestPercentage = percent;
        }
    }
    
//    if(ofGetKeyPressed()) {
        float distance = (cam.getPosition() - ofVec3f(0, 0, 0)).length();
        cameraPosTarget = distance * texts[highestPercentageIndex].getViewPosition();
        camUpVectorTarget = texts[highestPercentageIndex].getUpVector();
    
    for(int i = 0; i < texts.size(); i++) {
        ofColor col = ofColor(0, 0, 0);
        if(i == highestPercentageIndex) {
            texts[i].setIsSelected(true);
            //col.lerp(ofColor(255, 255, 255), highestPercentage);
        } else {
            texts[i].setIsSelected(false);
            //col.lerp(ofColor(127, 127, 127), 0.0);
        }
        texts[i].setColor(col.r, col.g, col.b);
        if(i != highestPercentageIndex) {
            texts[i].draw();
        }
    }
    
    texts[highestPercentageIndex].draw();

    
//    ofDrawAxis(10);
    
    cam.end();
    
    if(animating) {
        noise += 0.01;
    }
    
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        textIndex++;
        textIndex %= texts.size();
        float distance = (cam.getPosition() - ofVec3f(0, 0, 0)).length();
        cameraPosTarget = distance * texts[textIndex].getViewPosition();
        camUpVectorTarget = texts[textIndex].getUpVector();
        cout<<"camUpVectorTarget: "<<camUpVectorTarget.x<<", "<<camUpVectorTarget.y<<", "<<camUpVectorTarget.z<<endl;

    }
    if(key == 'a') {
        animating = !animating;
    }
    if(key == 'f') {
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
ofVec3f ofApp::drawText(string text, ofVec3f viewPositionNorm) {
    ofPushMatrix();
    float theta = atan(viewPositionNorm.y/viewPositionNorm.x);
    float phi = atan(sqrt(viewPositionNorm.x * viewPositionNorm.x + viewPositionNorm.y * viewPositionNorm.y)/(viewPositionNorm.z));
    ofRotateX(theta * 180 / PI);
    ofRotateY(phi * 180 / PI);
//    ofRotateX(viewPositionNorm.x);
//    ofRotateY(viewPositionNorm.y);
    //ofRotate(rotation, rotationAxis.x, rotationAxis.y, rotationAxis.z);
    ofSetColor(0, 255, 255);
    float dist = (ofVec3f(0, 0, 0) - cam.getPosition()).length();
    
    ofRectangle rect = font->getStringBoundingBox(text, 0, 0);
    
    vector<ofTTFCharacter> characters = font->getStringAsPoints(text);
    
    ofMesh mesh;

    for(int j = 0; j < characters.size(); j++) {
        vector<ofPolyline> lines = characters[j].getOutline();
        
        ofRectangle lineRect = lines[0].getBoundingBox();
        
        vector<ofPoint> points = lines[0].getVertices();
        
        for(int i = 0; i < points.size(); i++) {
            ofColor col = ofColor(0, 255, 255/*ofMap(i, 0, points.size(), 0, 255), 0, 0*/);
            mesh.addVertex(ofVec3f(points[i].x - rect.width/2, -points[i].y - lineRect.getHeight() + rect.height/2, 0));
            mesh.addColor(col);
            mesh.addVertex(ofVec3f(points[(i+1)%points.size()].x - rect.width/2, -points[(i+1)%points.size()].y - lineRect.getHeight() + rect.height/2, 0));
            mesh.addColor(col);
        }
    }
    
    float zOffset = ofMap(ofNoise(noise), 0, 1, 50, -50);
    for(int i = 0; i < mesh.getNumVertices(); i++) {
        ofVec3f vertex = mesh.getVertex(i);
//        vertex.x -= rect.width/2;
//        vertex.y += rect.height/2;
        if(i%2 == 0) zOffset = ofMap(ofNoise(noise + i), 0, 1, 500, -500);
        vertex.z += zOffset;
        float distanceToTarget = (cam.getPosition() - ofVec3f(0, 0, 0)).length();
        float distanceToObject = distanceToTarget - zOffset;
        float scale = distanceToObject / distanceToTarget;
        vertex.x *= scale;
        vertex.y *= scale;
        mesh.setVertex(i, vertex);
    }
    
    ofSetLineWidth(3);
    
    mesh.setMode(OF_PRIMITIVE_LINES);
    mesh.draw();
    ofPopMatrix();
    
    return viewPositionNorm;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    //cameraPosTarget = cam.getPosition();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::drawSquares() {
    float distanceToTarget = (cam.getPosition() - ofVec3f(0, 0, 0)).length();
    
    float minDist = 100;
    float maxDist = -100;
    
    ofSetColor(0, 0, 255);
    float zOffset = ofMap(ofNoise(noise + noiseOffset1), 0, 1.0, minDist, maxDist);
    float distanceToObject = distanceToTarget - zOffset;
    float scale = distanceToObject / distanceToTarget;
    ofDrawBox(12.5*scale, 12.5*scale, zOffset, 25*scale, 25*scale, 0);
    
    zOffset = ofMap(ofNoise(noise + noiseOffset2), 0, 1.0, minDist, maxDist);
    distanceToObject = distanceToTarget - zOffset;
    scale = distanceToObject / distanceToTarget;
    ofDrawBox(-12.5*scale, 12.5*scale, zOffset, 25*scale, 25*scale, 0);
    
    zOffset = ofMap(ofNoise(noise + noiseOffset3), 0, 1.0, minDist, maxDist);
    distanceToObject = distanceToTarget - zOffset;
    scale = distanceToObject / distanceToTarget;
    ofDrawBox(-12.5*scale, -12.5*scale, zOffset, 25*scale, 25*scale, 0);
    
    zOffset = ofMap(ofNoise(noise + noiseOffset4), 0, 1.0, minDist, maxDist);
    distanceToObject = distanceToTarget - zOffset;
    scale = distanceToObject / distanceToTarget;
    ofDrawBox(12.5*scale, -12.5*scale, zOffset, 25*scale, 25*scale, 0);
    
    ofRotate(90, 1, 0, 0);
    ofSetColor(0, 255, 0);
    zOffset = ofMap(ofNoise(noise + noiseOffset1), 0, 1.0, minDist, maxDist);
    distanceToObject = distanceToTarget - zOffset;
    scale = distanceToObject / distanceToTarget;
    ofDrawBox(12.5*scale, 12.5*scale, zOffset, 25*scale, 25*scale, 0);
    
    zOffset = ofMap(ofNoise(noise + noiseOffset2), 0, 1.0, minDist, maxDist);
    distanceToObject = distanceToTarget - zOffset;
    scale = distanceToObject / distanceToTarget;
    ofDrawBox(-12.5*scale, 12.5*scale, zOffset, 25*scale, 25*scale, 0);
    
    zOffset = ofMap(ofNoise(noise + noiseOffset3), 0, 1.0, minDist, maxDist);
    distanceToObject = distanceToTarget - zOffset;
    scale = distanceToObject / distanceToTarget;
    ofDrawBox(-12.5*scale, -12.5*scale, zOffset, 25*scale, 25*scale, 0);
    
    zOffset = ofMap(ofNoise(noise + noiseOffset4), 0, 1.0, minDist, maxDist);
    distanceToObject = distanceToTarget - zOffset;
    scale = distanceToObject / distanceToTarget;
    ofDrawBox(12.5*scale, -12.5*scale, zOffset, 25*scale, 25*scale, 0);
    
    ofRotate(90, 0, 1, 0);
    
    ofSetColor(255, 0, 0);
    zOffset = ofMap(ofNoise(noise + noiseOffset1), 0, 1.0, minDist, maxDist);
    distanceToObject = distanceToTarget - zOffset;
    scale = distanceToObject / distanceToTarget;
    ofDrawBox(12.5*scale, 12.5*scale, zOffset, 25*scale, 25*scale, 0);
    
    zOffset = ofMap(ofNoise(noise + noiseOffset2), 0, 1.0, minDist, maxDist);
    distanceToObject = distanceToTarget - zOffset;
    scale = distanceToObject / distanceToTarget;
    ofDrawBox(-12.5*scale, 12.5*scale, zOffset, 25*scale, 25*scale, 0);
    
    zOffset = ofMap(ofNoise(noise + noiseOffset3), 0, 1.0, minDist, maxDist);
    distanceToObject = distanceToTarget - zOffset;
    scale = distanceToObject / distanceToTarget;
    ofDrawBox(-12.5*scale, -12.5*scale, zOffset, 25*scale, 25*scale, 0);
    
    zOffset = ofMap(ofNoise(noise + noiseOffset4), 0, 1.0, minDist, maxDist);
    distanceToObject = distanceToTarget - zOffset;
    scale = distanceToObject / distanceToTarget;
    ofDrawBox(12.5*scale, -12.5*scale, zOffset, 25*scale, 25*scale, 0);
}
