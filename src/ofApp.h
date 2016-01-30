#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxImageSequenceRecorder.h"



class ofApp : public ofBaseApp {
public:

	void setup();
	void update();
	void draw();
	void exit();

	void drawPointCloud();
	void keyPressed(int key);

	//kinect and point cloud
	ofEasyCam easyCam;
	ofxKinect kinect;
	bool bDrawPointCloud;
	int angle;

	//video
	ofVideoGrabber cam;
	int camWidth;
	int camHeight;

	// second image recorder
	ofxImageSequenceRecorder recorder;
	ofxImageSequenceRecorder recorderAlpha;
	bool recording;

};
