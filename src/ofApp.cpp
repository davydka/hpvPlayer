#include "ofApp.h"

int vidVar;
int _w = 320;
int _h = 320;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowPosition(0, 0);
	ofSetWindowShape(_w, _h);
	ofSetWindowTitle("hpvPlayer");

	/* Syphon setup */
	//mainOutputSyphonServer.setName("Screen Output");

	/* Init HPV Engine */
	HPV::InitHPVEngine();

	/* Create resources for new player */
	hpvPlayer.init(HPV::NewPlayer());

	/* Try to load file and start playback */
	if(hpvPlayer.load("videos/hpv/fract.hpv")){
		handleOpen();
	}

	/* Enable vertical sync, if file has fps > 60, you might have to set to false */
	ofSetVerticalSync(true);

	/* Alternatively, if you experience playback stutter, try to toggle double-buffering true/false
	 * Default: OFF
	 *
	 * hpvPlayer.setDoubleBuffered(true);
	 */
	//hpvPlayer.setDoubleBuffered(true);
}

//--------------------------------------------------------------
void ofApp::update(){
	/* Update happens on global level for all active players by HPV Manager */
	HPV::Update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	/* Draw the texture fullscreen */
	ofBackground(255, 0, 255);
	hpvPlayer.draw(0,0,_w,_h);

	/* Syphon output */
	//mainOutputSyphonServer.publishScreen();
}

//--------------------------------------------------------------
void ofApp::exit(){
    /* Cleanup and destroy HPV Engine upon exit */
    HPV::DestroyHPVEngine();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	std::cout<<vidVar<<std::endl;

	vidVar++;
	//Set MAX somewhere
	//if(vidVar > MAX-1) vidVar = 0;
	
	hpvPlayer.close();
	if(vidVar % 2){
		if (hpvPlayer.load("videos/hpv/motor.hpv")){
			handleOpen();
		}
	} else {
		hpvPlayer.close();
		if (hpvPlayer.load("videos/hpv/fract.hpv")){
			handleOpen();
		}
	}
}

void ofApp::handleOpen(){
	hpvPlayer.setLoopState(OF_LOOP_NORMAL);
	hpvPlayer.play();
}

//--------------------------------------------------------------
void ofApp::onHPVEvent(const HPVEvent& event)
{
	switch (event.type){
		case HPV::HPVEventType::HPV_EVENT_PLAY:
			cout << "'" << event.player->getFilename() << "': play event" << endl;
			break;
		case HPV::HPVEventType::HPV_EVENT_PAUSE:
			cout << "'" << event.player->getFilename() << "': pause event" << endl;
			break;
		case HPV::HPVEventType::HPV_EVENT_RESUME:
			cout << "'" << event.player->getFilename() << "': resume event" << endl;
			break;
		case HPV::HPVEventType::HPV_EVENT_STOP:
			cout << "'" << event.player->getFilename() << "': stop event" << endl;
			break;
		case HPV::HPVEventType::HPV_EVENT_LOOP:
			cout << "'" << event.player->getFilename() << "': loop event" << endl;
			break;
		case HPV::HPVEventType::HPV_EVENT_NUM_TYPES:
		default:
			break;
	}
}
