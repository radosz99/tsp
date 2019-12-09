#include <iostream>
#include "menu.h"
using namespace std;


int showMenu() {
	int ans;
	cout << "1. Wczytaj plik txt z macierza sasiedztwa" << endl;
	cout << "2. Wygeneruj losowy problem o danej wielkosci" << endl;
	cout << "3. Dla zadanej permutacji oblicz funkcje celu" << endl;
	cout << "4. Wyswietl macierz" << endl;
	cout << "5. Oblicz optymalna sciezke (brute force - nextpermutation)" << endl;
	cout << "6. Oblicz optymalna sciezke (brute force - swap)" << endl;
	cout << "7. Oblicz optymalna sciezke (brute force - drzewo)" << endl;
	cout << "8. Oblicz optymalna sciezke (branch&bound)" << endl;
	cout << "9. Oblicz optymalna sciezke (dynamiczne programowanie)" << endl;
	cout << "10. Oblicz (nie)optymalna sciezke (tabu search)" << endl;
	cout << "11. Oblicz (nie)optymalna sciezke (symulowane wyzarzanie)" << endl;
	cout << "12. TESTY AUTOMATYCZNE" << endl;
	cout << "13. EXIT" << endl;
	cin >> ans;

	return ans;
}