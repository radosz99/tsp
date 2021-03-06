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
#include <math.h>
#define M_E 2.71828182845904523536

using namespace std; 


void permute(int *a, int *b, int l, int r, int &min, int size, int **matrix)
{
	int value;
	if (l == r) {
		value = calculate(a,size,matrix);

		if (value < min) {
			for (int i = 0; i <= r; i++)
				b[i] = a[i];
			min = value;
		}
	}
	else
	{
		for (int i = l; i <= r; i++)
		{
			swap(a[l], a[i]);
			//cout << "Swapuje " << l << " z " << i << endl;
			permute(a, b, l + 1, r, min, size,matrix);
			swap(a[l], a[i]);
		}
	}
}

void showVector(vector <unsigned> vect) {
	for (int j = 0; j < vect.size(); j++)
		cout << vect.at(j) << " ";
	cout << endl;
}

int calculate(int* permut, int size, int **matrix) {

	int sum = 0;
	int i, j;
	for (int iter = 0; iter < size - 1; iter++) {
		i = permut[iter];
		j = permut[iter + 1];
		//cout << "I= " << i << " j = " << j << endl;
		sum += matrix[i][j];
	}
	sum = sum + matrix[j][permut[0]];
	return sum;
}

int getBestPermu(int size, int **matrix, int *bestPermut) {
	int min = INT_MAX;
	int *permut = new int[size];

	for (int i = 0; i < size; i++) {
		permut[i] = i;
	}

	do {
		if (calculate(permut,size,matrix) < min) {
			min = calculate(permut, size, matrix);
			for (int i = 0; i < size; i++)
				bestPermut[i] = permut[i];
		}
	} while (next_permutation(permut, permut + size));



	return min;
}


void showMatrix(int **matrix, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

void printVector(vector<Node>& newGraph, int a) {

	int size = newGraph.size();
	int matrixSize = a;
	int **macierz, *sciezka;

	for (int i = 0; i < size; i++) {
		//for(unsigned int i=0; i<newMyClass.size(); i++){ IF WE WANNA MODIFY CONTENT OF VECTOR
		cout << "Node index: " << newGraph[i].getIndex() << endl;
		cout << "Node id: " << newGraph[i].getId() << endl;
		cout << "Node value: " << newGraph[i].getValue() << endl;
		cout << "Node lvl: " << newGraph[i].getLvl() << endl;

		sciezka = newGraph[i].getRoute();
	
		cout << "Node route: ";
		for (int i = 0; i < matrixSize; i++)
			cout << sciezka[i] << "-";
		cout << endl;

	}
}

void tree(int &nodesAmount, int matrixSize, int *bestTab, int &min, int **TSPMatrix) {
	size_t graphSize;
	bool ifEnd = false;
	int tempLvl;
	int *wskRoute, *wskVisited, *wskBeforeVisited;
	int *route = new int[matrixSize];
	int *visited = new int[matrixSize];
	int *beforeVisited = new int[matrixSize];
	int visitedNodesAmount = 1;
	for (int i = 0; i < matrixSize; i++) {
		route[i] = 0;
		visited[i] = 0;
		beforeVisited[i] = 0;
	}
	visited[0] = 1;
	beforeVisited[0] = 1;
	int result;
	vector<Node> graph;


	Node newNode(route, visited, 0, matrixSize, visitedNodesAmount, beforeVisited);
	nodesAmount++;
	graph.push_back(newNode);
	graphSize = graph.size();

	while (graphSize != 0) {
		graphSize = graph.size();
		while (graph[graphSize - 1].getId() == matrixSize) {
			graph.erase(graph.begin() + graphSize - 1);
			graphSize = graph.size();
			if (graphSize < 1) {
				ifEnd = true;
				break;
			}
		}
		if (ifEnd == true)
			break;

		tempLvl = graph[graphSize - 1].getLvl();

		while (tempLvl != matrixSize) {
			tempLvl++;
			
			for (int i = 1; i < matrixSize; i++) {
				graphSize = graph.size();
				wskVisited = graph[graphSize - 1].getVisited();
				if (wskVisited[i] == 0) {
					wskRoute = graph[graphSize - 1].getRoute();
					wskBeforeVisited = graph[graphSize - 1].getBeforeVisited();
					visitedNodesAmount = graph[graphSize - 1].getId();

					for (int k = 0; k < matrixSize; k++) {
						route[k] = wskRoute[k];
						beforeVisited[k] = wskBeforeVisited[k];
					}
					route[tempLvl] = i;
					wskVisited[i] = 1;
					beforeVisited[i] = 1;
					visitedNodesAmount++;
					Node newNode(route, beforeVisited, tempLvl, matrixSize, graph[graphSize - 1].getLvl() + 2, beforeVisited);
					nodesAmount++;
					graph.push_back(newNode);
					graph[graphSize - 1].setId(visitedNodesAmount);
					break;
				}
			}
		}

		result = calculate(route, matrixSize, TSPMatrix);

		if (result < min) {
			min = result;
			for (int i = 0; i < matrixSize; i++)
				bestTab[i] = route[i];
		}
	}
}

void saveToFile(string a, string b, double c, string e,int d) {
	ofstream plik;
	plik.open("C:/Users/Radek/source/repos/PEA/Output/wyniki.csv", std::ios_base::app);
	plik << a << ";" << b << ";" << c << ";" << e << ";" <<d << endl;
	plik.close();
}

void saveToFileRand(string a, int k, int i, string b, double c, string g, int d, int e, int f) {
	ofstream plik;
	plik.open("C:/Users/Radek/source/repos/PEA/Output/wynikiTesty.csv", std::ios_base::app);
	plik << a << ";" << k << ";" << i << ";" << b << ";" << c << ";" << g << ";" << d << ";" << e << ";" << f << endl;
	plik.close();
}

double getTime(Czas czas, int odp) {
	double czasSek;

	switch (odp) {
	case 1:
		czasSek = czas.czasWykonaniaMili();
		break;
	case 2:
		czasSek = czas.czasWykonania();
		break;
	case 3:
		czasSek = czas.czasWykonaniaNano();
		break;
	case 4:
		czasSek = czas.czasWykonaniaSek();
		break;
	}
	return czasSek;
}

int askTime() {
	int odp = 0;
	while (odp < 1 || odp>3) {
		cout << "Liczyc w 1)milisekundach, 2)mikrosekundach, 3)nanosekundach: ";
		cin >> odp;
	}

	return odp;
}

void saveToFileGenetic(string a, double b, int c, vector<unsigned>& islandsBest, int e, int f, double g, int h, int i, int j, int k, int l, int m) {
	ofstream plik;
	plik.open("C:/Users/Radek/source/repos/PEA/Output/wynikiTestyGenetic.csv", std::ios_base::app);
	plik << a << ";" << b << ";" << "ms" << ";" << c << ";";
	for (int i = 0; i < islandsBest.size(); i++) {
		plik << islandsBest.at(i) << ";";
	}
	plik << e << ";" << f << ";" << g << ";" << h << ";" << i << ";" << j << ";" << k << ";" << l << ";" << m << ";";
	plik << endl;
	plik.close();
}

