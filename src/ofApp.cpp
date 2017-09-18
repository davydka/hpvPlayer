#include "ofApp.h"

int vidVar;
int _w = 320;
int _h = 320;
int velHolder = 0;

char noteHolder[4];
ofPixels pix;
ofImage img;

//string noteHolder = 0;

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetWindowPosition(0, 0);
	ofSetWindowShape(_w, _h);
	ofSetWindowTitle("hpvPlayer");

	/* Enable vertical sync, if file has fps > 60, you might have to set to false */
	ofSetVerticalSync(true);

	/* Syphon setup */
	//mainOutputSyphonServer.setName("Screen Output");

	/* Init HPV Engine */
	// HPV::InitHPVEngine();

	/* Create resources for new player */
	// hpvPlayer.init(HPV::NewPlayer());

	/* Try to load file and start playback */
	// if(hpvPlayer.load("videos/04.hpv")){
	// 	handleOpen();
	// }

	/* Alternatively, if you experience playback stutter, try to toggle double-buffering true/false
	 * Default: OFF
	 *
	 * hpvPlayer.setDoubleBuffered(true);
	 */
	//hpvPlayer.setDoubleBuffered(true);
	

	midiIn.listPorts();
	midiIn.openPort(1);
	//midiIn.openPort("from Max 1");
	midiIn.ignoreTypes(false, false, false);
	midiIn.addListener(this);
	midiIn.setVerbose(false);
	//snprintf (noteHolder, 6, "%06d", 0);
	
	/* plain video */
	videoPlayer.load("videos/07.mp4");
	videoPlayer.setLoopState(OF_LOOP_NORMAL);
	videoPlayer.setVolume(0);
	videoPlayer.play();
	/* end plain video */

	/* LUT */
	dir.allowExt("cube");
	dir.listDir("LUTs/");
	dir.sort();
	if (dir.size()>0) {
		dirLoadIndex=0;
		loadLUT(dir.getPath(dirLoadIndex));
		doLUT = true;
	}else{
		doLUT = false;
	}

	lutImg.allocate(_w, _h, OF_IMAGE_COLOR);
	/* END LUT */
}

//--------------------------------------------------------------
void ofApp::update(){
	/* Update happens on global level for all active players by HPV Manager */
	// HPV::Update();
	videoPlayer.update();

	if(midiMessage.velocity != 0 && midiMessage.velocity != velHolder){
		snprintf (noteHolder, 4, "%03d", midiMessage.pitch);

		//std::cout<<noteHolder<<std::endl;
		//std::cout<<midiMessage.pitch<<std::endl;

		// hpvPlayer.close();
		// if (hpvPlayer.load("videos/hpv/"+ofToString(noteHolder)+".hpv")){
		// 	handleOpen();
		// }
	}

	velHolder = midiMessage.velocity;

	/* LUT */
	if (doLUT) {
		pix = videoPlayer.getPixels();
		img.setFromPixels(pix);
		img.resize(_w,_h);
		applyLUT(img.getPixels());
		// applyLUT(videoPlayer.getPixels());
	}
	/* END LUT */
}

//--------------------------------------------------------------
void ofApp::draw(){
	/* Draw the texture fullscreen */
	ofBackground(0, 0, 0);
	// hpvPlayer.draw(0,0,_w,_h);
	// videoPlayer.draw(0,0,_w,_h);

	/* Syphon output */
	//mainOutputSyphonServer.publishScreen();

	/* LUT */
	if(doLUT){
		lutImg.draw(0,0,_w,_h);
	}else {
		videoPlayer.draw(0,0,_w,_h);
	}
	/* END LUT */
}

//--------------------------------------------------------------
void ofApp::exit(){
    /* Cleanup and destroy HPV Engine upon exit */
    // HPV::DestroyHPVEngine();
}

void ofApp::loadLUT(string path){
	LUTloaded=false;
	
	ofFile file(path);
	string line;
	for(int i = 0; i < 5; i++) {
		getline(file, line);
		ofLog() << "Skipped line: " << line;	
	}
	for(int z=0; z<32; z++){
		for(int y=0; y<32; y++){
			for(int x=0; x<32; x++){
				ofVec3f cur;
				file >> cur.x >> cur.y >> cur.z;
				lut[x][y][z] = cur;
			}
		}
	}
	
	LUTloaded = true;
}

void ofApp::applyLUT(ofPixelsRef pix){
	if (LUTloaded) {
		
		for(int y = 0; y < pix.getHeight(); y++){
			for(int x = 0; x < pix.getWidth(); x++){
				
				ofColor color = pix.getColor(x, y);
				
				int lutPos [3];
				for (int m=0; m<3; m++) {
					lutPos[m] = color[m] / 8;
					if (lutPos[m]==31) {
						lutPos[m]=30;
					}
				}
				
				ofVec3f start = lut[lutPos[0]][lutPos[1]][lutPos[2]];
				ofVec3f end = lut[lutPos[0]+1][lutPos[1]+1][lutPos[2]+1]; 
				
				for (int k=0; k<3; k++) {
					float amount = (color[k] % 8) / 8.0f;
					color[k]= (start[k] + amount * (end[k] - start[k])) * 255;
				}
				
				lutImg.setColor(x, y, color);
				
			}			
		}

		// lutImg.resize(_w,_h);
		lutImg.update();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	std::cout<<vidVar<<std::endl;

	vidVar++;
	//Set MAX somewhere
	// if(vidVar > MAX-1) vidVar = 0;
	
	// hpvPlayer.close();
	/*
	if(vidVar % 2){
		if (videoPlayer.load("videos/02.mp4")){
			handleOpen();
		}
	} else {
		// hpvPlayer.close();
		if (videoPlayer.load("videos/01.mp4")){
			handleOpen();
		}
	}
	*/

	switch (key) {
		case ' ':
			doLUT^=true;
			break;
		case OF_KEY_UP:
			dirLoadIndex++;
			if (dirLoadIndex>=(int)dir.size()) {
				dirLoadIndex=0;
			}
			loadLUT(dir.getPath(dirLoadIndex));
			
			break;
		case OF_KEY_DOWN:
			dirLoadIndex--;
			if (dirLoadIndex<0) {
				dirLoadIndex=dir.size()-1;
			}
			loadLUT(dir.getPath(dirLoadIndex));
			break;
		case OF_KEY_LEFT:
			if (videoPlayer.load("videos/06.MOV")){
				handleOpen();
			}
			break;
		case OF_KEY_RIGHT:
			if (videoPlayer.load("videos/04.mp4")){
				handleOpen();
			}
			break;
		default:
			break;
	}

}

void ofApp::handleOpen(){
	videoPlayer.setVolume(0);
	videoPlayer.setLoopState(OF_LOOP_NORMAL);
	videoPlayer.play();
	// hpvPlayer.setLoopState(OF_LOOP_NORMAL);
	// hpvPlayer.play();
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

void ofApp::newMidiMessage(ofxMidiMessage& msg) {
	midiMessage = msg;
}
