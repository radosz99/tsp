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
#include "BruteForceFunc.h"
#include "BBFunc.h"
#include "DPFunc.h"
#include "Tabu.h"
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

			case 1:{
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

			case 3:{
				int *knownPermu = new int[matrixSize];
				for (int i = 0; i < matrixSize; i++) {
					cout << "Podaj ktory wierzcholek bedzie odwiedzony jako " << i+1 << ": ";
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
				min=getBestPermu(matrixSize, TSPMatrix, bestPermut);
				czas.czasStop();

				time = getTime(czas,2);
				cout << "\nMinimalna funkcja celu = " << min << endl;
				cout << "Najlepsza droga: ";
				for (int i = 0; i < matrixSize; i++)
					cout << bestPermut[i] << " ";
				saveToFile(instanceName, "BruteForce-nextpermu", time,"us", min);
				cout << endl << endl;
				break;

			case 6:{
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

					time = getTime(czas,2);
					cout << "\nMinimalna funkcja celu = " << calculate(bestTab, matrixSize, TSPMatrix) << endl;
					cout << "\Najlepsza droga: ";
					for (int i = 0; i < start.getStartSize(); i++)
						cout << bestTab[i] << " ";
					saveToFile(instanceName, "BruteForce-swap", time, "us",min);
					cout << endl << endl;
				}

				break;
			}

			case 7:{
				double time;
				int nodesAmount = 0;
				//int matrixSize = start.getStartSize();
				int *bestTab = new int[start.getStartSize() ];
				int min = 9999;

				czas.czasStart();
				tree(nodesAmount, matrixSize, bestTab, min, TSPMatrix);
				czas.czasStop();

				time = getTime(czas,2);
				cout << "\nMinimalna funkcja celu = " << min << endl;
				cout << "Najlepsza droga: ";
				for (int i = 0; i < matrixSize; i++)
					cout << bestTab[i] << " ";
				//cout << "\nUtworzonych wierzcholkow = " << nodesAmount;
				saveToFile(instanceName, "BruteForce-drzewo", time,"us", min);
				cout << endl << endl;
				break;
			}

			case 8:{
				int minimum;
				double time;
				string method;
				int *route = new int[matrixSize];
				int amount;
				int timeType = askTime();
				int odp=0;

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
					minimum = BBMain(start, instanceName, route,odp);
					czas.czasStop();

					time = getTime(czas,timeType);
					if (timeType == 1) {
						cout << "\nCzas wykonania =  " << time << "ms";
						saveToFile(instanceName, method, time, "ms",minimum);
					}
					if (timeType == 2) {
						cout << "\nCzas wykonania =  " << time << "us";
						saveToFile(instanceName, method, time, "us", minimum);
					}
					if (timeType == 3) {
						cout << "\nCzas wykonania =  " << time << "ns";
						saveToFile(instanceName, method, time, "ns",minimum);
					}
					cout << "\nMinimalna funkcja celu = " << minimum;
					cout << "\nNajlepsza droga: ";
					for (int i = 0; i < matrixSize; i++)
						cout << route[i] << " ";
					cout << endl << endl;
				}
				break;
			}

			case 9:{
				double time;
				int min=INT_MAX;
				vector<vector<int>> tabNodeValues;
				vector<vector<int>> possibleRouteTab;
				int amount;
				int *bestTab = new int[matrixSize];

				tabNodeValues = vector<vector<int>>(matrixSize);
				possibleRouteTab = vector<vector<int>>(matrixSize);

				for (int i = 0; i < matrixSize; i++) {
					tabNodeValues[i] = vector<int>(pow(2, matrixSize)-1);
					possibleRouteTab[i] = vector<int>(pow(2, matrixSize)-1);
				}

				int timeType = askTime();

				cout << "Podaj ile razy ma zostac wykonany algorytm: ";
				cin >> amount;

				for (int j = 0; j < amount; j++) {

					czas.czasStart();
					startDynamic(min, TSPMatrix, matrixSize, tabNodeValues, possibleRouteTab,bestTab);
					czas.czasStop();

					time = getTime(czas,timeType);

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

				//----------------------------------------------------------------
				//-------POROWNANIE REDUCTION_RANDOM z GREEDY_RANDOM--------
				/*
				int counterReduc = 0, counterGreedy = 0;
				for (int i = 0; i < 100; i++) {
					vector < unsigned > bestRouteRed;
					int actualMinReduc = 0;
					getInitialReductionAndRandom(bestRouteRed, actualMinReduc, start, 1);
					//cout << "Reduc" << actualMinReduc << endl;


					int actualMinGreedy = 0;
					vector < unsigned > bestRouteGreedy;
					getInitialGreedyAndRandom(bestRouteGreedy, actualMinGreedy, start, 1);
					//cout << "Greedy" << actualMinGreedy << endl;

					if (actualMinReduc > actualMinGreedy) {
						counterGreedy++;
					}
					if (actualMinReduc < actualMinGreedy) {
						counterReduc++;
					}
				}
				cout << "Losowa redukcja okazala sie lepsza " << counterReduc << " razy." << endl;
				cout << "Losowy zachlanny okazal sie lepszy " << counterGreedy << " razy." << endl;

				vector < unsigned > bestRoute;
				vector < unsigned > bestRouteGreedy;
				int min = 0;

				getInitialReduction(bestRoute, min, start);
				cout << "Funkcja celu redukcja " << min << endl;
				min = 0;
				getInitialGreedy(bestRouteGreedy, min, start);
				cout << "Funkcja celu zachlanny " << min << endl << endl;
				*/
				//----------------------------------------------------------------

				/*
				int result = 0;
				for (int i = 0; i < 10; i++) {
					result = Tabu(start, matrixSize, TSPMatrix, 20, 15, 10000, 4, 1, true); /dla data34
					cout << result << endl;
				}
				*/
				
				///*
				int cadenceTab[3] = {27,30,33};
				int timeTab[1] = {60};
				int iterTab[2] = { 10000,20000 };
				int divCadTab[1] = {4 };
				int randNodesTab[1] = { 1 };
				bool algTab[1] = { true };

				for (int a = 0; a < sizeof(cadenceTab) / sizeof(*cadenceTab); a++) {
					for (int b = 0; b < sizeof(timeTab) / sizeof(*timeTab); b++) {
						for (int c = 0; c < sizeof(iterTab) / sizeof(*iterTab); c++) {
							for (int d = 0; d < sizeof(divCadTab) / sizeof(*divCadTab); d++) {
								for (int e = 0; e < sizeof(randNodesTab) / sizeof(*randNodesTab);e++) {
									for (int f = 0; f < sizeof(algTab) / sizeof(*algTab); f++) {
										int result = 0;
										for (int repeat = 0; repeat < 5; repeat++) {
											result = 0;
											result = Tabu(start, matrixSize, TSPMatrix, cadenceTab[a], timeTab[b], iterTab[c], divCadTab[d], randNodesTab[e], algTab[f]);
											cout << result << endl;
											saveToFileTabu(cadenceTab[a], timeTab[b], iterTab[c], divCadTab[d], randNodesTab[e], algTab[f], result);
										}
									}
								}
							}
						}
					}
				}
				//*/
				
				break;
			}

			case 11:{
				int odp;
				int odp1, valMin, valMax, instSize, instSizeMin;
				int **matrix = 0;
				int *tab=0;
				int *bestTab = 0;
				int min;
				double time;
				Czas czas1;
				cout << "1. BRUTE FORCE (swap)" << endl;
				cout << "2. BRANCH & BOUND" << endl;
				cout << "3. DYNAMICZNE PROGRAMOWANIE" << endl;
				cout << "4. TABU SEARCH" << endl;
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
									saveToFileRand("rand", k, i, "BruteForce-swap", time, "us",min, valMin, valMax);
								}

								if (k > 10) {
									time = getTime(czas1, 1);
									saveToFileRand("rand", k, i, "BruteForce-swap", time, "ms",min, valMin, valMax);
								}
							}
						}
						cout << endl;
						cout << "Testy wykonane, wyniki zapisane do Output/wynikiTesty.csv" << endl;
						cout << endl;
						break;
					}

					case 2:{
						int minimum;
						int *route=0;
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
										saveToFileRand("rand", k, i, method, time, "us",minimum, valMin, valMax);
									}

									if (k > 19) {
										time = getTime(czas1, 1);
										saveToFileRand("rand", k, i, method, time, "ms",minimum, valMin, valMax);
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


						for (int k = instSizeMin; k <=instSize; k++) {

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
									saveToFileRand("rand", k, i, "Dynamiczne", time, "ns",min, valMin, valMax);
								}

								if (k >= 9 && k<18) {
									time = getTime(czas1, 2);
									saveToFileRand("rand", k, i, "Dynamiczne", time, "us",min, valMin, valMax);
								}

								if (k >= 18) {
									time = getTime(czas1, 1);
									saveToFileRand("rand", k, i, "Dynamiczne", time, "ms",min, valMin, valMax);
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

			case 12:
				system("pause");
				break;
		}
	} while (answer != 12);

	return 0;
}
