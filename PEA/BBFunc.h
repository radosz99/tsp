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

#ifndef BB_H
#define BB_H


int getFirstUpperBound(int *bestTab, int size, int&helpMin, int **macierz, int **mainMacierz, int *visitedTab, int &tempMin, int *routeTab, int &savedBestCol, int &nodesAmount, vector<Node>& graph, int &deleteNodesAmount);
int graphTidying(vector<Node>& graph, int &tempLevel, int&deleteNodesAmount, int min, int &index, int odp);
void prepareNextIteration(int &helpMin, vector<Node>& graph, int size, int *visitedTab, int *routeTab, int index, int **macierz, int **mainMacierz, int &tempLevel, int&counter, int &deleteNodesAmount);
void developingGraph(int min, int tempLevel, int size, bool &ifBetter, int &bestMin, int *visitedTab, int **macierz, int **mainMacierz, int *routeTab, int savedBestCol, int &tempMin, int &helpMin, int counter, vector<Node>& graph, int &nodesAmount, int &deleteNodesAmount);
void tree(int &nodesAmount, int matrixSize, int *bestTab, int &min);
int BBMain(Node start, string instanceName, int *bestTab,int odp);
void suitableRowColToInf(int **matrix, int row, int col, int size);
int reduceMatrix(int **matrix, int size);
#endif#pragma once
#pragma once
#pragma once
