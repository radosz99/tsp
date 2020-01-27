#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <float.h>
#include <bits.h> 
#include <string>
#include <conio.h>
#include "menu.h"
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "Czas.h"
#include "Node.h"
#include <vector>
#include "BruteForce.h"
#include "BranchBound.h"
#include "Dynamic.h"
#include "LocalSearch.h"
#include "Genetic.h"
#include <thread>
using namespace std;

int main()
{
	srand(time(NULL));
	string instanceName;
	int answer = 0;
	int matrixSize;
	int **TSPMatrix = 0;
	cout << "-----------------------------" << endl;
	cout << "-----PEA_zimowy2019/2020-----" << endl;
	cout << "---------RadoslawLis---------" << endl;
	cout << "-----------241385------------" << endl;
	cout << "-----------------------------" << endl << endl;

	Node start;

	do {
		answer = showMenu();
		Czas czas;
		system("cls");

		switch (answer) {

		case 1: {
			start.loadInfo();
			matrixSize = start.getStartSize();
			instanceName = start.getInstanceName();
			TSPMatrix = new int *[matrixSize];
			for (int i = 0; i < matrixSize; i++)
				TSPMatrix[i] = new int[matrixSize];
			start.copyMatrix(TSPMatrix);
			cout << endl;
			break;
		}

		case 2: {
			start.generateRandMatrix();
			matrixSize = start.getStartSize();
			instanceName = start.getInstanceName();
			TSPMatrix = new int *[matrixSize];
			for (int i = 0; i < matrixSize; i++)
				TSPMatrix[i] = new int[matrixSize];
			start.copyMatrix(TSPMatrix);
			cout << endl;
			break;
		}

		case 3: {
			int *knownPermu = new int[matrixSize];
			for (int i = 0; i < matrixSize; i++) {
				cout << "Podaj ktory wierzcholek bedzie odwiedzony jako " << i + 1 << ": ";
				cin >> knownPermu[i];
			}
			cout << "\nFunkcja celu dla zadanej drogi wynosi: " << calculate(knownPermu, matrixSize, TSPMatrix) << endl;
			break;
		}


		case 4:
			showMatrix(TSPMatrix, matrixSize);
			break;

		case 5:
			double time;
			int *bestPermut;
			int min;
			bestPermut = new int[matrixSize];

			czas.czasStart();
			min = getBestPermu(matrixSize, TSPMatrix, bestPermut);
			czas.czasStop();

			time = getTime(czas, 2);
			cout << "\nMinimalna funkcja celu = " << min << endl;
			cout << "Najlepsza droga: ";
			for (int i = 0; i < matrixSize; i++)
				cout << bestPermut[i] << " ";
			saveToFile(instanceName, "BruteForce-nextpermu", time, "us", min);
			cout << endl << endl;
			break;

		case 6: {
			int amount;
			double time;
			int *tab = new int[start.getStartSize()];
			int *bestTab = new int[start.getStartSize()];
			int min = INT_MAX;
			for (int i = 0; i < start.getStartSize(); i++)
				tab[i] = i;

			cout << "Podaj ile razy ma zostac wykonany algorytm: ";
			cin >> amount;

			for (int i = 0; i < amount; i++) {

				czas.czasStart();
				permute(tab, bestTab, 1, matrixSize - 1, min, matrixSize, TSPMatrix);
				czas.czasStop();

				time = getTime(czas, 2);
				cout << "\nMinimalna funkcja celu = " << calculate(bestTab, matrixSize, TSPMatrix) << endl;
				cout << "\Najlepsza droga: ";
				for (int i = 0; i < start.getStartSize(); i++)
					cout << bestTab[i] << " ";
				saveToFile(instanceName, "BruteForce-swap", time, "us", min);
				cout << endl << endl;
			}

			break;
		}

		case 7: {
			double time;
			int nodesAmount = 0;
			//int matrixSize = start.getStartSize();
			int *bestTab = new int[start.getStartSize()];
			int min = 9999;

			czas.czasStart();
			tree(nodesAmount, matrixSize, bestTab, min, TSPMatrix);
			czas.czasStop();

			time = getTime(czas, 2);
			cout << "\nMinimalna funkcja celu = " << min << endl;
			cout << "Najlepsza droga: ";
			for (int i = 0; i < matrixSize; i++)
				cout << bestTab[i] << " ";
			//cout << "\nUtworzonych wierzcholkow = " << nodesAmount;
			saveToFile(instanceName, "BruteForce-drzewo", time, "us", min);
			cout << endl << endl;
			break;
		}

		case 8: {
			int minimum;
			double time;
			string method;
			int *route = new int[matrixSize];
			int amount;
			int timeType = askTime();
			int odp = 0;

			cout << "Podaj ile razy ma zostac wykonany algorytm: ";
			cin >> amount;

			while (odp != 1 && odp != 2) {
				cout << "Wersja 1)depth first, 2)best first: ";
				cin >> odp;
			}
			if (odp == 1)
				method = "Branch&Bound DF";
			if (odp == 2)
				method = "Branch&Bound BF";
			for (int i = 0; i < amount; i++) {
				czas.czasStart();
				minimum = BBMain(start, instanceName, route, odp);
				czas.czasStop();

				time = getTime(czas, timeType);
				if (timeType == 1) {
					cout << "\nCzas wykonania =  " << time << "ms";
					saveToFile(instanceName, method, time, "ms", minimum);
				}
				if (timeType == 2) {
					cout << "\nCzas wykonania =  " << time << "us";
					saveToFile(instanceName, method, time, "us", minimum);
				}
				if (timeType == 3) {
					cout << "\nCzas wykonania =  " << time << "ns";
					saveToFile(instanceName, method, time, "ns", minimum);
				}
				cout << "\nMinimalna funkcja celu = " << minimum;
				cout << "\nNajlepsza droga: ";
				for (int i = 0; i < matrixSize; i++)
					cout << route[i] << " ";
				cout << endl << endl;
			}
			break;
		}

		case 9: {
			double time;
			int min = INT_MAX;
			vector<vector<int>> tabNodeValues;
			vector<vector<int>> possibleRouteTab;
			int amount;
			int *bestTab = new int[matrixSize];

			tabNodeValues = vector<vector<int>>(matrixSize);
			possibleRouteTab = vector<vector<int>>(matrixSize);

			for (int i = 0; i < matrixSize; i++) {
				tabNodeValues[i] = vector<int>(pow(2, matrixSize) - 1);
				possibleRouteTab[i] = vector<int>(pow(2, matrixSize) - 1);
			}

			int timeType = askTime();

			cout << "Podaj ile razy ma zostac wykonany algorytm: ";
			cin >> amount;

			for (int j = 0; j < amount; j++) {

				czas.czasStart();
				startDynamic(min, TSPMatrix, matrixSize, tabNodeValues, possibleRouteTab, bestTab);
				czas.czasStop();

				time = getTime(czas, timeType);

				if (timeType == 1) {
					cout << "\nCzas wykonania =  " << time << "ms";
					saveToFile(instanceName, "Dynamiczne", time, "ms", min);
				}
				if (timeType == 2) {
					cout << "\nCzas wykonania =  " << time << "us";
					saveToFile(instanceName, "Dynamiczne", time, "us", min);
				}
				if (timeType == 3) {
					cout << "\nCzas wykonania =  " << time << "ns";
					saveToFile(instanceName, "Dynamiczne", time, "ns", min);
				}

				cout << "\nMinimalna funkcja celu = " << min;
				cout << "\nNajlepsza droga: ";
				for (int i = 0; i < matrixSize; i++)
					cout << bestTab[i] << " ";
				cout << endl << endl;

			}
			break;
		}

		case 10: {
			/*
			//-----tabele z roznymi wartosciami parametrow do testow---
			string matrixes[5] = { "data45.txt","data53.txt","data56.txt","data65.txt","ftv47.txt" };
			int cadenceTab[1] = { 45 }; //kadencja
			int timeTab[7] = {1,5,10,15,25,30 };//czas wykonania algorytmu
			int iterTab[1] = { 5000 };//limit iteracji bez poprawy, po osiagnieciu ktorego jest generowana nowa sciezka
			int divCadTab[1] = { 4}; //dzielnik kadencji (intensyfikacja) w przypadku znalezienia globalnie najlepszego rozwiazania
			int randNodesTab[1] = {2 };//liczba poczatkowych losowych wierzcholkow przy generowaniu nowej sciezki
			bool algTab[1] = { false };//rodzaj algorytmu generujacego nowa sciezke
			int typesTab[1] = { 2 };//rodzaj sasiedztwa

			LocalSearch tabu;


			
			for (int x = 0; x < sizeof(matrixes) / sizeof(*matrixes); x++) {

				Node start;
				start.loadInfoGiven(matrixes[x]);
				matrixSize = start.getStartSize();
				instanceName = start.getInstanceName();
				TSPMatrix = new int *[matrixSize];
				for (int i = 0; i < matrixSize; i++)
					TSPMatrix[i] = new int[matrixSize];
				start.copyMatrix(TSPMatrix);


				for (int a = 0; a < sizeof(cadenceTab) / sizeof(*cadenceTab); a++)
					for (int b = 0; b < sizeof(timeTab) / sizeof(*timeTab); b++)
						for (int c = 0; c < sizeof(iterTab) / sizeof(*iterTab); c++)
							for (int d = 0; d < sizeof(divCadTab) / sizeof(*divCadTab); d++)
								for (int e = 0; e < sizeof(randNodesTab) / sizeof(*randNodesTab); e++)
									for (int f = 0; f < sizeof(algTab) / sizeof(*algTab); f++)
										for (int g = 0; g < sizeof(typesTab) / sizeof(*typesTab); g++) {
											tabu.setSettingsTabu(cadenceTab[a], timeTab[b], iterTab[c], divCadTab[d], randNodesTab[e], algTab[f], typesTab[g]);
											for (int repeat = 0; repeat < 5; repeat++) {
												tabu.TabuMechanism(matrixSize, TSPMatrix);

												cout << "Optimum = " << tabu.getOptMin() << endl;
												
												cout << "Sciezka: ";
												tabu.displayRoute();
											
												tabu.saveToFileTabu(instanceName);
											}
										}
			}
			*/


			//-----tabele z roznymi wartosciami parametrow do testow---
			string matrixes[1] = { "data171.txt" };
			int cadenceTab[1] = { 120 }; //kadencja
			int timeTab[7] = { 5,10,15,20,25,30 };//czas wykonania algorytmu
			int iterTab[1] = { 5000 };//limit iteracji bez poprawy, po osiagnieciu ktorego jest generowana nowa sciezka
			int divCadTab[1] = {9}; //dzielnik kadencji (intensyfikacja) w przypadku znalezienia globalnie najlepszego rozwiazania
			int randNodesTab[1] = { 5 };//liczba poczatkowych losowych wierzcholkow przy generowaniu nowej sciezki
			bool algTab[1] = { false };//rodzaj algorytmu generujacego nowa sciezke
			int typesTab[1] = { 2};//rodzaj sasiedztwa

			LocalSearch tabu;


			int matrixId = -1;
			for (int x = 0; x < sizeof(matrixes) / sizeof(*matrixes); x++) {
				matrixId++;
				Node start;
				start.loadInfoGiven(matrixes[x]);
				matrixSize = start.getStartSize();
				instanceName = start.getInstanceName();
				TSPMatrix = new int *[matrixSize];
				for (int i = 0; i < matrixSize; i++)
					TSPMatrix[i] = new int[matrixSize];
				start.copyMatrix(TSPMatrix);
				//*/

				for (int a = 0; a < sizeof(timeTab) / sizeof(*timeTab); a++){
					tabu.setSettingsTabu(cadenceTab[matrixId], timeTab[a], iterTab[matrixId], divCadTab[matrixId], randNodesTab[matrixId], algTab[matrixId], typesTab[matrixId]);
					for (int repeat = 0; repeat < 20; repeat++) {
						tabu.TabuMechanism(matrixSize, TSPMatrix);
						cout << "Optimum = " << tabu.getOptMin() << endl;
						tabu.saveToFileTabu(instanceName);
					}
				}
			}

			cout << endl;
			cout << "Testy wykonane, wyniki zapisane do Output/wynikiTestyTabu.csv" << endl;
			cout << endl;
			break;
		}

		case 11: {
			
			//string matrixes[1] = {"data34.txt" }; //nazwy plikow txt z instancjami do testow
			double initTemp[1] = {100.0}; //poczatkowe temperatury
			double minTemp[1] = { 0.1 }; //minimalne temperatury
			int iter[1] = { 100 };//liczba iteracji przypadajaca na jedna temperature
			double cooling[1] = { 0.999};//wspolczynnik chlodzenia
			bool alg[1]= { false};//rodzaj algorytmu generujacego poczatkowa sciezke
			int neigh[1] = {1}; //rodzaj sasiedztwa
		

			LocalSearch sa;

			/* dla testow wielu macierzy na raz
			for (int x = 0; x < sizeof(matrixes) / sizeof(*matrixes); x++) {
				
				Node start;
				start.loadInfoGiven(matrixes[x]);
				matrixSize = start.getStartSize();
				instanceName = start.getInstanceName();
				TSPMatrix = new int *[matrixSize];
				for (int i = 0; i < matrixSize; i++)
					TSPMatrix[i] = new int[matrixSize];
				start.copyMatrix(TSPMatrix);
			*/

				for (int a = 0; a < sizeof(initTemp) / sizeof(*initTemp); a++) 
					for (int b = 0; b < sizeof(minTemp) / sizeof(*minTemp); b++)
						for (int c = 0; c < sizeof(iter) / sizeof(*iter); c++)
							for (int d = 0; d < sizeof(cooling) / sizeof(*cooling); d++)
								for (int e = 0; e < sizeof(alg) / sizeof(*alg); e++)
									for (int f = 0; f < sizeof(neigh) / sizeof(*neigh); f++) {
										sa.setSettingSA(initTemp[a], minTemp[b], iter[c], cooling[d], alg[e], neigh[f]);
										for (int repeat = 0; repeat < 10; repeat++) { //liczba powtorzen dla jednej kombinacji parametrow
											sa.SimulatedAnnealingMechanism(matrixSize, TSPMatrix);
											cout << "Optimum = " << sa.getOptMin() << endl;
											sa.saveToFileSA(instanceName);
										}
									}
			//}
			
			cout << endl;
			cout << "Testy wykonane, wyniki zapisane do Output/wynikiTestySA.csv" << endl;
			cout << endl;

			break;
		}

		case 12: {

			// W FORMIE TESTOW DLA WIELU PARAMETROW
			//string matrixes[5] = {"data45.txt","data53.txt","data56.txt","data65.txt","ftv47.txt"};
			string matrixes[1] = { "data171.txt"};
			int populationSize[1] = { 150};
			int amountRandomNodes[1] = { 3 };
			double mutationProb[1] = { 0.10};
			int selectionType[1] = { 1};// 1-TS, 2-RS, 3-RWS
			int mutationType[1] = { 1 };// 2-reverse, 1-insert, 0-swap
			int memeticType[1] = { 1 };// 2-reverse, 1-insert, 0-swap
			int crossoverType[1] = {6}; // 1-PMX, 2-OX, 3-CX, 4-CX2, 5-TPX, 6-SCX, 7-ESCX
			int elitismNumber[1] = { 10 };
			int timeGenetic[6] = {5,10,15,20,25,30}; //czasy
			Czas czas1;
			int result;


			for (int x = 0; x < sizeof(matrixes) / sizeof(*matrixes); x++) {
				Node start;
				start.loadInfoGiven(matrixes[x]);
				matrixSize = start.getStartSize();
				instanceName = start.getInstanceName();
				TSPMatrix = new int *[matrixSize];
				for (int i = 0; i < matrixSize; i++)
					TSPMatrix[i] = new int[matrixSize];
				start.copyMatrix(TSPMatrix);

				for (int a = 0; a < sizeof(populationSize) / sizeof(*populationSize); a++)
					for (int b = 0; b < sizeof(amountRandomNodes) / sizeof(*amountRandomNodes); b++)
						for (int c = 0; c < sizeof(elitismNumber) / sizeof(*elitismNumber); c++)
							for (int d = 0; d < sizeof(mutationProb) / sizeof(*mutationProb); d++)
								for (int e = 0; e < sizeof(selectionType) / sizeof(*selectionType); e++)
									for (int f = 0; f < sizeof(mutationType) / sizeof(*mutationType); f++)
										for (int g = 0; g < sizeof(memeticType) / sizeof(*memeticType); g++)
											for (int h = 0; h < sizeof(crossoverType) / sizeof(*crossoverType); h++)
												for (int i = 0; i < sizeof(timeGenetic) / sizeof(*timeGenetic); i++)
													for (int repeat = 0; repeat < 50; repeat++) {
														Genetic gen;
														gen.setSettingsGenetic(populationSize[a], amountRandomNodes[b], mutationProb[d], crossoverType[h], selectionType[e], mutationType[f], elitismNumber[c],memeticType[g],timeGenetic[i]);
														vector<unsigned> iterations;
														czas1.czasStart();
														result = gen.GeneticMechanism(matrixSize, TSPMatrix, iterations);
														czas1.czasStop();
														saveToFileGenetic(instanceName, getTime(czas1, 1), result, iterations, populationSize[a], amountRandomNodes[b], mutationProb[d], crossoverType[h], selectionType[e], mutationType[f], elitismNumber[c], memeticType[g], timeGenetic[i]);
														cout << "Best = " << result << endl;
													}
			}
			

			cout << endl;
			cout << "Testy wykonane, wyniki zapisane do Output/wynikiTestyGenetic.csv" << endl;
			cout << endl;

			break;
		}


		case 13: {
			int odp;
			int odp1, valMin, valMax, instSize, instSizeMin;
			int **matrix = 0;
			int *tab = 0;
			int *bestTab = 0;
			int min;
			double time;
			Czas czas1;
			cout << "1. BRUTE FORCE (swap)" << endl;
			cout << "2. BRANCH & BOUND" << endl;
			cout << "3. DYNAMICZNE PROGRAMOWANIE" << endl;
			cin >> odp;
			system("cls");
			switch (odp) {
			case 1: {
				cout << "Liczba testow: ";
				cin >> odp1;
				cout << "Dolna granica wartosci: ";
				cin >> valMin;
				cout << "Gorna granica wartosci: ";
				cin >> valMax;
				cout << "Minimalna wielkosc instancji: ";
				cin >> instSizeMin;
				cout << "Maksymalna wielkosc instancji: ";
				cin >> instSize;


				for (int k = instSizeMin; k <= instSize; k++) {
					for (int i = 0; i < odp1; i++) {

						//-----------------------------------
						matrix = new int*[k];
						for (int i = 0; i < k; i++)
							matrix[i] = new int[k];

						for (int i = 0; i < k; i++) {
							for (int j = 0; j < k; j++) {
								if (i == j)
									matrix[i][j] = -1;
								else
									matrix[i][j] = (double)rand() / (RAND_MAX + 1) * (valMax - valMin) + valMin;
							}
						}
						//-----------------------------------

						/*
						for (int i = 0; i < k; i++) {
							for (int j = 0; j < k; j++)
								cout << matrix[i][j] << " ";
							cout << endl;
						}
						*/

						tab = new int[k];
						bestTab = new int[k];
						min = INT_MAX;

						for (int i = 0; i < k; i++)
							tab[i] = i;

						czas1.czasStart();
						permute(tab, bestTab, 1, k - 1, min, k, matrix);
						czas1.czasStop();

						if (k < 7) {
							time = getTime(czas1, 3);
							saveToFileRand("rand", k, i, "BruteForce-swap", time, "ns", min, valMin, valMax);
						}
						if (k >= 7 && k <= 10) {
							time = getTime(czas1, 2);
							saveToFileRand("rand", k, i, "BruteForce-swap", time, "us", min, valMin, valMax);
						}

						if (k > 10) {
							time = getTime(czas1, 1);
							saveToFileRand("rand", k, i, "BruteForce-swap", time, "ms", min, valMin, valMax);
						}
					}
				}
				cout << endl;
				cout << "Testy wykonane, wyniki zapisane do Output/wynikiTesty.csv" << endl;
				cout << endl;
				break;
			}

			case 2: {
				int minimum;
				int *route = 0;
				string method;
				Node start1;
				int odpBB = 0;
				while (odpBB != 1 && odpBB != 2) {
					cout << "Wersja 1)depth first, 2)best first: ";
					cin >> odpBB;
				}


				cout << "Liczba testow: ";
				cin >> odp1;
				cout << "Dolna granica wartosci: ";
				cin >> valMin;
				cout << "Gorna granica wartosci: ";
				cin >> valMax;
				cout << "Minimalna wielkosc instancji: ";
				cin >> instSizeMin;
				cout << "Maksymalna wielkosc instancji: ";
				cin >> instSize;


				for (int k = instSizeMin; k <= instSize; k++) {
					for (int i = 0; i < odp1; i++) {
						route = new int[k];
						//-----------------------------------
						matrix = new int*[k];
						for (int i = 0; i < k; i++)
							matrix[i] = new int[k];

						for (int i = 0; i < k; i++) {
							for (int j = 0; j < k; j++) {
								if (i == j)
									matrix[i][j] = -1;
								else
									matrix[i][j] = (double)rand() / (RAND_MAX + 1) * (valMax - valMin) + valMin;
							}
						}

						start1.setMatrix(matrix, k);
						//-----------------------------------
						tab = new int[k];
						bestTab = new int[k];
						min = INT_MAX;

						czas1.czasStart();
						minimum = BBMain(start1, "rand", route, odpBB);
						czas1.czasStop();

						if (odpBB == 1)
							method = "Branch&Bound DF";
						if (odpBB == 2)
							method = "Branch&Bound BF";

						if (k < 7) {
							time = getTime(czas1, 3);
							saveToFileRand("rand", k, i, method, time, "ns", minimum, valMin, valMax);
						}
						if (k >= 7 && k <= 19) {
							time = getTime(czas1, 2);
							saveToFileRand("rand", k, i, method, time, "us", minimum, valMin, valMax);
						}

						if (k > 19) {
							time = getTime(czas1, 1);
							saveToFileRand("rand", k, i, method, time, "ms", minimum, valMin, valMax);
						}

					}

					delete[]route;
					delete[]tab;
					delete[]bestTab;

					for (int i = 0; i < k; i++) {
						delete[]matrix[i];
					}
					delete[]matrix;
				}
				cout << endl;
				cout << "Testy wykonane, wyniki zapisane do Output/wynikiTesty.csv" << endl;
				cout << endl;
				break;
			}
			case 3: {
				cout << "Liczba testow: ";
				cin >> odp1;
				cout << "Dolna granica wartosci: ";
				cin >> valMin;
				cout << "Gorna granica wartosci: ";
				cin >> valMax;
				cout << "Minimalna wielkosc instancji: ";
				cin >> instSizeMin;
				cout << "Maksymalna wielkosc instancji: ";
				cin >> instSize;


				for (int k = instSizeMin; k <= instSize; k++) {

					for (int i = 0; i < odp1; i++) {
						//-----------------------------------
						matrix = new int*[k];
						for (int i = 0; i < k; i++)
							matrix[i] = new int[k];

						for (int i = 0; i < k; i++) {
							for (int j = 0; j < k; j++) {
								if (i == j)
									matrix[i][j] = -1;
								else
									matrix[i][j] = (double)rand() / (RAND_MAX + 1) * (valMax - valMin) + valMin;
							}
						}

						min = INT_MAX;
						vector<vector<int>> tabNodeValues;
						vector<vector<int>> possibleRouteTab;
						int *bestTab = new int[k];

						tabNodeValues = vector<vector<int>>(k);
						possibleRouteTab = vector<vector<int>>(k);

						for (int i = 0; i < k; i++) {
							tabNodeValues[i] = vector<int>(pow(2, k) - 1);
							possibleRouteTab[i] = vector<int>(pow(2, k) - 1);
						}


						czas1.czasStart();
						startDynamic(min, matrix, k, tabNodeValues, possibleRouteTab, bestTab);
						czas1.czasStop();


						delete[]bestTab;

						for (int i = 0; i < k; i++) {
							delete[]matrix[i];
						}
						delete[]matrix;

						if (k < 9) {
							time = getTime(czas1, 3);
							saveToFileRand("rand", k, i, "Dynamiczne", time, "ns", min, valMin, valMax);
						}

						if (k >= 9 && k < 18) {
							time = getTime(czas1, 2);
							saveToFileRand("rand", k, i, "Dynamiczne", time, "us", min, valMin, valMax);
						}

						if (k >= 18) {
							time = getTime(czas1, 1);
							saveToFileRand("rand", k, i, "Dynamiczne", time, "ms", min, valMin, valMax);
						}
					}
				}
				cout << endl;
				cout << "Testy wykonane, wyniki zapisane do Output/wynikiTesty.csv" << endl;
				cout << endl;
				break;
			}
			}
			break;
		}

		case 14:
			system("pause");
			break;
		}
	} while (answer != 14);

	return 0;
}
