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
#include <vector>

#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

class LocalSearch {
public:
	//Default constructor
	LocalSearch();

	//Accessor Function
	void setSettingsTabu(int a, unsigned b, int c, int d, int e, bool f, int g);
	void setSettingSA(double a, double b, int c, double d, bool e, int f);
	int calculateCost(vector <unsigned> a);
	int getInitialReduction();
	int getInitialGreedy(vector < unsigned >&route);
	int getInitialGreedyAndRandom(vector < unsigned >&route);
	int getInitialReductionAndRandom(vector < unsigned >&route);
	void TabuMechanism(int matrixSize, int **TSPMatrix);
	void saveToFileTabu(string name);
	void saveToFileSA(string name);
	void copyMatrix(int **macierz);
	void displayRoute(vector <unsigned> a);
	void cleanTabuList();
	int getBestNeighborhoodInsert(int &bestI, int &bestJ);
	int getBestNeighborhoodReverse(int &bestI, int &bestJ);
	int getBestNeighborhoodSwap(int &bestI, int &bestJ);
	void reverseVector(int a, int b);
	void insertVector(int a, int b);
	void swapVector(int a, int b);
	void SimulatedAnnealingMechanism(int a, int **TSPMatrix);
	int reshufflePath(vector <unsigned>& shuffled);
	int calculateProbability(int newCost, int oldCost, double temperature);
	void clearParameters();
	void calculateInsert(int i, int j, int &balance);
	void calculateReverse(int i, int j, int &balance);
	void calculateSwap(int i, int j, int &balance);

	void setOptMin(int a) {
		optMin = a;
	}

	int getOptMin() {
		return optMin;
	}


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
	int iterations;
	double time;

};
#endif
