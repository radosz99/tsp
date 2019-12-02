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
#include "BruteForceFunc.h"
#include "Node.h"
#include <vector>
#include "BBFunc.h"
#include "Tabu.h"
#include <ctime>
#include <random>

using namespace std;

void getInitialReduction(vector < unsigned >&bestTab, int &helpMin, Node start) {
	int size = start.getStartSize();

	int **macierz = new int *[size]; //macierz do operacji
	int **mainMacierz = new int *[size]; //ta sama macierz, ale do odtwarzania
	for (int i = 0; i < size; i++) {
		macierz[i] = new int[size];
		mainMacierz[i] = new int[size];
	}
		start.copyMatrix(macierz);
		//reduceMatrix(macierz, size);
		int bestMin = 0, tempMin = 0, savedBestCol = 0;
		helpMin = 0;
		int *routeTab = new int[size];
		int *visitedTab = new int[size];
		for (int i = 0; i < size; i++) {
			visitedTab[i] = 0;
			routeTab[i] = 0;
		}

		for (int i = 0; i < size - 1; i++) {
			bestMin = INT_MAX;
			if (i == 0) {
				for (int k = 0; k < size; k++)
					for (int l = 0; l < size; l++)
						mainMacierz[k][l] = macierz[k][l];
			}
			for (int j = 1; j < size; j++) {
				if (visitedTab[j] != -1) {
					suitableRowColToInf(macierz, routeTab[i], j, size); //ustawienie wartosci w odpowiednim wierszu i kolumnie na -1
					tempMin = reduceMatrix(macierz, size); //zredukowanie macierzy
					tempMin += mainMacierz[routeTab[i]][j]; //dodanie przejscia

					/*cout << "------------TEMPMIN----" << tempMin << "------" << endl;
					cout << "------------BESTMIN----" << bestMin << "------" << endl;
					for (int i = 0; i < size; i++) {
						for (int j = 0; j < size; j++) {
							cout << mainMacierz[i][j] << " ";
						}
						cout << endl;
					}
					cout << endl;
					cout << "----------------------------------------------" << endl;*/

					if (tempMin < bestMin) { //jako ze wszyscy synowie maja ten sam lower bound ojca to nie jest tu uwzgledniany
						bestMin = tempMin; //najlepszy lower bound (bez uwzglednienia lower bound ojca) jest zapisywany do bestMin
						savedBestCol = j; //id wierzcholka z bestMin jest zapisywane do savedBestCol
					}

					for (int k = 0; k < size; k++)
						for (int l = 0; l < size; l++)
							macierz[k][l] = mainMacierz[k][l];


				}
			}
			visitedTab[savedBestCol] = -1; //w tabeli wizyt oznaczamy wierzcholek o id savedBestCol na odwiedzony
			routeTab[i + 1] = savedBestCol; //w tabeli drogi komorka o nastepnym indeksie ma id wierzcholka


			helpMin = bestMin + helpMin; // ustalenie lower bound
			suitableRowColToInf(macierz, routeTab[i], savedBestCol, size);
			reduceMatrix(macierz, size);
			for (int k = 0; k < size; k++)
				for (int l = 0; l < size; l++)
					mainMacierz[k][l] = macierz[k][l];

		}
		for (int i = 0; i < size; i++)
			bestTab.push_back(routeTab[i]);

		bestTab.push_back(0);
		delete[]routeTab;
		delete[]visitedTab;

		for (int i = 0; i < size; i++) {
			delete[]macierz[i];
			delete[]mainMacierz[i];
		}
		delete[]macierz;
		delete[]mainMacierz;
}

int getInitialReductionAndRandom(vector < unsigned >&bestTab, int &helpMin, Node start, int divider) {
	int size = start.getStartSize();
	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> nodesRand(1, size / divider);
	uniform_int_distribution<> nodeRand(1, size - 1);
	helpMin = 0;
	//int randomNodes = nodesRand(randomGen);
	int randomNodes = divider;
	//cout << "--------------------" << endl;
	//cout << "random nodes :" << randomNodes << endl;
	int randomNode;
	int **macierz = new int *[size]; //macierz do operacji
	int **mainMacierz = new int *[size]; //ta sama macierz, ale do odtwarzania
	for (int i = 0; i < size; i++) {
		macierz[i] = new int[size];
		mainMacierz[i] = new int[size];
	}
	start.copyMatrix(macierz);
	bool ifVisited;
	int bestMin = 0, tempMin = 0, savedBestCol = 0;
	helpMin = 0;
	int *routeTab = new int[size];
	int *visitedTab = new int[size];
	for (int i = 0; i < size; i++) {
		visitedTab[i] = 0;
		routeTab[i] = 0;
	}

	for (int i = 0; i < randomNodes; i++) {
		ifVisited = false;
		while (ifVisited == false) {
			randomNode = nodeRand(randomGen);
			ifVisited = true;
			if (visitedTab[randomNode] == -1) {
					ifVisited = false;
			}
		}
		//cout << i << " node: " << randomNode << endl;
		visitedTab[randomNode] = -1;
		routeTab[i + 1] = randomNode;
		helpMin = helpMin + macierz[routeTab[i]][randomNode];
		suitableRowColToInf(macierz, routeTab[i], randomNode, size);


	}

	for (int i = randomNodes; i < size - 1; i++) {
		bestMin = INT_MAX;
		if (i == randomNodes) {
			for (int k = 0; k < size; k++)
				for (int l = 0; l < size; l++)
					mainMacierz[k][l] = macierz[k][l];
		}
		for (int j = 1; j < size; j++) {
			if (visitedTab[j] != -1) {
				suitableRowColToInf(macierz, routeTab[i], j, size); //ustawienie wartosci w odpowiednim wierszu i kolumnie na -1
				tempMin = reduceMatrix(macierz, size); //zredukowanie macierzy
				tempMin += mainMacierz[routeTab[i]][j]; //dodanie przejscia

				/*cout << "------------TEMPMIN----" << tempMin << "------" << endl;
				cout << "------------BESTMIN----" << bestMin << "------" << endl;
				for (int i = 0; i < size; i++) {
					for (int j = 0; j < size; j++) {
						cout << mainMacierz[i][j] << " ";
					}
					cout << endl;
				}
				cout << endl;
				cout << "----------------------------------------------" << endl;*/

				if (tempMin < bestMin) { //jako ze wszyscy synowie maja ten sam lower bound ojca to nie jest tu uwzgledniany
					bestMin = tempMin; //najlepszy lower bound (bez uwzglednienia lower bound ojca) jest zapisywany do bestMin
					savedBestCol = j; //id wierzcholka z bestMin jest zapisywane do savedBestCol
				}

				for (int k = 0; k < size; k++)
					for (int l = 0; l < size; l++)
						macierz[k][l] = mainMacierz[k][l];


			}
		}
		visitedTab[savedBestCol] = -1; //w tabeli wizyt oznaczamy wierzcholek o id savedBestCol na odwiedzony
		routeTab[i + 1] = savedBestCol; //w tabeli drogi komorka o nastepnym indeksie ma id wierzcholka


		helpMin = bestMin + helpMin; // ustalenie lower bound
		suitableRowColToInf(macierz, routeTab[i], savedBestCol, size);
		reduceMatrix(macierz, size);
		for (int k = 0; k < size; k++)
			for (int l = 0; l < size; l++)
				mainMacierz[k][l] = macierz[k][l];

	}

	start.copyMatrix(macierz);

	//for (int l = 0; l < size; l++)
	//	cout << routeTab[l] << "-";
	//cout << endl;

	//cout << " Min " << helpMin;
	helpMin = calculate(routeTab, size, macierz);
	//cout << " i po kalkulacji " << helpMin << endl;
	//cout << "--------------------" << endl;
	for (int i = 0; i < size; i++)
		bestTab.push_back(routeTab[i]);

	bestTab.push_back(0);
	delete[]routeTab;
	delete[]visitedTab;

	for (int i = 0; i < size; i++) {
		delete[]macierz[i];
		delete[]mainMacierz[i];
	}
	delete[]macierz;
	delete[]mainMacierz;

	return randomNodes;
}

void getInitialGreedy(vector < unsigned >&bestTab, int &helpMin, Node start) {

	int size = start.getStartSize();
	int bestMin, tempBest = 0, oldTempBest = 0;
	helpMin = 0;
	int *visitedTab = new int[size];
	for (int i = 0; i < size; i++) {
		visitedTab[i] = 0;
	}
	int **macierz = new int *[size];
	for (int i = 0; i < size; i++) {
		macierz[i] = new int[size];
	}

	start.copyMatrix(macierz);
	bool ifVisited;
	for (int i = 0; i < size; i++) {
		bestMin = INT_MAX;
		oldTempBest = tempBest;
		for (int j = 0; j < size; j++) {
			ifVisited = true;
			if (j != oldTempBest) {
				for (int k = 0; k <= i; k++) {
					if (j == visitedTab[k]) {
						ifVisited = false;
					}
				}
				if (macierz[oldTempBest][j] < bestMin && ifVisited==true) {
					bestMin = macierz[oldTempBest][j];
					tempBest = j;
				}
			}
		}
		if (i < size - 1) 
			helpMin = helpMin + bestMin;
		else
			helpMin = helpMin + macierz[oldTempBest][0];

		bestTab.push_back(oldTempBest);
		visitedTab[i] = tempBest;
	}
	bestTab.push_back(0);

	for (int i = 0; i < size; i++) {
		delete[]macierz[i];
	}
	delete[]macierz;
	delete[]visitedTab;

}

void getInitialGreedyAndRandom(vector < unsigned >&bestTab, int &helpMin, Node start, int divider) {

	int size = start.getStartSize();
	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> nodesRand(1, size / divider);
	uniform_int_distribution<> nodeRand(0, size - 1);

	int bestMin, tempBest = 0, oldTempBest = 0;
	helpMin = 0;
	int *visitedTab = new int[size];
	for (int i = 0; i < size; i++) {
		visitedTab[i] = 0;
	}
	int **macierz = new int *[size];
	for (int i = 0; i < size; i++) {
		macierz[i] = new int[size];
	}

	start.copyMatrix(macierz);
	bool ifVisited;

	//int randomNodes = nodesRand(randomGen);
	//cout << "Random nodes: " << randomNodes << endl;
	int randomNodes = divider;

	int randomNode;

	for (int i = 0; i < size; i++) {
		bestMin = INT_MAX;
		oldTempBest = tempBest;
		if (randomNodes != 0) {
			ifVisited = false;
			while (ifVisited == false) {
				randomNode = nodeRand(randomGen);
				ifVisited = true;
				for (int k = 0; k <= i; k++) {
					if (randomNode == visitedTab[k]) {
						ifVisited = false;
					}
				}
			}
			//cout << "Random Node: " << randomNode << endl;
			tempBest = randomNode;
			bestMin = macierz[oldTempBest][randomNode];
			randomNodes--;

		}
		else {
			for (int j = 0; j < size; j++) {
				ifVisited = true;
				if (j != oldTempBest) {
					for (int k = 0; k <= i; k++) {
						if (j == visitedTab[k]) {
							ifVisited = false;
						}
					}
					if (macierz[oldTempBest][j] < bestMin && ifVisited == true) {
						bestMin = macierz[oldTempBest][j];
						tempBest = j;
					}
				}
			}
		}
		if (i < size - 1)
			helpMin = helpMin + bestMin;
		else
			helpMin = helpMin + macierz[oldTempBest][0];

		bestTab.push_back(oldTempBest);
		visitedTab[i] = tempBest;
	}
	bestTab.push_back(0);
	//cout << "Greedy min: " << helpMin << endl;
	for (int i = 0; i < size; i++) {
		delete[]macierz[i];
	}
	delete[]macierz;


}

int Tabu(Node start, int matrixSize, int **TSPMatrix, int c, unsigned d, int e, int f, int g,bool alg) {
	//cout << "TABU SEARCH" << endl;
	vector < unsigned > bestRoute;
	vector < unsigned > bestRouteGreedy;
	int min = 0;

	//int tabuIterationsToRestart = 10000;
	int tabuIterationsToRestart = e;
	//unsigned tabuStopTime = 30;
	unsigned tabuStopTime = d;
	int greedyStart = 1;
	getInitialReduction(bestRoute, min, start);

	/*
	cout << "\nREDUKCJA Minimalna funkcja celu = " << min;
	cout << "\nREDUKCJA Najlepsza droga: ";
	for (int i = 0; i <= matrixSize; i++)
		cout << bestRoute.at(i) << " ";
	cout << endl << endl;
	*/

	vector < unsigned > currentRoute = bestRoute;
	vector< vector<unsigned> > tabuList;

	vector < unsigned > routeToDisplay;
	int routeToDisplayFunc;
	int randomNodes;
	bool continuing = true;
	//int defaultCadence = 16;
	int defaultCadence = c;
	int cadence = defaultCadence;
	int balance = 0, bestBalance = INT_MAX;
	int actualMin = min;
	int counter = 0;
	int bestI = 0;
	int bestJ = 0;
	bool intensification;
	bool ifTabu;
	int iterWithoutImprovement=0;
	int iterWithoutImprovementRand = 0;
	Czas onboardClock;
	onboardClock.start();
	bool diversification = true;
	//cout << "JESTEM TU" << endl;
	while (continuing == true) {
		intensification = false;
		bestBalance = INT_MAX;
		vector<unsigned> currentTabu(3, 0);

		for (int i = 1; i < matrixSize - 1; i++) {
			for (int j = i + 1; j < matrixSize; j++) {
				currentTabu.at(2) = cadence;

				//cout << "ZAMIENIAM " << i << " Z " << j << endl;

				if (i + 1 == j) {
					balance = 0 - TSPMatrix[currentRoute.at(i - 1)][currentRoute.at(i)];
					balance = balance - TSPMatrix[currentRoute.at(i)][currentRoute.at(j)];
					balance = balance - TSPMatrix[currentRoute.at(j)][currentRoute.at(j + 1)];
					balance = balance + TSPMatrix[currentRoute.at(i - 1)][currentRoute.at(j)];
					balance = balance + TSPMatrix[currentRoute.at(j)][currentRoute.at(i)];
					balance = balance + TSPMatrix[currentRoute.at(i)][currentRoute.at(j + 1)];
					//cout << "BILANS = " << balance << endl;
					//cout << "WYNIK = " << min+balance << endl;
				}
				else {
					balance = 0 - TSPMatrix[currentRoute.at(i - 1)][currentRoute.at(i)];
					balance = balance - TSPMatrix[currentRoute.at(i)][currentRoute.at(i + 1)];
					balance = balance - TSPMatrix[currentRoute.at(j - 1)][currentRoute.at(j)];
					balance = balance - TSPMatrix[currentRoute.at(j)][currentRoute.at(j + 1)];
					balance = balance + TSPMatrix[currentRoute.at(i - 1)][currentRoute.at(j)];
					balance = balance + TSPMatrix[currentRoute.at(j)][currentRoute.at(i + 1)];
					balance = balance + TSPMatrix[currentRoute.at(j - 1)][currentRoute.at(i)];
					balance = balance + TSPMatrix[currentRoute.at(i)][currentRoute.at(j + 1)];
					//cout << "BILANS = " << balance << endl;
					//cout << "WYNIK = " << min + balance << endl;
				}


				ifTabu = false;

				for (int k = 0; k < tabuList.size(); k++)
				{
					if (tabuList.at(k).at(0) == currentRoute.at(i) && tabuList.at(k).at(1) == currentRoute.at(j))
					{
						ifTabu = true;
						break;
					}
					if (tabuList.at(k).at(0) == currentRoute.at(j) && tabuList.at(k).at(1) == currentRoute.at(i))
					{
						ifTabu = true;
						break;
					}
				}

				// Kryterium aspiracji...
				if (ifTabu == true && actualMin + balance >= min)
					// ...jezeli niespelnione - pomijamy ruch
					continue;

				if (balance < bestBalance) {
					bestBalance = balance;
					currentTabu.at(0) = currentRoute[i];
					currentTabu.at(1) = currentRoute[j];
					bestI = i;
					bestJ = j;
				}

			}
		}
		//cout << "Najlepszy bilans = " << bestBalance << ", to swap miedzy " << currentTabu.at(0) << " i " << currentTabu.at(1) << endl;
		actualMin = actualMin + bestBalance;

		unsigned buffer = currentRoute.at(bestJ);
		currentRoute.at(bestJ) = currentRoute.at(bestI);
		currentRoute.at(bestI) = buffer;


		if (actualMin < min) {
			if (actualMin == 1286) {
				/*
				cout << "AKTUALNA LISTA TABU:" << endl;
				for (int k = 0; k < tabuList.size(); k++)
				{
					cout << "Kadencja " << tabuList.at(k).at(2) << " swap " << tabuList.at(k).at(0) << " i " << tabuList.at(k).at(1) << endl;
				}
				cout << "Iteration no. " << iterWithoutImprovementRand << endl;
				cout << "routeToDisplayFunc " << routeToDisplayFunc << endl;
				for (int y = 0; y < matrixSize; y++)
					cout << routeToDisplay.at(y) << "-";
				cout << endl;
				*/
	
			}
			iterWithoutImprovement = 0;
			intensification = true;
			min = actualMin;
			bestRoute = currentRoute;

		}


		for (int i = 0; i < tabuList.size(); i++)
		{
			tabuList.at(i).at(2)--;
		}

		for (int i = 0; i < tabuList.size(); i++)
		{
			if (tabuList.at(i).at(2) == 0) {
				tabuList.erase(tabuList.begin() + i);
				i = i - 1;
			}
		}

		tabuList.push_back(currentTabu);

		/*
		cout << "AKTUALNA LISTA TABU:" << endl;
		for (int k = 0; k < tabuList.size(); k++)
		{
			cout << "Kadencja " << tabuList.at(k).at(2) << " swap " << tabuList.at(k).at(0) << " i " << tabuList.at(k).at(1) << endl;
		}


		cout << "\nNajlepsza droga: ";
		for (int i = 0; i <= matrixSize; i++)
			cout << currentRoute.at(i) << " ";
		cout << endl;
		cout << "Min = " << actualMin << endl;
		*/

		counter++;
		iterWithoutImprovement++;
		iterWithoutImprovementRand++;

		onboardClock.stop();
		if (onboardClock.read() > tabuStopTime)
			continuing = false;

		if (intensification == true)
		{
			// Intensyfikacja przeszukiwania przez skrócenie kadencji
			// (jezeli w ostatnim przebiegu znaleziono nowe minimum)
			//cadence = defaultCadence / 2;
			cadence = defaultCadence / f;
			//cout << "Obecna " << cadence << endl;
			intensification = false;
		}

		if (iterWithoutImprovement > tabuIterationsToRestart) {
			if (diversification == true)
			{
				// W innym przypadku wlasciwa dywersyfikacja przez wygenerowanie nowego
				// rozwiazania startowego algorytmem hybrydowym losowo-zachlannym
				vector < unsigned > bestRouteGreedy;
				//cout << "ponowienie" << endl;
				actualMin = 0;
				if (greedyStart != 0) {
					getInitialGreedy(bestRouteGreedy, actualMin, start);
					greedyStart--;
				}
				else {
					if (alg==true)
					randomNodes=getInitialReductionAndRandom(bestRouteGreedy, actualMin, start, g);
					else
					getInitialGreedyAndRandom(bestRouteGreedy, actualMin, start, g);
				}
				cadence = defaultCadence;
				iterWithoutImprovement = 0;
				currentRoute = bestRouteGreedy;
				routeToDisplay = bestRouteGreedy;
				iterWithoutImprovementRand = 0;
				routeToDisplayFunc = actualMin;

				//cout << "SWAP SIZE " << swaps.size() << endl;
				/*
				cout << "\nGREEDY droga: ";
				for (int i = 0; i <= matrixSize; i++)
					cout << bestRouteGreedy.at(i) << " ";
				cout << endl;

				cout << "\nGREEDY BEST Min = " << actualMin << endl;
				cout << endl << endl;
				*/
			}
		}
	}
	/*
	cout << "Iteracji: " << counter;
	cout << "\nBEST droga: ";
	for (int i = 0; i <= matrixSize; i++)
		cout << bestRoute.at(i) << " ";
	cout << endl;
	cout << "BEST Min = " << min << endl;
	cout << endl << endl;
	*/
	return min;
}