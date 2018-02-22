#include "Population.h"
/* This is my population class, it manages my population */

//--------------------------------------------------------------
Population::Population(){
    populationSize = 0;
    generation = 1;
    fitnessSum = 0;

    /* Print the generation number, for debugging */
    //cout << "Generation: " + ofToString(generation) << endl;
}

//--------------------------------------------------------------
/* This function creates a random population */
void Population::createPopulation(float _popSize){
    /* Set default population size */
    populationSize = _popSize;

    /* Create a random population */
    for(int i=0; i<populationSize; i++){
        myPopulation.push_back(new Individual());
        myPopulation[i]->createRandomGeno();
    }
}

//--------------------------------------------------------------
/* This function chooses two individuals to mate together */
void Population::poolSelection(){
    /* Pick a random number between, 0 and the total of all the fitness values */
    float getParentA = ofRandom(0, fitnessSum);
    float prevFitness = 0;

    /* Loop over the population */
    for(int i=0; i<myPopulation.size(); i++){
	/* Add the current fitness to the previous fitness value */
        float low = myPopulation[i]->getFitness() + prevFitness;

	/* If the value is between two values then select that parent */
        if(getParentA > prevFitness && getParentA < low)
        {
            parentA = i;
            parentB = i;
        }

        prevFitness = low;
    }

   /* While the two parents are the same pick a new one */
    while(parentB == parentA)
    {
        float getParentB = ofRandom(0, fitnessSum);
        prevFitness = 0;
        for(int i=0; i<myPopulation.size(); i++){
            float low = myPopulation[i]->getFitness() + prevFitness;
            if(getParentB > prevFitness && getParentB < low)
            {
                parentB = i;
            }
            prevFitness = low;
        }
    }

    /* Swap parents if parentA fitness is less than parentB fitness */
    if(myPopulation[parentA]->getFitness() < myPopulation[parentB]->getFitness())
    {
        int tempParent = parentA;
        parentA = parentB;
        parentB = tempParent;
    }
}

//--------------------------------------------------------------
/* This function creates a new population from the old population */
void Population::newPopulation(){

    /* Calculate the sum of all the fitness values */
    fitnessSum = 0;
    for(int i=0; i<myPopulation.size(); i++){
        fitnessSum += myPopulation[i]->getFitness();
    }

    /* Create a temporary vector for a new population */
    vector<Individual *> tempPopulation;

    /* Create new population, from two parents from the old population */
    for(int i=0; i<populationSize; i++){
        tempPopulation.push_back(new Individual());
        poolSelection();
        tempPopulation[i]->crossover(myPopulation[parentA]->getGeno(), myPopulation[parentB]->getGeno());
    }

    /* Clear the old population and swap in the new one */
    myPopulation.clear();

    for(int i=0; i<tempPopulation.size(); i++){
        myPopulation.push_back(tempPopulation[i]);
    }

    /* Increment the generation */
    generation++;

    /* Print the generation number, for debugging */
    //cout << "Generation: " + ofToString(generation) << endl;
}

//--------------------------------------------------------------
/* Returns an individuals DNA */
string Population::getIndividualPheno(int _index){
    return myPopulation[_index]->pheno;
}

//--------------------------------------------------------------
/* Returns the current generation */
int Population::getGeneration(){
    return generation;
}

//--------------------------------------------------------------
/* Returns the population size */
int Population::getPopulationSize(){
    return populationSize;
}
