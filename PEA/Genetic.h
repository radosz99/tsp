#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <float.h>
#include <bits.h> 
#include <string>
#include <conio.h>
#include "menu.h"
#include "Czas.h"
#include "Node.h"
#include "LocalSearch.h"
#include <vector>

#ifndef GENETIC_H
#define GENETIC_H

class Genetic : public LocalSearch{

public:
	//Default constructor
	Genetic();
	int costXY(int a, int b);
	void memeticImprovement(vector <unsigned>& ind);
	void setSettingsGenetic(int a, int  b, double c, int d, int e, int f, int g, int h, int i);
	void setSettingsGenetic2(int a, int  b);
	void generateInitialPopulation(vector <vector <unsigned>>& pop, vector <double>&fitnesses, Czas onboardClock);
	int GeneticMechanism(int a, int **TSPMatrix, vector<unsigned>& islandsBest);
	void launchIslands(int a, int **TSPMatrix, vector < vector<unsigned>>& best, vector<unsigned>& islandsBest);
	void sortVector(vector <vector<unsigned>>&vect);
	void evaluatePopulation(vector <vector <unsigned>> popul, vector <double> &fitnesses);
	void overwritePopulation(vector <vector<unsigned>>&population, vector <vector<unsigned>>popul);
	void GeneticEngine(int a, int **TSPMatrix,int islandId, vector < vector<unsigned>>& best, vector<unsigned>& islandBest);
	void mutation(vector <unsigned>& ind);
	void islandExchange(vector <vector<unsigned>>&population, vector < vector<unsigned>> best, int islandId);
	void doCrossover(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>&offspring1, vector <unsigned>&offspring2);
	void doSelection(vector <unsigned>& parent1, vector <unsigned>& parent2, vector <vector <unsigned>> population, vector <double>& fitnesses);
	void displayBestRoute();
	void PartiallyMappedCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2);
	void OrderCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2);
	void CycleCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2);
	void CycleCO2(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2);
	void TwoPointCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2);
	void SequentialCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring);
	void EnhancedSequentialCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring);
	vector <unsigned> tournamentSelection(vector <vector <unsigned>> pop);
	int rouletteWheelSelection(vector <double> fit);
	int rankSelection(vector <double> fitnesses);

#pragma once

private:
	//Member variables
	int optMin;
	int currentOptMin;
	int tabuCadence;
	int currentTabuCadence;
	unsigned tabuTime;
	int iterationsLimit;
	int intensificationDiv;
	int amountRandomNodes;
	bool algorithmType;
	int neighborhoodType;
	vector < unsigned >bestRoute;
	vector < unsigned > currentRoute;
	int **matrix;
	int matrixSize;
	vector< vector<unsigned> > tabuList;
	double initialTemperature;
	double cooling;
	double minTemperature;
	int timeGenetic;
	int iterations;
	double time;
	int populationSize;
	double mutationProb;
	int crossoverType;
	int selectionType;
	int mutationType;
	int elitismNumber;
	int memeticType;
};
#endif
