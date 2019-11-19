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

#ifndef BRUTEFORCE_H
#define BRUTEFORCE_H

void permute(int *a, int *b, int l, int r, int &min, int size, int **matrix);
int calculate(int* permut, int size, int **matrix);
int getBestPermu(int size, int **matrix, int *bestPermut);
void printVector(vector<Node>& newGraph, int a);
void showMatrix(int **matrix, int size);
void tree(int &nodesAmount, int matrixSize, int *bestTab, int &min, int **TSPMatrix);
void saveToFile(string a, string b, double c, int d);
double getTime(Czas czas, int odp);
int askTime();

#endif#pragma once
#pragma once
