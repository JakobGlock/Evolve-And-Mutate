#ifndef PINGPONGBUFFER_H
#define PINGPONGBUFFER_H

#include "ofMain.h"

/* Adapted from the GPU Particle system Example */
class PingPongBuffer{

public:
    /* Methods */
    PingPongBuffer();
    void allocate(int _width, int _height, int _internalformat = GL_RGBA);
    void swap();
    void clear();

    /* OF */
    ofFbo& operator[]( int n ){ return FBOs[n];}
    ofFbo *src = &FBOs[0]; // Source       ->  Ping
    ofFbo *dst = &FBOs[1]; // Destination  ->  Pong

private:
    ofFbo FBOs[2]; //PingPong
};

#endif // PINGPONGBUFFER_H
