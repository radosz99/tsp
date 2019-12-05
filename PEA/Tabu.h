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

#ifndef TABU_H
#define TABU_H

class Tabu {
public:
	//Default constructor
	Tabu();

	//Accessor Function
	void setSettingsTabu(int a, unsigned b, int c, int d, int e, bool f, int g);
	int getInitialReduction();
	int getInitialGreedy(vector < unsigned >&route);
	int getInitialGreedyAndRandom(vector < unsigned >&route);
	int getInitialReductionAndRandom(vector < unsigned >&route);
	void TabuMechanism(int matrixSize, int **TSPMatrix);
	void saveToFileTabu();
	void copyMatrix(int **macierz);
	void displayRoute();
	void cleanTabuList();
	int getBestNeighborhoodInsert(vector <unsigned>&currentTabu);
	int getBestNeighborhoodReverse(vector <unsigned>&currentTabu);
	int getBestNeighborhoodSwap(vector <unsigned>&currentTabu);

	void setOptMin(int a) {
		optMin = a;
	}

	int getOptMin() {
		return optMin;
	}
	/*
		void setTempMin(int a) {
			tempMin = a;
		}

		int getTempMin() {
			return tempMin;
		}

		int getCadence() {
			return cadence;
		}

		void setCadence(int a) {
			cadence = a;
		}

	*/

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
	bool randomAlgorithmType;
	int neighborhoodType;
	vector < unsigned >bestRoute;
	vector < unsigned > currentRoute;
	int **matrix;
	int matrixSize;
	vector< vector<unsigned> > tabuList;

};
#endif
