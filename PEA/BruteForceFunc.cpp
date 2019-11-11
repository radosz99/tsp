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
			permute(a, b, l + 1, r, min, size,matrix);
			swap(a[l], a[i]);
		}
	}
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


int reduceMatrix(int **matrix, int size) {

	int *row, *col;
	int min = INT_MAX;
	bool done = false;
	row = new int[size];
	col = new int[size];

	for (int i = 0; i < size; i++) {
		row[i] = min;
		done = false;
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] != -1 && matrix[i][j] < row[i]) {
				row[i] = matrix[i][j];
				done = true;
			}
			if ((j == (size - 1)) && !done) {
				row[i] = 0;
			}
		}
		//cout << "najmniejsza wartosc do zredukowania w wierszu " << i << "to "<<row[i] << endl;
		//min = 999999;
	}
	//odjecie zredukowanych wierszy
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] != -1) {
				matrix[i][j] = matrix[i][j] - row[i];
			}
		}
	}
	//cout << endl << "Otow po zredukowaniu wierszy:" << endl;
	//printMatrix(matrix);
	//redukcja kolumn
	for (int i = 0; i < size; i++) {
		col[i] = min;
		done = false;
		for (int j = 0; j < size; j++) {
			if (matrix[j][i] != -1 && matrix[j][i] < col[i]) {
				col[i] = matrix[j][i];
				done = true;
			}
			if ((j == (size - 1)) && !done) {
				col[i] = 0;
			}
		}
		//cout << "najmniejsza wartosc do zredukowania w kolumnie " << i << "to " << col[i] << endl;
		//min = 999999;
	}
	//odjecie zredukowanych kolumn
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (matrix[j][i] != -1) {
				matrix[j][i] = matrix[j][i] - col[i];
			}
		}
	}
	min = 0;
	//cout << endl << "Otow po zredukowaniu kolumn:" << endl;
	//printMatrix(matrix);
	for (int i = 0; i < size; i++) {
		min = min + row[i] + col[i];
	}
	delete[] row;
	delete[] col;
	return min;
}

void suitableRowColToInf(int **matrix, int row, int col, int size) {
	for (int i = 0; i < size; i++) {
		matrix[row][i] = -1;
		matrix[i][col] = -1;
	}
	matrix[col][0] = -1;
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
		cout << endl << endl;
		macierz = newGraph[i].getMatrix();
		/*
		cout << "Node matrix: " << endl;
		for (int i=0; i<matrixSize; i++){
			for (int j=0; j<matrixSize; j++)
				cout << macierz[i][j] << " ";
			cout << endl;
		}*/
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
			for (int i = 0; i < matrixSize; i++) {
				graphSize = graph.size();
				wskVisited = graph[graphSize - 1].getVisited();
				wskRoute = graph[graphSize - 1].getRoute();
				wskBeforeVisited = graph[graphSize - 1].getBeforeVisited();
				visitedNodesAmount = graph[graphSize - 1].getId();

				if (wskVisited[i] == 0) {
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
		graphSize = graph.size();
		graph.erase(graph.begin() + graphSize - 1);
		graphSize = graph.size();
		graph.erase(graph.begin() + graphSize - 1);

	}
}

void saveToFile(string a, string b, double c, int d) {
	ofstream plik;
	plik.open("C:/Users/Radek/source/repos/PEA/Input/test2.csv", std::ios_base::app);
	plik << a << ";" << b << ";" << c << ";" << d << endl;
	plik.close();
}

double getTime(Czas czas) {
	double czasSek;

	czasSek = czas.czasWykonaniaNano();
	czasSek = czasSek / 1000000000;
	printf("\nCzas wykonania algorytmu: %.9gs", czasSek);
		
	return czasSek;
}