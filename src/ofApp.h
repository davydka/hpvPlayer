#pragma once

#include "ofMain.h"
#include "ofxHPVPlayer.h"
#include "ofxMidi.h"
//#include "ofxSyphon.h"

class ofApp : public ofBaseApp, public ofxMidiListener{
public:
	void setup();
	void update();
	void draw();
	void exit();
    
	void onHPVEvent(const HPVEvent& event);
        
	void keyPressed(int key);

	void handleOpen();

	void newMidiMessage(ofxMidiMessage& eventArgs);
    
	//ofxSyphonServer mainOutputSyphonServer;
	ofxHPVPlayer hpvPlayer;

	ofxMidiIn midiIn;
	ofxMidiMessage midiMessage;

	ofVideoPlayer videoPlayer;
	
	/* LUT */
	void loadLUT(string path);
	void applyLUT(ofPixelsRef pix);
	
	bool doLUT;
	int dirLoadIndex;
	ofDirectory dir;
	ofPoint lutPos;
	ofPoint thumbPos;
	
	bool LUTloaded;
	ofVec3f lut[32][32][32];
	
	ofImage lutImg;
	/* END LUT */

};
