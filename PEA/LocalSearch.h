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
	friend class Genetic;
public:
	//Default constructor
	LocalSearch();

	//Accessor Function
	void setSettingsTabu(int a, unsigned b, int c, int d, int e, bool f, int g);
	void setSettingSA(double a, double b, int c, double d, bool e, int f);
	void TabuMechanism(int matrixSize, int **TSPMatrix);
	void saveToFileTabu(string name);
	void saveToFileSA(string name);
	void SimulatedAnnealingMechanism(int a, int **TSPMatrix);



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
	int **matrix;
	int matrixSize;
	vector< vector<unsigned> > tabuList;
	double initialTemperature;
	double cooling;
	double minTemperature;
	int iterations;
	double time;

	int calculateCost(vector <unsigned> a);
	int getInitialReduction(vector <unsigned>& route);
	int getInitialGreedy(vector < unsigned >&route);
	int getInitialGreedyAndRandom(vector < unsigned >&route);
	int getInitialReductionAndRandom(vector < unsigned >&route);
	void copyMatrix(int **macierz);
	void displayRoute(vector <unsigned> a);
	void cleanTabuList();
	int getBestNeighborhoodInsert(int &bestI, int &bestJ, vector <unsigned> currentRoute);
	int getBestNeighborhoodReverse(int &bestI, int &bestJ, vector <unsigned> currentRoute);
	int getBestNeighborhoodSwap(int &bestI, int &bestJ, vector <unsigned> currentRoute);
	void reverseVector(int a, int b, vector <unsigned>& currentRoute);
	void insertVector(int a, int b, vector <unsigned>& currentRoute);
	void swapVector(int a, int b, vector <unsigned>& currentRoute);
	int reshufflePath(vector <unsigned>& shuffled, vector <unsigned>currentRoute);
	int calculateProbability(int newCost, int oldCost, double temperature);
	void clearParameters(vector <unsigned> currentRoute);
	void calculateInsert(int i, int j, int &balance, vector <unsigned> currentRoute);
	void calculateReverse(int i, int j, int &balance, vector <unsigned> currentRoute);
	void calculateSwap(int i, int j, int &balance, vector <unsigned> currentRoute);
	int getInitialRandom(vector < unsigned >&bestTab);

};
#endif
