#include <iostream>
#include "menu.h"
using namespace std;


int showMenu() {
	int ans;
	cout << "1. Load .txt file as instance" << endl;
	cout << "2. Generate a random instance of the given size" << endl;
	cout << "3. Calculate the objective function for a given permutation" << endl;
	cout << "4. Show instance" << endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << "---------------------Exact-algorithms----------------------" << endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << "5. BF - brute force (nextpermutation)" << endl;
	cout << "6. BF - brute force (swap)" << endl;
	cout << "7. BF - brute force (tree)" << endl;
	cout << "8. B&B - Branch & Bound" << endl;
	cout << "9. DP - Dynamic Programming" << endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << "-----------------------Local-search------------------------" << endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << "10. TS - Tabu Search" << endl;
	cout << "11. SA - Simulated annealing" << endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << "--------------------Genetic-algorithms---------------------" << endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << "12. IMGA - Island Model Genetic Algorithm" << endl;
	cout << "-----------------------------------------------------------" << endl;
	cout << "13. AUTOMATIC TESTS (BF,BB,DP)" << endl;
	cout << "14. EXIT" << endl;
	cin >> ans;

	return ans;
}