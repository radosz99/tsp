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


int dynamicProgramming(int startV, int set, int N, int **array, vector<vector<int>>&g, vector<vector<int>>&nxt) {
		int min = INT_MAX, newSubset, bitMask, tempMin;

		if (g[startV][set] != -1) {	// zabezpieczenie przed kolejnym obliczaniem juz wyliczonej wartosci dla podproblemu
			return g[startV][set];
		}
		else {
			for (int i = 0; i < N; i++) {
				bitMask = (int)pow(2, N) - 1 - (int)pow(2, i);	// maska pozwalajaca na 'odklejanie' kolejnych elementow z aktualnego podzbioru i tworzenie mniejszych
				newSubset = set & bitMask;	// tworzenie maski bitowej: 0 - juz wykorzystane, 1 - odpowiadajace wierzcholki wchodza w sklad podzbioru
				if (newSubset != set) {
					tempMin = array[startV][i] + dynamicProgramming(i, newSubset, N, array, g, nxt);	// c(start,x) + g(x,S-{x}) = Cij + G(j, S-{j})
					if (tempMin < min) {	// funkcja minimalizujaca - przyrownanie z dotychczasowa najmniejsza wartoscia
						min = tempMin;
						nxt[startV][set] = i;
					}
				}
			}
		}

		g[startV][set] = min;
		return min;
	}

void initDP(int &min, int **array, int N, vector<vector<int>>&g, vector<vector<int>>&nxt) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < pow(2, N); j++) {
				g[i][j] = -1;
				nxt[i][j] = 1;
			}
		}

		for (int i = 0; i < N; i++) {
			g[i][0] = array[i][0];
		}

		min = dynamicProgramming(0, pow(2, N) - 2, N, array,g,nxt);

		cout << "\nMinimalna funkcja celu = " << min;
	}
