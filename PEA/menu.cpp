#include <iostream>
#include "menu.h"
using namespace std;


int showMenu() {
	int ans;
	cout << "\n1. Wczytaj plik txt z macierza sasiedztwa" << endl;
	cout << "2. Dla zadanej permutacji oblicz funkcje celu" << endl;
	cout << "3. Oblicz optymalna sciezke (brute force - nextpermutation)" << endl;
	cout << "4. Oblicz optymalna sciezke (brute force - swap)" << endl;
	cout << "5. Oblicz optymalna sciezke (brute force - drzewo)" << endl;
	cout << "6. Oblicz optymalna sciezke (branch&bound)" << endl;
	cout << "7. Oblicz optymalna sciezke (dynamiczne programowanie)" << endl;
	cout << "8. Wyswietl macierz" << endl;
	cout << "9. EXIT" << endl;
	cout << "10. TESTY AUTOMATYCZNE" << endl;
	cin >> ans;

	return ans;
}