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
#include "BruteForce.h"
#include "Node.h"
#include <vector>
#include "BranchBound.h"
#include "LocalSearch.h"
#include <ctime>
#include <random>

using namespace std;


LocalSearch::LocalSearch() {
}

void LocalSearch::setSettingsTabu(int a, unsigned b, int c, int d, int e, bool f, int g) {
	tabuCadence = a;
	tabuTime = b;
	iterationsLimit = c;
	intensificationDiv = d;
	amountRandomNodes = e;
	algorithmType = f;
	neighborhoodType = g;
}

void LocalSearch::setSettingSA(double a, double b, int c, double d, bool e, int f) {
	initialTemperature = a;
	minTemperature = b;
	iterationsLimit = c;
	cooling = d;
	algorithmType = e;
	neighborhoodType = f;

}

int LocalSearch::calculateCost(vector <unsigned> a) {

	int sum = 0;
	int i, j;
	for (int iter = 0; iter < matrixSize; iter++) {
		i = a.at(iter);
		j = a.at(iter + 1);
		//cout << "I= " << i << " j = " << j << endl;
		sum += matrix[i][j];
		//cout << matrix[i][j] << " ";
	}
	cout << endl;
	return sum;
}


void LocalSearch::saveToFileTabu(string name) {
	ofstream plik;
	plik.open("C:/Users/Radek/source/repos/PEA/Output/wynikiTestyTabu.csv", std::ios_base::app);
	plik << name << ";" << tabuCadence << ";" << tabuTime << ";" << iterationsLimit << ";" << intensificationDiv << ";" << amountRandomNodes << ";" << algorithmType << ";" << neighborhoodType << ";" << optMin << endl;
	plik.close();
}

void LocalSearch::saveToFileSA(string name) {
	ofstream plik;
	plik.open("C:/Users/Radek/source/repos/PEA/Output/wynikiTestySA.csv", std::ios_base::app);
	plik << name << ";" << initialTemperature << ";" << minTemperature << ";" << iterationsLimit << ";" << cooling << ";" << algorithmType << ";" << neighborhoodType << ";" << iterations << ";" << optMin << ";" << time << endl;
	plik.close();
}

void LocalSearch::copyMatrix(int **macierz) {
	for (int i = 0; i < matrixSize; i++)
		for (int j = 0; j < matrixSize; j++)
			macierz[i][j] = matrix[i][j];
}

void LocalSearch::displayRoute(vector <unsigned> a) {
	for (int i = 0; i < a.size(); i++)
		cout << a.at(i) << "-";
	cout << endl;

}
int LocalSearch::getInitialReduction() {
	int localMin = 0;
	int **macierz = new int *[matrixSize]; //macierz do operacji
	int **mainMacierz = new int *[matrixSize]; //ta sama macierz, ale do odtwarzania
	for (int i = 0; i < matrixSize; i++) {
		macierz[i] = new int[matrixSize];
		mainMacierz[i] = new int[matrixSize];
	}
	copyMatrix(macierz);

	int bestMin = 0, tempMin = 0, savedBestCol = 0;
	int *routeTab = new int[matrixSize];
	int *visitedTab = new int[matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		visitedTab[i] = 0;
		routeTab[i] = 0;
	}

	for (int i = 0; i < matrixSize - 1; i++) {
		bestMin = INT_MAX;
		if (i == 0) {
			for (int k = 0; k < matrixSize; k++)
				for (int l = 0; l < matrixSize; l++)
					mainMacierz[k][l] = macierz[k][l];
		}
		for (int j = 1; j < matrixSize; j++) {
			if (visitedTab[j] != -1) {
				suitableRowColToInf(macierz, routeTab[i], j, matrixSize); //ustawienie wartosci w odpowiednim wierszu i kolumnie na -1
				tempMin = reduceMatrix(macierz, matrixSize); //zredukowanie macierzy
				tempMin += mainMacierz[routeTab[i]][j]; //dodanie przejscia

				if (tempMin < bestMin) { //jako ze wszyscy synowie maja ten sam lower bound ojca to nie jest tu uwzgledniany
					bestMin = tempMin; //najlepszy lower bound (bez uwzglednienia lower bound ojca) jest zapisywany do bestMin
					savedBestCol = j; //id wierzcholka z bestMin jest zapisywane do savedBestCol
				}

				for (int k = 0; k < matrixSize; k++)
					for (int l = 0; l < matrixSize; l++)
						macierz[k][l] = mainMacierz[k][l];


			}
		}
		visitedTab[savedBestCol] = -1; //w tabeli wizyt oznaczamy wierzcholek o id savedBestCol na odwiedzony
		routeTab[i + 1] = savedBestCol; //w tabeli drogi komorka o nastepnym indeksie ma id wierzcholka


		localMin = bestMin + localMin; // ustalenie lower bound
		suitableRowColToInf(macierz, routeTab[i], savedBestCol, matrixSize);
		reduceMatrix(macierz, matrixSize);
		for (int k = 0; k < matrixSize; k++)
			for (int l = 0; l < matrixSize; l++)
				mainMacierz[k][l] = macierz[k][l];

	}
	for (int i = 0; i < matrixSize; i++)
		bestRoute.push_back(routeTab[i]);

	bestRoute.push_back(0);
	delete[]routeTab;
	delete[]visitedTab;

	for (int i = 0; i < matrixSize; i++) {
		delete[]macierz[i];
		delete[]mainMacierz[i];
	}
	delete[]macierz;
	delete[]mainMacierz;

	return localMin;
}

int LocalSearch::getInitialReductionAndRandom(vector < unsigned >&bestTab) {
	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> nodeRand(1, matrixSize - 1);
	int localMin = 0;
	int randomNode;
	int **macierz = new int *[matrixSize]; //macierz do operacji
	int **mainMacierz = new int *[matrixSize]; //ta sama macierz, ale do odtwarzania
	for (int i = 0; i < matrixSize; i++) {
		macierz[i] = new int[matrixSize];
		mainMacierz[i] = new int[matrixSize];
	}
	copyMatrix(macierz);
	bool ifVisited;
	int bestMin = 0, tempMin = 0, savedBestCol = 0;
	int *visitedTab = new int[matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		visitedTab[i] = 0;
	}
	bestTab.push_back(0);

	for (int i = 0; i < amountRandomNodes; i++) {
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
		bestTab.push_back(randomNode);
		localMin = localMin + macierz[bestTab.at(i)][randomNode];
		suitableRowColToInf(macierz, bestTab.at(i), randomNode, matrixSize);

	}

	for (int i = amountRandomNodes; i < matrixSize - 1; i++) {
		bestMin = INT_MAX;
		if (i == amountRandomNodes) {
			for (int k = 0; k < matrixSize; k++)
				for (int l = 0; l < matrixSize; l++)
					mainMacierz[k][l] = macierz[k][l];
		}
		for (int j = 1; j < matrixSize; j++) {
			if (visitedTab[j] != -1) {
				suitableRowColToInf(macierz, bestTab.at(i), j, matrixSize); //ustawienie wartosci w odpowiednim wierszu i kolumnie na -1
				tempMin = reduceMatrix(macierz, matrixSize); //zredukowanie macierzy
				tempMin += mainMacierz[bestTab.at(i)][j]; //dodanie przejscia

				if (tempMin < bestMin) { //jako ze wszyscy synowie maja ten sam lower bound ojca to nie jest tu uwzgledniany
					bestMin = tempMin; //najlepszy lower bound (bez uwzglednienia lower bound ojca) jest zapisywany do bestMin
					savedBestCol = j; //id wierzcholka z bestMin jest zapisywane do savedBestCol
				}

				for (int k = 0; k < matrixSize; k++)
					for (int l = 0; l < matrixSize; l++)
						macierz[k][l] = mainMacierz[k][l];


			}
		}
		visitedTab[savedBestCol] = -1; //w tabeli wizyt oznaczamy wierzcholek o id savedBestCol na odwiedzony

		bestTab.push_back(savedBestCol);

		localMin = bestMin + localMin; // ustalenie lower bound
		suitableRowColToInf(macierz, bestTab.at(i), savedBestCol, matrixSize);
		reduceMatrix(macierz, matrixSize);
		for (int k = 0; k < matrixSize; k++)
			for (int l = 0; l < matrixSize; l++)
				mainMacierz[k][l] = macierz[k][l];

	}

	bestTab.push_back(0);

	delete[]visitedTab;

	for (int i = 0; i < matrixSize; i++) {
		delete[]macierz[i];
		delete[]mainMacierz[i];
	}
	delete[]macierz;
	delete[]mainMacierz;

	return localMin;
}

int LocalSearch::getInitialGreedy(vector < unsigned >&bestTab) {

	int localMin = 0;
	int bestMin, tempBest = 0, oldTempBest = 0;
	int *visitedTab = new int[matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		visitedTab[i] = 0;
	}
	int **macierz = new int *[matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		macierz[i] = new int[matrixSize];
	}

	copyMatrix(macierz);
	bool ifVisited;
	for (int i = 0; i < matrixSize; i++) {
		bestMin = INT_MAX;
		oldTempBest = tempBest;
		for (int j = 0; j < matrixSize; j++) {
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
		if (i < matrixSize - 1)
			localMin = localMin + bestMin;
		else
			localMin = localMin + macierz[oldTempBest][0];

		bestTab.push_back(oldTempBest);
		visitedTab[i] = tempBest;
	}
	bestTab.push_back(0);

	for (int i = 0; i < matrixSize; i++) {
		delete[]macierz[i];
	}
	delete[]macierz;
	delete[]visitedTab;

	return localMin;

}

int LocalSearch::getInitialGreedyAndRandom(vector < unsigned >&bestTab) {

	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> nodeRand(0, matrixSize - 1);

	int bestMin, tempBest = 0, oldTempBest = 0;
	int localMin = 0;
	int *visitedTab = new int[matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		visitedTab[i] = 0;
	}
	int **macierz = new int *[matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		macierz[i] = new int[matrixSize];
	}

	copyMatrix(macierz);
	bool ifVisited;
	int randomNode;

	for (int i = 0; i < matrixSize; i++) {
		bestMin = INT_MAX;
		oldTempBest = tempBest;
		if (amountRandomNodes != 0) {
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
			tempBest = randomNode;
			bestMin = macierz[oldTempBest][randomNode];
			amountRandomNodes--;

		}
		else {
			for (int j = 0; j < matrixSize; j++) {
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
		if (i < matrixSize - 1)
			localMin = localMin + bestMin;
		else
			localMin = localMin + macierz[oldTempBest][0];

		bestTab.push_back(oldTempBest);
		visitedTab[i] = tempBest;
	}
	bestTab.push_back(0);
	//cout << "Greedy min: " << helpMin << endl;
	for (int i = 0; i < matrixSize; i++) {
		delete[]macierz[i];
	}
	delete[]macierz;

	return localMin;
}

void LocalSearch::TabuMechanism(int a, int **TSPMatrix) {
	matrixSize = a;
	matrix = new int *[matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		matrix[i] = new int[matrixSize];
		for (int j = 0; j < matrixSize; j++)
			matrix[i][j] = TSPMatrix[i][j];
	}

	int greedyStart = 1;
	///*
	optMin = getInitialReduction();
	currentRoute = bestRoute;
	//*/
	/*
	vector < unsigned > route;
	optMin = getInitialGreedy(route);
	bestRoute = route;
	currentRoute = route;
	//*/
	bool continuing = true;
	currentTabuCadence = tabuCadence;
	currentOptMin = optMin;
	int balance = 0, bestBalance;
	int bestI = 0, bestJ = 0;
	int counter = 0, iterWithoutImprovement = 0;
	bool intensification, diversification = true, ifTabu;
	Czas onboardClock;

	onboardClock.start();

	while (continuing == true) {
		intensification = false;
		vector<unsigned> currentTabu(3, 0);

		currentTabu.at(2) = currentTabuCadence;

		if (neighborhoodType == 0) {
			bestBalance = getBestNeighborhoodReverse(bestI, bestJ);
			currentTabu.at(0) = currentRoute[bestI];
			currentTabu.at(1) = currentRoute[bestJ];
			reverseVector(bestI, bestJ);
		}

		if (neighborhoodType == 1) {
			bestBalance = getBestNeighborhoodSwap(bestI,bestJ);
			currentTabu.at(0) = currentRoute[bestI];
			currentTabu.at(1) = currentRoute[bestJ];
			swapVector(bestI, bestJ);
		}

		if (neighborhoodType == 2) {
			bestBalance = getBestNeighborhoodInsert(bestI, bestJ);
			currentTabu.at(0) = currentRoute[bestI];
			currentTabu.at(1) = currentRoute[bestJ - 1];
			insertVector(bestI, bestJ);
		}

		currentOptMin = currentOptMin + bestBalance;

		if (currentOptMin < optMin) {
			iterWithoutImprovement = 0; //wyzerowanie w razie znalezienia globalnego minimum
			intensification = true;
			optMin = currentOptMin; //lokalne minimum = globalne minimum
			bestRoute = currentRoute; //lokalna sciezka = globalna sciezka

		}

		//-----zinkrementowanie kadencji i usuniecie zerowych------
		cleanTabuList();

		//-----dodanie ruchu do listy tabu------
		tabuList.push_back(currentTabu);

		counter++;
		iterWithoutImprovement++;

		onboardClock.stop();
		//-----sprawdzenie czy uplynal czas------
		if (onboardClock.read() > tabuTime)
			continuing = false;

		//-----w razie znalezienie globalnego optimum intensyfikacja------
		if (intensification == true)
		{
			currentTabuCadence = tabuCadence / intensificationDiv;
			intensification = false;
		}

		//-----sprawdzenie czy nie doszlo do przekroczenia limutu iteracji------
		if (iterWithoutImprovement > iterationsLimit) {
			if (diversification == true)
			{
				///*
				tabuList.clear();
				// */
				vector < unsigned > route;

				//-----jednorazowe uruchomienie greedy------
				if (greedyStart != 0) {
					currentOptMin = getInitialGreedy(route);
					greedyStart--;
				}
				//-----dalej uruchamianie randomowych------
				else {

					if (algorithmType == true)
						currentOptMin = getInitialReductionAndRandom(route);
					else
						currentOptMin = getInitialGreedyAndRandom(route);
				}
				//-----powrot do domyslnej kadencji------
				currentTabuCadence = tabuCadence;
				//-----wyzerowanie liczby iteracji------
				iterWithoutImprovement = 0;
				//-----ustawienie obecnej sciezki------
				currentRoute = route;

			}
		}
	}
	clearParameters();
}

int LocalSearch::getBestNeighborhoodInsert(int &bestI, int &bestJ) {
	int bestBalance = INT_MAX;
	int balance;
	bool ifTabu;
	bestI = 0;
	bestJ = 0;
	for (int i = 1; i < matrixSize - 1; i++) {
		for (int j = 1; j <= matrixSize; j++) {

			if (i != j - 1 && i != j && i != j + 1) {

				calculateInsert(i, j, balance);

				ifTabu = false;

				for (int k = 0; k < tabuList.size(); k++)
				{
					if (tabuList.at(k).at(0) == currentRoute.at(i) && tabuList.at(k).at(1) == currentRoute.at(j - 1))
					{
						ifTabu = true;
						break;
					}
				}

				if (ifTabu == true && currentOptMin + balance >= optMin)
					continue;

				if (balance < bestBalance) {
					bestBalance = balance;
					bestI = i;
					bestJ = j;
				}
			}
		}
	}

	return bestBalance;
}

void LocalSearch::insertVector(int a, int b) {
	currentRoute.insert(currentRoute.begin() + b, currentRoute.at(a));
	if (b > a)
		currentRoute.erase(currentRoute.begin() + a);
	else
		currentRoute.erase(currentRoute.begin() + a + 1);
}

void LocalSearch::calculateInsert(int i, int j, int &balance) {
	balance = 0 - matrix[currentRoute.at(i)][currentRoute.at(i + 1)];
	balance = balance - matrix[currentRoute.at(j - 1)][currentRoute.at(j)];
	balance = balance - matrix[currentRoute.at(i - 1)][currentRoute.at(i)];
	balance = balance + matrix[currentRoute.at(i - 1)][currentRoute.at(i + 1)];
	balance = balance + matrix[currentRoute.at(j - 1)][currentRoute.at(i)];
	balance = balance + matrix[currentRoute.at(i)][currentRoute.at(j)];
}

int LocalSearch::getBestNeighborhoodReverse(int &bestI, int &bestJ) {
	int bestBalance = INT_MAX;
	int balance;
	bool ifTabu;
	bestI = 0;
	bestJ = 0;

	for (int i = 1; i < matrixSize - 1; i++) {
		for (int j = i + 1; j < matrixSize; j++) {

			calculateReverse(i, j, balance);

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

			if (ifTabu == true && currentOptMin + balance >= optMin)
				continue;

			if (balance < bestBalance) {
				bestBalance = balance;
				bestI = i;
				bestJ = j;
			}

		}
	}

	return bestBalance;
}

void LocalSearch::reverseVector(int a, int b) {
	reverse(currentRoute.begin() + a, currentRoute.begin() + b + 1);
}

void LocalSearch::calculateReverse(int i, int j, int &balance) {

	balance = 0 - matrix[currentRoute.at(i - 1)][currentRoute.at(i)] - matrix[currentRoute.at(j)][currentRoute.at(j + 1)];
	balance = balance + matrix[currentRoute.at(i - 1)][currentRoute.at(j)] + matrix[currentRoute.at(i)][currentRoute.at(j + 1)];

	for (int k = i; k < j; k++)
		balance = balance - matrix[currentRoute.at(k)][currentRoute.at(k + 1)] + matrix[currentRoute.at(k + 1)][currentRoute.at(k)];

}


int LocalSearch::getBestNeighborhoodSwap(int &bestI, int &bestJ) {
	int bestBalance = INT_MAX;
	int balance;
	bool ifTabu;
	bestI = 0;
	bestJ = 0;


	for (int i = 1; i < matrixSize - 1; i++) {
		for (int j = i + 1; j < matrixSize; j++) {

			calculateSwap(i, j, balance);

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

			if (ifTabu == true && currentOptMin + balance >= optMin)
				continue;

			if (balance < bestBalance) {
				bestBalance = balance;
				bestI = i;
				bestJ = j;
			}

		}
	}

	return bestBalance;
}

void LocalSearch::swapVector(int a, int b) {
	unsigned buffer = currentRoute.at(b);
	currentRoute.at(b) = currentRoute.at(a);
	currentRoute.at(a) = buffer;
}

void LocalSearch::calculateSwap(int i, int j, int &balance) {

	if (i + 1 == j) {
		balance = 0 - matrix[currentRoute.at(i - 1)][currentRoute.at(i)];
		balance = balance - matrix[currentRoute.at(i)][currentRoute.at(j)];
		balance = balance - matrix[currentRoute.at(j)][currentRoute.at(j + 1)];
		balance = balance + matrix[currentRoute.at(i - 1)][currentRoute.at(j)];
		balance = balance + matrix[currentRoute.at(j)][currentRoute.at(i)];
		balance = balance + matrix[currentRoute.at(i)][currentRoute.at(j + 1)];
	}
	else {
		balance = 0 - matrix[currentRoute.at(i - 1)][currentRoute.at(i)];
		balance = balance - matrix[currentRoute.at(i)][currentRoute.at(i + 1)];
		balance = balance - matrix[currentRoute.at(j - 1)][currentRoute.at(j)];
		balance = balance - matrix[currentRoute.at(j)][currentRoute.at(j + 1)];
		balance = balance + matrix[currentRoute.at(i - 1)][currentRoute.at(j)];
		balance = balance + matrix[currentRoute.at(j)][currentRoute.at(i + 1)];
		balance = balance + matrix[currentRoute.at(j - 1)][currentRoute.at(i)];
		balance = balance + matrix[currentRoute.at(i)][currentRoute.at(j + 1)];
	}

}


void LocalSearch::cleanTabuList() {

	for (int i = 0; i < tabuList.size(); i++) {
		tabuList.at(i).at(2)--;

		if (tabuList.at(i).at(2) == 0) {
			tabuList.erase(tabuList.begin() + i);
			i = i - 1;
		}
	}
}

void LocalSearch::SimulatedAnnealingMechanism(int a, int **TSPMatrix) {

	iterations = 0;
	optMin = 0;
	currentOptMin = 0;
	matrixSize = a;
	matrix = new int *[matrixSize];
	for (int i = 0; i < matrixSize; i++) {
		matrix[i] = new int[matrixSize];
		for (int j = 0; j < matrixSize; j++)
			matrix[i][j] = TSPMatrix[i][j];
	}

	if (algorithmType == 0) {
		optMin = getInitialReduction();
	}
	else if (algorithmType==1) {
		optMin = getInitialGreedy(bestRoute);
	}

	int counter = 0;
	//cout << "optMin = " << optMin << endl;
	currentRoute = bestRoute;
	//displayRoute(currentRoute);
	Czas onboardClock;
	int currentCost = optMin;
	onboardClock.start();

	double temperature = initialTemperature;
	currentOptMin = optMin;
	int balance;
	vector<unsigned> shuffled;

	do
	{
		for (auto i = 0; i < iterationsLimit; i++)
		{
			shuffled = currentRoute;
			balance = reshufflePath(shuffled);
			currentCost = currentCost + balance;
			
			if (currentCost < currentOptMin)
			{
				currentRoute = shuffled;
				currentOptMin = currentCost;
			}
			else if (static_cast<float>(rand()) / RAND_MAX < calculateProbability(currentOptMin, currentCost, temperature)) // Metropolis condition
			{
				currentRoute = shuffled;
				currentOptMin = currentCost;
			}
			else {
				currentCost = currentCost - balance;
			}
			iterations++;
		}
		static_cast<double>(temperature = initialTemperature/(1+0.1*counter));
		//cout << "temperature = " << temperature << endl;
		counter++;
	} while (temperature>minTemperature);
	//cout << "Iteracje = " << counter << endl;
	onboardClock.stop();
	time = onboardClock.read();
	bestRoute = currentRoute;
	optMin = currentOptMin;
	clearParameters();
	for (int i = 0; i < matrixSize; i++) {
		delete[]matrix[i];
	}
	delete[]matrix;

	shuffled.clear();
	vector<unsigned>().swap(shuffled);
}

int LocalSearch::reshufflePath(vector <unsigned>& shuffled) {

	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> nodeRand(1, matrixSize - 2);
	int i, j, balance = 0;

	if (neighborhoodType == 0) { //swap
		do {
			i = nodeRand(randomGen);
			j = nodeRand(randomGen);
		} while (i == j || j < i);

		calculateSwap(i, j, balance);

		unsigned buffer = shuffled.at(j);
		shuffled.at(j) = shuffled.at(i);
		shuffled.at(i) = buffer;
	}

	else if (neighborhoodType == 1) {
		do {
			i = nodeRand(randomGen);
			j = nodeRand(randomGen);
		} while (i == j - 1 || i == j || i == j + 1);

		calculateInsert(i, j, balance);

		shuffled.insert(shuffled.begin() + j, shuffled.at(i));
		if (j > i)
			shuffled.erase(shuffled.begin() + i);
		else
			shuffled.erase(shuffled.begin() + i + 1);
	}

	else if (neighborhoodType == 2) {
		do {
			i = nodeRand(randomGen);
			j = nodeRand(randomGen);
		} while (i == j || j < i);

		calculateReverse(i, j, balance);

		reverse(shuffled.begin() + i, shuffled.begin() + j + 1);

	}
	//cout << "balance " << balance << endl;
	return balance;

}

int LocalSearch::calculateProbability(int newCost, int oldCost, double temperature)
{
	float result;

	result = (exp(-(static_cast<float>((newCost - oldCost) / temperature))));

	return result;
}

void LocalSearch::clearParameters() {
	bestRoute.clear();
	currentRoute.clear();
}