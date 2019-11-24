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
using namespace std;


int main()
{
	srand(time(NULL));
	string instanceName;
	int answer;
	int sizeMatrix;
	int **TSPMatrix;
	Node start;
	start.loadInfo();
	instanceName = start.getInstanceName();
	sizeMatrix = start.getStartSize();
	TSPMatrix = new int *[sizeMatrix];
	for (int i = 0; i < sizeMatrix; i++)
		TSPMatrix[i] = new int[sizeMatrix];
	start.copyMatrix(TSPMatrix);

	do {
		answer = showMenu();
		Czas czas;
		system("cls");

		switch (answer) {

		case 1:
		{
			start.loadInfo();
			sizeMatrix = start.getStartSize();
			instanceName = start.getInstanceName();
			TSPMatrix = new int *[sizeMatrix];
			for (int i = 0; i < sizeMatrix; i++)
				TSPMatrix[i] = new int[sizeMatrix];
			start.copyMatrix(TSPMatrix);
			break;
		}

		case 2:
		{
			int *knownPermu = new int[sizeMatrix];
			for (int i = 0; i < sizeMatrix; i++) {
				cout << "Podaj ktory wierzcholek bedzie odwiedzony jako " << i+1 << ": ";
				cin >> knownPermu[i];
			}
			cout << "\nFunkcja celu dla zadanej drogi wynosi: " << calculate(knownPermu, sizeMatrix, TSPMatrix) << endl;
			break;
		}

		case 3:
			double time; 
			int *bestPermut;
			int min;
			bestPermut = new int[sizeMatrix];

			czas.czasStart();
			min=getBestPermu(sizeMatrix, TSPMatrix, bestPermut);
			czas.czasStop();

			time = getTime(czas,2);
			cout << "\nMinimalna funkcja celu = " << min << endl;
			cout << "Najlepsza droga: ";
			for (int i = 0; i < sizeMatrix; i++)
				cout << bestPermut[i] << " ";
			saveToFile(instanceName, "BruteForce-nextpermu", time, min);
			cout << endl << endl;
			break;

		case 4:
		{
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
				permute(tab, bestTab, 1, sizeMatrix - 1, min, sizeMatrix, TSPMatrix);
				czas.czasStop();

				time = getTime(czas,2);
				cout << "\nMinimalna funkcja celu = " << calculate(bestTab, sizeMatrix, TSPMatrix) << endl;
				cout << "\Najlepsza droga: ";
				for (int i = 0; i < start.getStartSize(); i++)
					cout << bestTab[i] << " ";
				saveToFile(instanceName, "BruteForce-swap", time, min);
				cout << endl << endl;
			}

			break;
		}

		case 5:
		{
			double time;
			int nodesAmount = 0;
			int matrixSize = start.getStartSize();
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
			saveToFile(instanceName, "BruteForce-drzewo", time, min);
			cout << endl << endl;
			break;
		}

		case 6:
		{
			int minimum;
			double time;
			int *route = new int[sizeMatrix];
			int amount;
			int timeType = askTime();
			int odp=0;

			cout << "Podaj ile razy ma zostac wykonany algorytm: ";
			cin >> amount;

			while (odp != 1 && odp != 2) {
				cout << "Wersja 1)depth first, 2)best first: ";
				cin >> odp;
			}

			for (int i = 0; i < amount; i++) {
				czas.czasStart();
				minimum = BBMain(start, instanceName, route,odp);
				czas.czasStop();

				time = getTime(czas,timeType);
				cout << "\nMinimalna funkcja celu = " << minimum;
				cout << "\nNajlepsza droga: ";
				for (int i = 0; i < sizeMatrix; i++)
					cout << route[i] << " ";
				cout << endl;
				if(odp==1)
					saveToFile(instanceName, "Branch&Bound DF ", time, minimum);
				if(odp==2)
					saveToFile(instanceName, "Branch&Bound BF", time, minimum);
			}
			break;
		}

		case 7:
		{
			double time;
			int min=INT_MAX;
			vector<vector<int>> tabNodeValues;
			vector<vector<int>> possibleRouteTab;
			int amount;
			int *bestTab = new int[sizeMatrix];

			tabNodeValues = vector<vector<int>>(sizeMatrix);
			possibleRouteTab = vector<vector<int>>(sizeMatrix);

			for (int i = 0; i < sizeMatrix; i++) {
				tabNodeValues[i] = vector<int>(pow(2, sizeMatrix)-1);
				possibleRouteTab[i] = vector<int>(pow(2, sizeMatrix)-1);
			}

			int timeType = askTime();

			cout << "Podaj ile razy ma zostac wykonany algorytm: ";
			cin >> amount;

			for (int j = 0; j < amount; j++) {

				czas.czasStart();
				startDynamic(min, TSPMatrix, sizeMatrix, tabNodeValues, possibleRouteTab,bestTab);
				czas.czasStop();

				time = getTime(czas,timeType);

				cout << "\nMinimalna funkcja celu = " << min;
				cout << "\nNajlepsza droga: ";
				for (int i = 0; i < sizeMatrix; i++)
					cout << bestTab[i] << " ";
				cout << endl;

				saveToFile(instanceName, "Dynamiczne", time, min);
			}
		}
		break;


		case 8:
			showMatrix(TSPMatrix, sizeMatrix);
			break;



		case 9:
			system("pause");
			break;

		case 10:
		{
			int odp;
			int odp1, valMin, valMax, instSize, instSizeMin;
			int **matrix;
			int *tab;
			int *bestTab;
			int min;
			double time;
			Czas czas1;
			cout << "TESTY 1) BRUTE FORCE 2) BRANCH BOUND 3) PROGR. DYNAMICZNE: ";
			cin >> odp;
			switch (odp) {
			case 1:
			{

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
							time = time;
						}
						if (k >= 7 && k <= 10)
							time = getTime(czas1, 2);

						if (k > 10) {
							time = getTime(czas1, 1);
							time = time;
						}

						saveToFileRand("rand", k, i, "BruteForce-swap", time, min, valMin, valMax);


					}
				}
				break;
			}

			case 2:
			{

				int minimum;
				int *route;
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
						/*
						for (int i = 0; i < k; i++) {
							for (int j = 0; j < k; j++)
								cout << matrix[i][j] << " ";
							cout << endl;
						}
						*/

						start1.setMatrix(matrix, k);
						//-----------------------------------
						tab = new int[k];
						bestTab = new int[k];
						min = INT_MAX;

						czas1.czasStart();
						minimum = BBMain(start1, "rand", route, odpBB);
						czas1.czasStop();

						if (odpBB == 1) {

							if (k < 7) {
								time = getTime(czas1, 3);
								time = time;
							}
							if (k >= 7 && k <= 19)
								time = getTime(czas1, 2);

							if (k > 19) {
								time = getTime(czas1, 1);
								time = time;
							}


							saveToFileRand("rand", k, i, "Branch&Bound DF", time, minimum, valMin, valMax);
						}
						if (odpBB == 2) {

							if (k < 7) {
								time = getTime(czas1, 3);
								time = time;
							}
							if (k >= 7 && k <= 19)
								time = getTime(czas1, 2);

							if (k > 19) {
								time = getTime(czas1, 1);
								time = time;
							}

							saveToFileRand("rand", k, i, "Branch&Bound BF", time, minimum, valMin, valMax);
						}

						
						delete[]route;
						delete[]tab;
						delete[]bestTab;

						for (int i = 0; i < k; i++) {
							delete[]matrix[i];
						}
						delete[]matrix;
					}
				}
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
						/*
						for (int i = 0; i < k; i++) {
							for (int j = 0; j < k; j++)
								cout << matrix[i][j] << " ";
							cout << endl;
						}
						*/
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
							time = time;
						}

						if (k >= 9 && k<18) {
							time = getTime(czas1, 2);
							time = time;
						}

						if (k >= 18) {
							time = getTime(czas1, 1);
							time = time;
						}


						saveToFileRand("rand", k, i, "Dynamiczne", time, min, valMin, valMax);


					}
				}
				break;
			}
			}
		}
		}
	} while (answer != 9);

	return 0;
}
