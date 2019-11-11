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
#include "BruteForceFunc.h"
#include "BBFunc.h"
#include "DPFunc.h"
using namespace std;


int main()
{
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

			time = getTime(czas);
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
				permute(tab, bestTab, 0, start.getStartSize() - 1, min, sizeMatrix, TSPMatrix);
				czas.czasStop();

				time = getTime(czas);
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

			time = getTime(czas);
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

			cout << "Podaj ile razy ma zostac wykonany algorytm: ";
			cin >> amount;

			for (int i = 0; i < amount; i++) {
				czas.czasStart();
				minimum = BBMain(start, instanceName, route);
				czas.czasStop();

				time = getTime(czas);
				cout << "\nMinimalna funkcja celu = " << minimum;
				cout << "\nNajlepsza droga: ";
				for (int i = 0; i < sizeMatrix; i++)
					cout << route[i] << " ";
				cout << endl;
				saveToFile(instanceName, "Branch&Bound", time, minimum);
			}
			break;
		}

		case 7:
			showMatrix(TSPMatrix, sizeMatrix);
			break;
		
		case 8:
		{
			double time;
			int min=INT_MAX;
			vector<vector<int>> g;
			vector<vector<int>> nxt;
			int N;


			g = vector<vector<int>>(sizeMatrix);
			nxt = vector<vector<int>>(sizeMatrix);

			for (int i = 0; i < sizeMatrix; i++) {
				g[i] = vector<int>(pow(2, sizeMatrix));
				nxt[i] = vector<int>(pow(2, sizeMatrix));
			}

			cout << "Podaj ile razy ma zostac wykonany algorytm: ";
			cin >> N;

			for (int j = 0; j < N; j++) {
				czas.czasStart();
				initDP(min, TSPMatrix, sizeMatrix, g, nxt);
				czas.czasStop();

				time = getTime(czas);
				cout << endl;
				saveToFile(instanceName, "Dynamiczne", time, min);
			}
		}
		break;

		case 9:
			system("pause");
		}
	} while (answer != 9);

	return 0;
}
