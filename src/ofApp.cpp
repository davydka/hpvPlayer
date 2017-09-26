#include "ofApp.h"

int vidVar;
int _w = 320;
int _h = 320;
int velHolder = 0;
int velHolderV = 0;

int midiChannel = 4;
int midiPgm = 0;
std::string videosFolder = "00_golden_clone/";

char noteHolder[4];
ofPixels pix;
ofImage img;

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

	midiIn.listPorts();
	midiIn.openPort(1);
	//midiIn.openPort("from Max 1");
	midiIn.ignoreTypes(false, false, false);
	midiIn.addListener(this);
	midiIn.setVerbose(false);
	snprintf (noteHolder, 4, "%03d", 0);
	
	/* plain video */
	videoPlayer.load("videos/box.mp4");
	handleOpen();
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
	videoPlayer.update();

	// Pgm Changes
	if(midiMessage.status == 192 && midiMessage.value != midiPgm && midiMessage.channel == midiChannel) {
	// if(midiMessage.status == 192 && midiMessage.value != midiPgm) {
	// if(midiMessage.velocity != 0 && midiMessage.velocity != velHolderV && midiMessage.channel == 4){
		cout << midiMessage.status << endl;
		cout << midiMessage.value << endl;
		cout << midiMessage.channel << endl;

		midiPgm = midiMessage.value;
		
		if(midiPgm == 0){
			videosFolder = "00_golden_clone/";
		}
		else if(midiPgm == 1){
			videosFolder = "01_bloomblahs/";
		}
		else if(midiPgm == 2){
			videosFolder = "02_drag_race/";
		}
		else if(midiPgm == 3){
			videosFolder = "03_higher_ups/";
		}
		else if(midiPgm == 4){
			videosFolder = "04_weekend_trip/";
		}

		cout << videosFolder << endl;
		snprintf (noteHolder, 4, "%03d", 0);
		videoPlayer.load("videos/" + videosFolder + ofToString(noteHolder) + ".mp4");
		handleOpen();
	}

	// Note events
	if(midiMessage.velocity != 0 && midiMessage.velocity != velHolder && midiMessage.channel == midiChannel){
		cout << ofToString(midiMessage.channel) << endl;
		snprintf (noteHolder, 4, "%03d", midiMessage.pitch);
		// cout << noteHolder << endl;
		videoPlayer.load("videos/" + videosFolder + ofToString(noteHolder) + ".mp4");
		handleOpen();
	}

	velHolder = midiMessage.velocity;

	/* LUT */
	if (doLUT) {
		pix = videoPlayer.getPixels();
		img.setFromPixels(pix);
		img.resize(_w,_h);
		applyLUT(img.getPixels());
	}
	/* END LUT */
}

//--------------------------------------------------------------
void ofApp::draw(){
	/* Draw the texture fullscreen */
	ofBackground(0, 0, 0);

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

		lutImg.update();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	std::cout<<vidVar<<std::endl;

	vidVar++;

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
			/*
			if (videoPlayer.load("videos/06.MOV")){
				handleOpen();
			}
			*/
			break;
		case OF_KEY_RIGHT:
			/*
			if (videoPlayer.load("videos/04.mp4")){
				handleOpen();
			}
			*/
			break;
		default:
			break;
	}

}

void ofApp::handleOpen(){
	videoPlayer.setVolume(0);
	videoPlayer.setLoopState(OF_LOOP_NORMAL);
	videoPlayer.play();
}

void ofApp::newMidiMessage(ofxMidiMessage& msg) {
	midiMessage = msg;
}
