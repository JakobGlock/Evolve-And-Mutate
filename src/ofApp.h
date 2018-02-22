#pragma once

#include "ofMain.h"

/* Addons */
#include "ofxGaussian.h"
#include "ofxOpenCv.h"

/* Custom Classes */
#include "PingPongBuffer.h"
#include "Population.h"

class ofApp : public ofBaseApp{

public:
    /* Methods */
    void setup();
    void update();
    void draw();

    /* Custom Methods */
    void loadShaders(int _index);
    void analyseImage();
    float calcVariance();
    float calcBlobSepDistance();

    /* OF */
    ofFbo scene;
    ofShader posShader, renderShader;
    PingPongBuffer posPingPong;
    ofVboMesh myMesh;
    ofxGuassian gausRand;

    /* Varibales */
    int numParticles, textureRes, popSize;
    int renderX, renderY;
    int index, frameCount;
    int getStringChar;
    float time, timeOut, timeStep;
    string storeString, drawString;

    /* Custom object */
    Population myPopulation;

    /* Blob detection */
    ofPixels myPixels;
    ofxCvColorImage colImgCv;
    ofxCvGrayscaleImage imgCv;
    ofxCvContourFinder finder;
};
