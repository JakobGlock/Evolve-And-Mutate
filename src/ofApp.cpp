#include "ofApp.h"
#define STRINGIFY(A) #A

//--------------------------------------------------------------
void ofApp::setup(){

    /* Genaral Setup */
    ofHideCursor();
    ofBackground(255);

    /* The render dimensions */
    renderX = 1920;
    renderY = 1080;

    /* Set number of particles and texture size */
    numParticles = 250000;
    textureRes = (int)sqrt((float)numParticles);
    numParticles = textureRes * textureRes;

    /* Allocate space for my pingpong buffer */
    posPingPong.allocate(textureRes, textureRes, GL_RGB32F);

    /* Create population and load the first shader */
    popSize = 100;
    myPopulation.createPopulation(popSize);
    loadShaders(0);
    storeString = myPopulation.getIndividualPheno(0);

    /* Blob detection */
    myPixels.allocate(renderX, renderY, GL_RGB);
    imgCv.allocate(renderX, renderY);
    colImgCv.allocate(renderX, renderY);

    /* Allocate space for the scene FBO */
    scene.allocate(renderX, renderY, GL_RGB32F);
    scene.begin();
    ofClear(255, 255, 255, 255);
    scene.end();

    /* Initalize some variables */
    index = 0;
    time = 0;
    timeStep = 0.01;
    frameCount = 0;
    timeOut = 4;

    /* Initalize mesh with some points */
    myMesh.setMode(OF_PRIMITIVE_POINTS);
    for(int x = 0; x < textureRes; x++){
        for(int y = 0; y < textureRes; y++){
            float xx = ofMap(x, 0, textureRes, -PI, PI);
            float yy = ofMap(y, 0, textureRes, -PI, PI);
            myMesh.addVertex(ofVec3f(xx,yy));
            myMesh.addTexCoord(ofVec2f(x, y));
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){

    /* Increase time */
    time += timeStep;

    if(time >= timeOut)
    {
        /* Calculate the blob detection */
        scene.readToPixels(myPixels);
        colImgCv.setFromPixels(myPixels);
        imgCv = colImgCv;
        imgCv.threshold(140, true);
        finder.findContours(imgCv, 1, 86800, 100, false);

        /* Analyse the image */
        analyseImage();

        index++;

        if(index % popSize == 0)
        {
            /* Create a new population */
            myPopulation.newPopulation();
        }

        /* Load a new shader */
        loadShaders(index%popSize);
        storeString = myPopulation.getIndividualPheno(index%popSize);
        drawString.clear();
        frameCount = 0;
        time = 0;
    }

    /* Calculate particle positions on the GPU */
    posPingPong.dst->begin();
    ofClear(0);
    posShader.begin();
    posShader.setUniformTexture("posData", posPingPong.src->getTexture(), 0); // passing the position information
    posShader.setUniform1f("time", time);
    posShader.setUniform1f("PI", PI);
    posShader.setUniform1f("TWO_PI", TWO_PI);

    /* Draw the source FBO */
    posPingPong.src->draw(0, 0);

    posShader.end();
    posPingPong.dst->end();

    /* Draw to the render FBO */
    scene.begin();
    //ofClear(255,255,255,255);
    renderShader.begin();
    renderShader.setUniformTexture("posTexture", posPingPong.dst->getTexture(), 0);
    renderShader.setUniform1i("res", (float)textureRes);
    renderShader.setUniform2f("screen", (float)renderX, (float)renderY);
    renderShader.setUniform1f("PI", PI);
    renderShader.setUniform1f("TWO_PI", TWO_PI);

    /* Enable blend mode and draw the points */
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofTranslate(renderX/2, renderY/2);
    ofSetColor(0, 0, 0, 10);
    glPointSize(1.);
    myMesh.draw();
    ofDisableBlendMode();

    glEnd();
    renderShader.end();
    scene.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    /* Draw my main scene FBO */
    ofSetColor(255);
    scene.draw(0, 0, ofGetWidth(), ofGetHeight());

    /* Draw the blobs, for debugging */
    //finder.draw(0,0);

    /* Draw some info to the screen for debugging */
    ofSetColor(0);
    //ofDrawBitmapString("nBlobs: " + ofToString(finder.nBlobs), ofGetWidth()-100, 30);
    //ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), ofGetWidth()-100, 15);
    //ofDrawBitmapString("Fitness: " + ofToString(myPopulation.myPopulation[index%popSize]->getFitness()), ofGetWidth()-100, 45);
    //ofDrawBitmapString("Time: " + ofToString(time), 15, 45);

    /* Read a string ten characters at a time */
    if(frameCount < storeString.length())
    {
        for(int i=frameCount; i<frameCount+10 && i<(int)storeString.length(); i++){
            drawString += storeString.at(i);
        }
        frameCount+=10;
    }

    /* Draw the string and info */
    ofSetColor(0, 0, 0, ofMap(time, 0, 2, 255, 0, true));
    ofDrawBitmapString(drawString, 15,60);

    ofSetColor(0);
    ofDrawBitmapString("Generation: " + ofToString(myPopulation.getGeneration()), 15, 15);
    ofDrawBitmapString("Individual: " + ofToString((index%popSize)+1) + " of " + ofToString(popSize), 15, 30);
}

//-------------------------------------------------------------
/* This function analyses the image */
void ofApp::analyseImage(){

    /* Calculate total distance between blobs and the variance of an image */
    float distSum = calcBlobSepDistance();
    float varSum = calcVariance();

    /* Set the fitness based on the number of blobs */
    myPopulation.myPopulation[index%popSize]->setFitness(finder.nBlobs, distSum, varSum);

    /* Print values to the console, for debugging */
    //cout << "Fitness: " + ofToString(myPopulation.myPopulation[index%popSize]->getFitness()) + "  -  ";
    //cout << "Total Distance: " + ofToString(distSum) + "  -  ";
    //cout << "Variance: " + ofToString(varSum) + "  -  ";
    //cout << "Number of Blobs: " + ofToString(finder.nBlobs) << endl;
}

//--------------------------------------------------------------
/* This function calculates the variance over an image */
float ofApp::calcVariance(){

    /* Get pixels from the scene fbo */
    ofPixels tempPix;
    scene.readToPixels(tempPix);
    float n = 0;
    float sum = 0;
    float sumSqrt = 0;

    /* Loop over all the pixels and get the brightness of each and find the average over the image */
    for(int i=0; i<scene.getWidth(); i++){
        for(int j=0; j<scene.getHeight(); j++){
            float x = tempPix.getColor(i, j).getBrightness();
            n = n + 1;
            sum = sum + x;
            sumSqrt = sumSqrt + x*x;
        }
    }

    /* Calculate the mean and variance */
    float mean = sum/n;
    float variance = (sumSqrt - sum*mean)/(n - 1);

    /* If the variance is less than zero, make it zero */
    if(variance < 0)
    {
        variance = 0;
    }

    return variance;
}

//--------------------------------------------------------------
float ofApp::calcBlobSepDistance(){
    /* Calculate the total distance between blobs on the image */
    float distSum = 0;

    /* Loop over all the blobs detected */
    for (int i = 0; i < finder.nBlobs; i++){
        ofxCvBlob blob = finder.blobs[i];

        /* Sum the distance between every blob */
        for(int j=0; j<finder.nBlobs; j++){
            if(i != j)
            {
                ofxCvBlob otherBlob = finder.blobs[j];
                float d = ofDist(blob.centroid.x, blob.centroid.y, otherBlob.centroid.x, otherBlob.centroid.y);
                distSum += d;
            }
        }
    }

    /* If no blobs are found then return an area and distSum of zero */
    if(finder.nBlobs == 0 || finder.nBlobs == 1)
    {
        distSum = 0;
    }
    else
    {
        /* Divide by number of blobs to get an average */
        distSum /= finder.nBlobs;
    }

    return distSum;
}

//--------------------------------------------------------------
/* This function loads my shaders */
void ofApp::loadShaders(int _index){

    /* Create initial positions */
    vector<float> pos(numParticles*3);
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;

            float xx = ofMap(x, 0, textureRes, -PI, PI);
            float yy = ofMap(y, 0, textureRes, -PI, PI);

            /* Offset positions */
            pos[i*3 + 0] = xx + gausRand.getNormal(0.002, 0.0);
            pos[i*3 + 1] = yy + gausRand.getNormal(0.002, 0.0);
        }
    }

    /* Load data into FBO */
    posPingPong.clear();
    posPingPong.src->getTexture().loadData(pos.data(), textureRes, textureRes, GL_RGB);
    posPingPong.dst->getTexture().loadData(pos.data(), textureRes, textureRes, GL_RGB);

    /* Clear the scene fbo */
    scene.begin();
    ofClear(255);
    scene.end();

    /* These strings create my shader program and allow me to write my shader programs within my program */
    /* Vertex Shader */
    string shaderHeader = "#version 330\n";
    string vertShader = shaderHeader;
           vertShader += STRINGIFY(
                                uniform mat4 modelViewProjectionMatrix;
                                in vec4 position;

                                void main() {
                                    gl_Position = modelViewProjectionMatrix * position;
                                }
                            );

    /* Fragment Shader */
    string path = "/func.glsl/";
    string shaderInclude = "#pragma include " + path + " \n";
    string fragShader = shaderHeader;
           fragShader += shaderInclude;
           fragShader += STRINGIFY(
                                precision highp float;
                                out vec4 outputColor;
                                uniform float time;

                                void main() {
                                    vec2 pos = gl_FragCoord.xy / vec2(1280.0,720.0);
                                    pos *= 2.0;
                                    pos -= 1.0;
                                );

            /* Get custom shader code from the current individual */
            string drawString = myPopulation.getIndividualPheno(_index);
            fragShader += drawString;

            fragShader += STRINGIFY(
                                outputColor = vec4(vec3(pos, 1.0), 1.0);
                                }
                            );


    /* Load these shaders */
    posShader.setupShaderFromSource(GL_VERTEX_SHADER, vertShader);
    posShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    posShader.linkProgram();

    /* Load the render shaders, from files */
    renderShader.load("render.vert", "render.frag");
}

