#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "ofMain.h"

class Individual{
public:
    /* Methods */
    Individual();
    void crossover(vector<float> _otherGeno_a, vector<float> _otherGeno_b);
    void createRandomGeno();
    void setFitness(float _nBlobs, float _totalSep, float _variance);
    void mutate();
    vector<float> getGeno();
    string genoToString();
    float getFitness();

    /* Variables */
    vector<string> dictionary;
    vector<float> geno;
    string pheno;
    float mutationRate, fitness, timeStep;
    int depth, numCats;
};

#endif // INDIVIDUAL_H
