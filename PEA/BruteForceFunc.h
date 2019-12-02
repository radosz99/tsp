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
void saveToFile(string a, string b, double c, string e, int d);
double getTime(Czas czas, int odp);
void saveToFileRand(string a, int k, int i, string b, double c, string g, int d, int e, int f);
int askTime();
void saveToFileTabu(int a, unsigned b, int c, int d, int e, bool i,int h);

#endif#pragma once
#pragma once
