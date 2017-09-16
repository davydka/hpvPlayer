#include "ofMain.h"
#include "ofApp.h"

int main( )
{
    ofGLWindowSettings settings;
    settings.width = 1024;
    settings.height = 512;
    settings.windowMode = OF_WINDOW;
    // settings.windowMode = OF_FULLSCREEN;
    settings.setGLVersion(4, 1);
    
    ofCreateWindow(settings);

	ofRunApp(new ofApp());
}
