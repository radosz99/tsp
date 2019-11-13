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

#ifndef DP_H
#define DP_H


int getMinimum(int startV, int set, int size, int **array, vector<vector<int>>&tabNodeValues, vector<vector<int>>&possibleRouteTab, int &c, int &bitMask, int &newSubset);
void startDynamic(int &min, int **array, int size, vector<vector<int>>&tabNodeValues, vector<vector<int>>&possibleRouteTab, int *bestTab);
void getRoute(int start, int set, int size, vector<vector<int>>&possibleRouteTab,int*bestTab, int &c, int &bitMask, int &newSubset);

#endif#pragma once
#pragma once
#pragma once
