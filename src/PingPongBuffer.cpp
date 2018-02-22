#include "PingPongBuffer.h"

//--------------------------------------------------------------
PingPongBuffer::PingPongBuffer(){
}

//--------------------------------------------------------------
/* Allocate space */
void PingPongBuffer::allocate(int _width, int _height, int _internalformat){
    // Allocate fbo space
    for(int i = 0; i < 2; i++){
        FBOs[i].allocate(_width,_height, _internalformat );
        FBOs[i].getTexture().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
    }

    // Clean the jun
    clear();
}

//--------------------------------------------------------------
/* Swap the buffers */
void PingPongBuffer::swap(){
    std::swap(src,dst);
}

//--------------------------------------------------------------
/* Clear the fbo */
void PingPongBuffer::clear(){
    for(int i = 0; i < 2; i++){
        FBOs[i].begin();
        ofClear(0,255);
        FBOs[i].end();
    }
}
