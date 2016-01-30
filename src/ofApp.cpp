#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetFrameRate(60);
	ofBackground(30, 30, 30);

	camWidth = 400;  
	camHeight = 300;

	kinect.setRegistration(false);
	kinect.init(true);
	kinect.open();

	angle = 0;
	kinect.setCameraTiltAngle(angle);

	bDrawPointCloud = false;
	

	cam.setDeviceID(0);
	cam.setDesiredFrameRate(60);
	cam.initGrabber(camWidth, camHeight);

	recording = false;
	recorder.setPrefix(ofToDataPath("recording1/frame_")); // this directory must already exist
	recorder.setFormat("bmp"); // png is really slow but high res, bmp is fast but big, jpg is just right
	recorderAlpha.setPrefix(ofToDataPath("recording1/alpha_")); // this directory must already exist
	recorderAlpha.setFormat("bmp"); // png is really slow but high res, bmp is fast but big, jpg is just right

}

//--------------------------------------------------------------
void ofApp::update() {

	kinect.update();
	cam.update();

		if (cam.isFrameNew() && recording) {
		recorder.addFrame(cam);
		recorderAlpha.addFrame(kinect.getPixels());
		
		
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255, 255, 255);

	if (bDrawPointCloud) {
		easyCam.begin();
		drawPointCloud();
		easyCam.end();
	}
	else {
		kinect.draw(440, 10, camWidth, camHeight);
		cam.draw(20, 10);
	}



	if (recording) {
		ofSetColor(255, 0, 0);
		ofCircle(ofGetWidth() - 20, 20, 5);
	}

	ofSetColor(198, 246, 55);
	stringstream reportStream;
	reportStream << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl		
		<< "fps: " << ofGetFrameRate() << endl
		<< "Recording: " << recording << "\nThread running: " << recorder.isThreadRunning() << "\nQueue Size: " 
		<< recorder.q.size() << "\n\nPress 'r' to toggle recording.\nPress 't' to toggle worker thread." << endl
		;
	ofDrawBitmapString(reportStream.str(), 20, 400);

}

void ofApp::drawPointCloud() {
	int w = 640;
	int h = 480;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
	for (int y = 0; y < h; y += step) {
		for (int x = 0; x < w; x += step) {
			if (kinect.getDistanceAt(x, y) > 0) {
				mesh.addColor(kinect.getColorAt(x, y));
				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
			}
		}
	}
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards' 
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	ofEnableDepthTest();
	mesh.drawVertices();
	ofDisableDepthTest();
	ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	recorder.waitForThread();
	recorderAlpha.waitForThread();

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	if (key == 'r') {
		recording = !recording;
	}

	if (key == 't') {
		if (recorder.isThreadRunning()) {
			recorder.stopThread();
			recorderAlpha.stopThread();
		}
		else {
			recorder.startThread(false, true);
			recorderAlpha.startThread(false, true);
		}
	}
	switch (key) {

	case'p':
		bDrawPointCloud = !bDrawPointCloud;
		break;


	case 'w':
		kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
		break;

	case 'o':
		kinect.setCameraTiltAngle(angle); // go back to prev tilt
		kinect.open();
		break;

	case 'c':
		kinect.setCameraTiltAngle(0); // zero the tilt
		kinect.close();
		break;


	case OF_KEY_UP:
		angle++;
		if (angle>30) angle = 30;
		kinect.setCameraTiltAngle(angle);
		break;

	case OF_KEY_DOWN:
		angle--;
		if (angle<-30) angle = -30;
		kinect.setCameraTiltAngle(angle);
		break;
	}

}