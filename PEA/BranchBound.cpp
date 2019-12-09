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
#include "BruteForce.h"
#include "Node.h"
#include <vector>
#include "BranchBound.h"

using namespace std;

int reduceMatrix(int **matrix, int size) {

	int *row, *col;
	int min = INT_MAX;
	bool done = false;
	row = new int[size];
	col = new int[size];

	for (int i = 0; i < size; i++) {
		row[i] = min;
		done = false;
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] != -1 && matrix[i][j] < row[i]) {
				row[i] = matrix[i][j];
				done = true;
			}
			if ((j == (size - 1)) && !done) {
				row[i] = 0;
			}
		}
		//cout << "najmniejsza wartosc do zredukowania w wierszu " << i << "to "<<row[i] << endl;
		//min = 999999;
	}
	//odjecie zredukowanych wierszy
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (matrix[i][j] != -1) {
				matrix[i][j] = matrix[i][j] - row[i];
			}
		}
	}
	//cout << endl << "Otow po zredukowaniu wierszy:" << endl;
	//printMatrix(matrix);
	//redukcja kolumn
	for (int i = 0; i < size; i++) {
		col[i] = min;
		done = false;
		for (int j = 0; j < size; j++) {
			if (matrix[j][i] != -1 && matrix[j][i] < col[i]) {
				col[i] = matrix[j][i];
				done = true;
			}
			if ((j == (size - 1)) && !done) {
				col[i] = 0;
			}
		}
		//cout << "najmniejsza wartosc do zredukowania w kolumnie " << i << "to " << col[i] << endl;
		//min = 999999;
	}
	//odjecie zredukowanych kolumn
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (matrix[j][i] != -1) {
				matrix[j][i] = matrix[j][i] - col[i];
			}
		}
	}
	min = 0;
	//cout << endl << "Otow po zredukowaniu kolumn:" << endl;
	//printMatrix(matrix);
	for (int i = 0; i < size; i++) {
		min = min + row[i] + col[i];
	}
	delete[] row;
	delete[] col;
	return min;
}

void suitableRowColToInf(int **matrix, int row, int col, int size) {
	for (int i = 0; i < size; i++) {
		matrix[row][i] = -1;
		matrix[i][col] = -1;
	}
	matrix[col][0] = -1;
}

int getFirstUpperBound(int *bestTab, int size, int &helpMin, int **macierz,int **mainMacierz,int *visitedTab, int &tempMin, int *routeTab, int &savedBestCol, int &nodesAmount,vector<Node>& graph,int &deleteNodesAmount) {
	int bestMin, counter = 0;


	for (int i = 0; i < size - 1; i++) {
		bestMin = INT_MAX;
		//----------------------------------------------------------------------
		//Dla pierwszego poziomu (i=0) przepisanie macierz do mainMacierz
		//z ktorej macierz bedzie pozniej odtwarzana
		//----------------------------------------------------------------------
		if (i == 0) {
			for (int k = 0; k < size; k++)
				for (int l = 0; l < size; l++)
					mainMacierz[k][l] = macierz[k][l];
		}

		//----------------------------------------------------------------------
		//Petla przechodzaca po calej visitedTab i liczaca wartosci synow rozwijanego wierzcholka
		//----------------------------------------------------------------------
		for (int j = 1; j < size; j++) {
			if (visitedTab[j] != -1) {
				//----------------------------------------------------------------------
				//Wzor na lower bound = lower bound ojca + przejscie z ojca do syna + koszt zredukowania macierzy
				//----------------------------------------------------------------------
				suitableRowColToInf(macierz, routeTab[i], j, size); //ustawienie wartosci w odpowiednim wierszu i kolumnie na -1
				tempMin = reduceMatrix(macierz, size); //zredukowanie macierzy
				tempMin += mainMacierz[routeTab[i]][j]; //dodanie przejscia

				if (tempMin < bestMin) { //jako ze wszyscy synowie maja ten sam lower bound ojca to nie jest tu uwzgledniany
					bestMin = tempMin; //najlepszy lower bound (bez uwzglednienia lower bound ojca) jest zapisywany do bestMin
					savedBestCol = j; //id wierzcholka z bestMin jest zapisywane do savedBestCol
				}
				Node newNode(counter, j, helpMin + tempMin, i + 1, macierz, size, routeTab);
				counter++;
				nodesAmount++;
				graph.push_back(newNode);
				for (int k = 0; k < size; k++)
					for (int l = 0; l < size; l++)
						macierz[k][l] = mainMacierz[k][l];


			}
		}
		visitedTab[savedBestCol] = -1; //w tabeli wizyt oznaczamy wierzcholek o id savedBestCol na odwiedzony
		routeTab[i + 1] = savedBestCol; //w tabeli drogi komorka o nastepnym indeksie ma id wierzcholka

		//----------------------------------------------------------------------
		//Usuniecie najlepszego syna
		//----------------------------------------------------------------------
		for (unsigned int j = 0; j < graph.size(); j++)
			if (savedBestCol == graph[j].getId() && i + 1 == graph[j].getLvl()) {
				graph.erase(graph.begin() + j);
				deleteNodesAmount++;
			}

		helpMin = bestMin + helpMin; // ustalenie lower bound

		//----------------------------------------------------------------------
		//Przypisujemy wartosci zredukowanej macierzy najlepszego syna
		//----------------------------------------------------------------------
		suitableRowColToInf(macierz, routeTab[i], savedBestCol, size);
		reduceMatrix(macierz, size);
		for (int k = 0; k < size; k++)
			for (int l = 0; l < size; l++)
				mainMacierz[k][l] = macierz[k][l];

	}
	for (int i = 0; i < size; i++)
		bestTab[i] = routeTab[i];
	//printVector(graph, size);
	return helpMin;
}

int graphTidying(vector<Node>& graph, int &tempLevel, int&deleteNodesAmount, int min, int &index, int odp){
	int helpMin = min;
	int betterNodeId;
	//----------------------------------------------------------------------
	//Uporzadkowanie atrybutow index, by zgadzaly sie z rzeczywista kolejnoscia
	//----------------------------------------------------------------------
	for (unsigned int i = 0; i < graph.size(); i++)
		graph[i].setIndex(i);

	//----------------------------------------------------------------------
	//Znalezienie indeksu wierzcholka ktory bedzie rozwijany
	//sprawdzamy w poziomie o jeden wyzszym, jesli jest znajdowany wierzcholek o lower bound mniejszym niz upper bound
	//to w zakresie poziomu sprawdzamy czy ktorys z wierzcholkow ma jeszcze mniejszy lower bound
	//to jego bedziemy sprawdzac
	if (odp == 1) {
		//----------------------------------------------------------------------
		//WERSJA DEPTH FIRST
		//----------------------------------------------------------------------

		for (unsigned int i = graph.size(); i > 0; i--)
			if (graph[i - 1].getValue() < min) {
				betterNodeId = i - 1;
				tempLevel = graph[i - 1].getLvl();

				/*
				for (int j = i - 1; graph[j].getLvl() == graph[i - 1].getLvl() && j>=0; j--) { // w obrebie jednego poziomu wybieramy najlepszy wierzcholek
					if (graph[j].getValue() < graph[i - 1].getValue() && (graph[j].getLvl() == tempLevel)) {
						betterNodeId = graph[j].getIndex();
					}
				}*/

				break;
			}
	}

	if (odp == 2) {
		//---------------------------------------------------------------------
		//WERSJA BEST FIRST
		//----------------------------------------------------------------------
		for (unsigned int i = 0; i < graph.size(); i++)
			if (graph[i].getValue() < helpMin) {
				betterNodeId = i;
				helpMin = graph[i].getValue();
				tempLevel = graph[i].getLvl();
			}
	}
	//----------------------------------------------------------------------
	//Usuniecie wierzcholkow o lower bound wiekszym lub rownym obecnemu upper bound
	//----------------------------------------------------------------------
	for (unsigned int i = 0; i < graph.size(); i++)
		if (graph[i].getValue() >= min) {
			graph.erase(graph.begin() + i);
			deleteNodesAmount++;
			i = i - 1;
		}
	//----------------------------------------------------------------------
	//Wyznaczenie indeksu  w wektorze rozwijanego wierzcholka
	//----------------------------------------------------------------------
	for (unsigned int i = 0; i < graph.size(); i++)
		if (graph[i].getIndex() == betterNodeId)
			index = i;

	for (unsigned int i = 0; i < graph.size(); i++)
		graph[i].setIndex(i);

	return index;
}

void prepareNextIteration(int &helpMin, vector<Node>& graph, int size, int *visitedTab, int *routeTab, int index, int **macierz, int **mainMacierz, int &tempLevel, int&counter, int &deleteNodesAmount) {
	int **helpMacierz, *helpTab1;
	helpMin = graph[index].getValue();

	for (int i = 0; i < size; i++)
		visitedTab[i] = i;

	helpTab1 = graph[index].getRoute();
	helpMacierz = graph[index].getMatrix();
	//----------------------------------------------------------------------
	//Ustalenie wartosci routeTab na routeTab z poprzedniego wierzcholka
	//i w visitedTab ustalenie odwiedzonych pozycji na -1
	//----------------------------------------------------------------------
	for (int i = 0; i < size; i++) {
		routeTab[i] = helpTab1[i];
		visitedTab[routeTab[i]] = -1;
		for (int j = 0; j < size; j++) {
			macierz[i][j] = helpMacierz[i][j];
			mainMacierz[i][j] = helpMacierz[i][j];
		}
	}

	tempLevel = graph[index].getLvl(); //wyluskanie poziomu grafu z ktorego bedziemy rozwijac 
	counter = graph.size(); //pomocnicza zmienna zapewniajaca unikalnosc atrybutu index nowych wierzcholkow
	graph.erase(graph.begin() + index); //usuniecie wierzcholka ktory bedziemy rozwijac - jego odpowiednie atrybuty zostaly juz zapisane
	deleteNodesAmount++;

	delete[]helpTab1;
	for (int i = 0; i < size; i++) {
		delete[]helpMacierz[i];
	}
	delete[]helpMacierz;
}

void developingGraph(int min, int tempLevel, int size, bool &ifBetter, int &bestMin, int *visitedTab, int **macierz, int **mainMacierz, int *routeTab, int savedBestCol, int &tempMin, int &helpMin, int counter, vector<Node>& graph, int &nodesAmount, int &deleteNodesAmount) {
	for (int i = tempLevel; i < size - 1; i++) {
		if (ifBetter == false) //ifBetter bedzie ustawione na false jesli wartosci wszystkich synow rozwijanego wierzcholka sa wieksze od min (upper bound)
			break;
		bestMin = INT_MAX;
		ifBetter = false;

		//----------------------------------------------------------------------
		//Petla przechodzaca po calej visitedTab i liczaca wartosci synow rozwijanego wierzcholka
		//----------------------------------------------------------------------
		for (int j = 1; j < size; j++) {
			if (visitedTab[j] != -1) {
				//----------------------------------------------------------------------
				//Wzor na lower bound = lower bound ojca + przejscie z ojca do syna + koszt zredukowania macierzy
				//----------------------------------------------------------------------
				suitableRowColToInf(macierz, routeTab[i], j, size); //ustawienie wartosci w odpowiednim wierszu i kolumnie na -1
				tempMin = reduceMatrix(macierz, size); //zredukowanie macierzy
				tempMin += mainMacierz[routeTab[i]][j]; //dodanie przejscia

				if (tempMin < bestMin) { //jako ze wszyscy synowie maja ten sam lower bound ojca to nie jest tu uwzgledniany
					bestMin = tempMin; //najlepszy lower bound (bez uwzglednienia lower bound ojca) jest zapisywany do bestMin
					savedBestCol = j; //id wierzcholka z bestMin jest zapisywane do savedBestCol
				}
				//cout << "Lower bound dla j " << j << " = " << helpMin + tempMin << endl;
				if (helpMin + tempMin < min) { //jesli lower bound sprawdzanego syna jest mniejszy od obecnego upper bound to tworzymy wierzcholek
					ifBetter = true; //zmierzamy w glab bo utworzylismy przynajmniej jeden obiecujacy wierzcholek (depth first)
					Node newNode(counter, j, helpMin + tempMin, i + 1, macierz, size, routeTab);
					counter++;
					nodesAmount++;
					graph.push_back(newNode);
				}

				for (int k = 0; k < size; k++) //odtworzenie macierzy starego
					for (int l = 0; l < size; l++)
						macierz[k][l] = mainMacierz[k][l];
			}
		}
		//----------------------------------------------------------------------
		//Po opcjonalnym utworzeniu synow do wartosci mainMacierz przypisujemy wartosci zredukowanej macierzy najlepsz syna
		//----------------------------------------------------------------------
		if (ifBetter == false) {//jesli nie utworzylismy zadnych synow to wracamy do poczatku petli while
			ifBetter = true;
			helpMin = INT_MAX;
			break;
		}

		suitableRowColToInf(macierz, routeTab[i], savedBestCol, size);
		reduceMatrix(macierz, size);
		for (int k = 0; k < size; k++)
			for (int l = 0; l < size; l++)
				mainMacierz[k][l] = macierz[k][l];

		visitedTab[savedBestCol] = -1; //w tabeli wizyt oznaczamy wierzcholek o id savedBestCol na odwiedzony
		routeTab[i + 1] = savedBestCol; //w tabeli drogi komorka o nastepnym indeksie ma id wierzcholka

		//printVector(graph, matrixSize);

		//----------------------------------------------------------------------
		//Usuniecie najlepszego syna
		//----------------------------------------------------------------------
		for (unsigned int j = 0; j < graph.size(); j++)
			if (savedBestCol == graph[j].getId() && (i + 1) == graph[j].getLvl()) {
				graph.erase(graph.begin() + j);
				deleteNodesAmount++;
			}
		helpMin = bestMin + helpMin; //ustalenie obecnej wartosci lower bound
	}
}


int BBMain(Node start, string instanceName, int *bestTab, int odp) {
	vector<Node> graph;
	int nodesAmount = 0, deleteNodesAmount = 0;
	int betterNodeId;//zmienna przechowujaca id wierzcholka ktory powinien byc rozwijany
	int matrixSize = start.getStartSize();
	int tempLevel;//zmienna przechowujaca poziom na ktorym jestesmy
	int counter = 0;//pomocnicza zmienna zapewniajaca unikalnosc atrybutu index nowych wierzcholkow
	int min = INT_MAX, tempMin = 0, helpMin = 0, bestMin;
	int savedBestCol;//zmienna przechowujaca id wierzcholka z najlepszym lower bound
	//int *bestTab = new int[matrixSize];//tablica przechowujaca obecna najlepsza droge
	int *visitedTab = new int[matrixSize];//tablica odwiedzonych wierzcholkow
	int *routeTab = new int[matrixSize];//tablica drogi
	int **macierz = new int *[matrixSize]; //macierz do operacji
	int **mainMacierz = new int *[matrixSize]; //ta sama macierz, ale do odtwarzania
	int counterek = 0, index;
	bool ifBetter;
	for (int i = 0; i < matrixSize; i++) {
		visitedTab[i] = i; //tabela sluzaca do przechowywania informacji o tym czy dany wierzcholek jest juz odwiedzony
		routeTab[i] = 0; //tabela sluzaca do przechowywania sciezki
		macierz[i] = new int[matrixSize];
		mainMacierz[i] = new int[matrixSize];
	}
	start.copyMatrix(macierz); //zapisanie do zmiennej macierz macierzy wczytanej z pliku

	routeTab[0] = 0; //routeTab to sciezka, pierwszy w sciezce bedzie wierzcholek nr 0
	helpMin = reduceMatrix(macierz, matrixSize);

	min = getFirstUpperBound(bestTab, matrixSize, helpMin, macierz, mainMacierz, visitedTab, tempMin, routeTab, savedBestCol, nodesAmount, graph, deleteNodesAmount);

	betterNodeId = graphTidying(graph, tempLevel, deleteNodesAmount, min, index,odp);

	//----------------------------------------------------------------------
	//				ZAPETLENIE-DO CALKOWITEGO USUNIECIA GRAFU
	//----------------------------------------------------------------------

	while (!graph.empty()) {
		counterek++;
		if (counterek % 100 == 0) {
			//cout << "Current min = " << min << ", iteration #" << counterek << ", " << nodesAmount << " nodes checked and " << nodesAmount - deleteNodesAmount << " still exist." << endl;
		}

		prepareNextIteration(helpMin, graph, matrixSize, visitedTab, routeTab, index, macierz, mainMacierz, tempLevel, counter, deleteNodesAmount);
		ifBetter = true; //pomocnicza zmienna logiczna okreslajaca czy z danego wierzcholka wychodza wierzcholki obiecujace

		//----------------------------------------------------------------------
		//Petla rozwijajaca wierzcholek az do liscia chyba ze ifbetter==false
		//----------------------------------------------------------------------
		developingGraph(min, tempLevel, matrixSize, ifBetter, bestMin, visitedTab, macierz, mainMacierz, routeTab, savedBestCol, tempMin, helpMin, counter, graph, nodesAmount, deleteNodesAmount);

		if (min > helpMin) {
			//cout << "nowe min = " << min << endl;
			min = helpMin; //opcjonalne ustawienie nowej wartosci upper bound
			for (int i = 0; i < matrixSize; i++)
				bestTab[i] = routeTab[i];
		}

		betterNodeId = graphTidying(graph, tempLevel, deleteNodesAmount, min, index,odp);
	}
	//----------------------------------------------------------------------


	//cout << "\nUtworzonych wierzcholkow: " << nodesAmount;
	//cout << "\nUsunietych wierzcholkow: " << deleteNodesAmount;
	//cout << "\nLacznie iteracji: " << counterek;
	delete[]visitedTab;
	delete[]routeTab;

	for (int i = 0; i < matrixSize; i++) { 
		delete[]macierz[i];
		delete[]mainMacierz[i];
	}
	delete[]macierz;
	delete[]mainMacierz;
	graph.clear();
	return min;
}