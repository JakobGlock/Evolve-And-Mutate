#include "Individual.h"

//--------------------------------------------------------------
Individual::Individual(){
    mutationRate = 0.03; // 3% mutation
    fitness = 0;

    numCats = 8;
    depth = (int)ofRandom(2, 32) * numCats;

    /* Setup dictionary */
    dictionary.push_back("sinusoidal(pos,€,#)");
    dictionary.push_back("hyperbolic(pos,€,#)");
    dictionary.push_back("julia(pos,€,#)");
    dictionary.push_back("swirl(pos,€,#)");
    dictionary.push_back("sech(pos*3.0,€,#)");
    dictionary.push_back("hank(pos,€,#)");
    dictionary.push_back("eyeFish(pos,€,#)");
    dictionary.push_back("pdj(pos,€,#)");
    dictionary.push_back("spherical(pos,€,#)");
    dictionary.push_back("spiral(pos,€,#)");
    dictionary.push_back("exponential(pos,€,#)");
    dictionary.push_back("cross(pos,€,#)");
//    dictionary.push_back("heart(pos, €,#)");
//    dictionary.push_back("disc(pos, €,#)");
}

//--------------------------------------------------------------
/* This function combines half of one individual with another */
void Individual::crossover(vector<float> _otherGeno_a, vector<float> _otherGeno_b){

    /* Clear the geno and pheno */
    geno.clear();
    pheno.clear();

    /* Calculate the split point (Aim for half), but make sure its a multiple of 8 */
    int splitA = (((_otherGeno_a.size() / 8) / 2) * 8);

    /* Add the first half */
    for(int i=0; i<splitA; i++){
        geno.push_back(_otherGeno_a[i]);
    }

    /* Calculate the split point (Aim for half), but make sure its a multiple of 8 */
    int splitB = (((_otherGeno_b.size() / 8) / 2) * 8);

    /* Add the second half */
    for(int i=splitB; i<_otherGeno_b.size(); i++){
        geno.push_back(_otherGeno_b[i]);
    }

    /* Do mutation */
    mutate();

    /* Turn the geno into a string */
    pheno = genoToString();
}

//--------------------------------------------------------------
/* This function creates a random DNA for the individual */
void Individual::createRandomGeno(){

    geno.clear();

    for(int i=0; i<depth; i++){
        geno.push_back(ofRandom(0, 1));
    }

    pheno = genoToString();
}

//--------------------------------------------------------------
/* This function mutates the DNA */
void Individual::mutate(){
    for(int i=0; i<geno.size(); i++){
        if(ofRandom(0, 1) < mutationRate)
        {
            geno[i] = ofRandom(0, 1);
        }
    }
}

//--------------------------------------------------------------
/* This function turns the DNA (values from 0-1) into a strings that makes up my shader */
string Individual::genoToString(){

    /* A string to store my GLSL code */
    string newString;

    /* Loop over the geno in steps equal to how many catagories there are */
    for(int i=0; i<geno.size(); i+=numCats){
        string tempString;

        int addForLoop = (int)ofMap(geno[i], 0, 1, 0, 6);

        if(addForLoop == 1)
        {
            tempString += "for(int i=0; i<#; i++) ";
            string forDepth = ofToString((int)ofMap(geno[i+1], 0, 1, 2, 10));
            ofStringReplace(tempString,"#", forDepth);
        }

        int funcIndex = (int)ofMap(geno[i+2], 0, 1, 0, dictionary.size());
        tempString += "pos = " + dictionary[funcIndex];

        string forAmt = ofToString(ofMap(geno[i+3], 0, 1, 0.25, 1));
        ofStringReplace(tempString,"€", forAmt);

        int addNextFunc = (int)ofMap(geno[i+4], 0, 1, 0, 4);
        if(addNextFunc == 1)
        {
            int nextFuncIndex = (int)ofMap(geno[i+5], 0, 1, 0, dictionary.size());
            tempString += " + " + dictionary[nextFuncIndex] + ";\n";

            string forAmt = ofToString(ofMap(geno[i+6], 0, 1, 0.25, 1));
            ofStringReplace(tempString,"€", forAmt);
        }
        else
        {
            tempString += ";\n";
        }

        int addTime = (int)ofMap(geno[i+7], 0, 1, 0, 2);
        if(addTime == 1)
        {
            ofStringReplace(tempString,"#","time");
        }
        else
        {
            ofStringReplace(tempString,"#","0.0");
        }

        newString += tempString;
    }

    return newString;
}

//--------------------------------------------------------------
/* Returns the pheno vector */
vector<float> Individual::getGeno(){
    return geno;
}

//--------------------------------------------------------------
/* Return the fitness of the individual */
float Individual::getFitness(){
    return fitness;
}

//--------------------------------------------------------------
/* Set the fitness of the individual */
void Individual::setFitness(float _nBlobs, float _totalSep, float _variance){

    /* scalars and influencers for the fitness function */
    float a = 1.2;
    float b = 0.6;
    float c = 3.0;

    /* Combine to calculate the fitness function */
    float calcFitness = a * _nBlobs + b * _totalSep + c * _variance;

    /* Map the fitness to a value between 0 - 1 */
    fitness = pow(ofMap(calcFitness, 0, 50000, 0, 1, true), 2);
    fitness = ofClamp(fitness, 0.0001, 1);
}
