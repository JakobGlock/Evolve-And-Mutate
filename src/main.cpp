#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main(){
    ofGLWindowSettings settings;
    settings.setGLVersion(3, 3);
    settings.width = 1920;
    settings.height = 1080;
    settings.windowMode = OF_FULLSCREEN; //Start in fullscreen
    ofCreateWindow(settings);
    ofRunApp(new ofApp());
}
