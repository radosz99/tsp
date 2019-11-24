#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <chrono>
#include <limits.h>
#include "Czas.h"
#include "BruteForceFunc.h"
#include "BBFunc.h"
using namespace std;


int getMinimum(int firstNode, int set, int size, int **matrix, vector<vector<int>>&tabNodeValues, vector<vector<int>>&possibleRouteTab, int &c, int &bitMask, int &newSubset) {
		int min = INT_MAX, tempMin;
		c++;
		if (tabNodeValues[firstNode][set] != -1) {
			return tabNodeValues[firstNode][set];
		}
		else {
			for (int i= 0; i < size; i++) {
				bitMask = pow(2, size) - 1 - pow(2, i);
				newSubset = set & bitMask;
				if (newSubset != set) {
					tempMin = matrix[firstNode][i] + getMinimum(i, newSubset, size, matrix, tabNodeValues, possibleRouteTab,c,bitMask,newSubset);	// c(start,x) + g(x,S-{x}) = Cij + G(j, S-{j})
					if (tempMin < min) { //minimalizacja w zakresie podwywolania
						min = tempMin;
						possibleRouteTab[firstNode][set] = i;
						
					}
				}
			}	
		}
		tabNodeValues[firstNode][set] = min;
		return min;
	}

void getRoute(int start, int set, int size, vector<vector<int>>&possibleRouteTab,int*bestTab, int &c, int &bitMask, int &newSubset) {
	//cout << possibleRouteTab[start][set] << endl;
	//cout << "Set: " << set << endl;
	//cout << "start: " << start << endl;
	if (possibleRouteTab[start][set] == -1) {
		return;
	}
	int i = possibleRouteTab[start][set];
	bestTab[c] = i;
	c++;

	bitMask = pow(2, size) - 1 - pow(2, i);
	newSubset = set & bitMask;
	//cout << "Set: " << set << endl;
	//cout << "bitMask: " << bitMask << endl;
	//cout << "newSubset: " << newSubset << endl;
	getRoute(i, newSubset, size, possibleRouteTab,bestTab,c,bitMask,newSubset);
}

void startDynamic(int &min, int **matrix, int size, vector<vector<int>>&tabNodeValues, vector<vector<int>>&possibleRouteTab, int *bestTab) {
		for (int i = 0; i < size; i++)
			for (int j = 0; j < pow(2, size); j++) {
				tabNodeValues[i][j] = -1;
				possibleRouteTab[i][j] = -1;
			}

		for (int i = 0; i < size; i++) {
			tabNodeValues[i][0] = matrix[i][0];
		}

		int bitMask = 0, newSubset = 0, counter = 0;
		bestTab[0] = 0;

		min = getMinimum(0, pow(2, size) - 2, size, matrix, tabNodeValues, possibleRouteTab,counter,bitMask,newSubset);
		counter = 1;

		getRoute(0, pow(2, size) - 2, size, possibleRouteTab,bestTab,counter,bitMask,newSubset);
	}

