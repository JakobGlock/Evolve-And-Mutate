#ifndef POPULATION_H
#define POPULATION_H

#include "ofMain.h"
#include "Individual.h"

class Population{
public:
    /* Methods */
    Population();
    void createPopulation(float _popSize);
    void poolSelection();
    void newPopulation();
    string getIndividualPheno(int _index);
    int getGeneration();
    int getPopulationSize();

    /* Variables */
    vector<Individual *> myPopulation;
    int parentA, parentB, populationSize;
    int generation;
    float fitnessSum;
};

#endif // POPULATION_H
