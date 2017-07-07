#pragma once

#include "ofMain.h"
#include "ofxHPVPlayer.h"
//#include "ofxSyphon.h"

class ofApp : public ofBaseApp
{
public:
	void setup();
	void update();
	void draw();
	void exit();
    
	void onHPVEvent(const HPVEvent& event);
        
	void keyPressed(int key);

	void handleOpen();
    
	//ofxSyphonServer mainOutputSyphonServer;
	ofxHPVPlayer hpvPlayer;
};
