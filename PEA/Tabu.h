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

#ifndef TABU_H
#define TABU_H

void getInitialReduction(vector < unsigned >&bestTab, int &helpMin, Node start);
void getInitialGreedy(vector < unsigned >&bestTab, int &helpMin, Node start);
void getInitialGreedyAndRandom(vector < unsigned >&bestTab, int &helpMin, Node start, int divider);
int Tabu(Node start, int matrixSize, int **TSPMatrix, int c, unsigned d, int e, int f, int g, bool alg);
int getInitialReductionAndRandom(vector < unsigned >&bestTab, int &helpMin, Node start, int divider);
#endif#pragma once

#pragma once
#pragma once
