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
#include "BruteForceFunc.h"
#include "BBFunc.h"
using namespace std;


int main()
{
	int answer;
	int sizeMatrix;
	int **TSPMatrix;
	Node start;

	start.loadInfo();
	sizeMatrix = start.getStartSize();
	TSPMatrix = new int *[sizeMatrix];
	for (int i = 0; i < sizeMatrix; i++)
		TSPMatrix[i] = new int[sizeMatrix];
	start.copyMatrix(TSPMatrix);

	do {
		answer = showMenu();
		Czas czas;
		system("cls");

		switch (answer) {

		case 1:
		{
			start.loadInfo();
			sizeMatrix = start.getStartSize();
			TSPMatrix = new int *[sizeMatrix];
			for (int i = 0; i < sizeMatrix; i++)
				TSPMatrix[i] = new int[sizeMatrix];
			start.copyMatrix(TSPMatrix);
			break;
		}

		case 2:
		{
			int *knownPermu = new int[sizeMatrix];
			for (int i = 1; i <= sizeMatrix; i++) {
				cout << "Podaj ktory wierzcholek bedzie odwiedzony jako " << i << ": ";
				cin >> knownPermu[i - 1];
			}
			cout << "\nFunkcja celu dla zadanej drogi wynosi: " << calculate(knownPermu, sizeMatrix, TSPMatrix) << endl;
			break;
		}

		case 3:
			double czasSek; 
			int *bestPermut;
			int min;
			bestPermut = new int[sizeMatrix];
			czas.czasStart();

			min=getBestPermu(sizeMatrix, TSPMatrix, bestPermut);

			czas.czasStop();
			czasSek = czas.czasWykonania();
			czasSek = czasSek / 1000000;
			printf("\nCzas wykonania algorytmu: %.8gs", czasSek);
			cout << "\nMinimalna funkcja celu = " << min << endl;
			cout << "Najlepsza droga: ";
			for (int i = 0; i < sizeMatrix; i++)
				cout << bestPermut[i] << " ";
			cout << endl << endl;
			break;

		case 4:
		{
			int *tab = new int[start.getStartSize()];
			int *bestTab = new int[start.getStartSize()];
			int min = 99999;
			for (int i = 0; i < start.getStartSize(); i++)
				tab[i] = i;
			czas.czasStart();
			permute(tab, bestTab, 0, start.getStartSize() - 1, min, sizeMatrix, TSPMatrix);
			czas.czasStop();
			double czasSek;
			czasSek = czas.czasWykonania();
			czasSek = czasSek / 1000000;
			printf("\nCzas wykonania algorytmu: %.8gs", czasSek);
			cout << "\nMinimalna funkcja celu = " << calculate(bestTab,sizeMatrix,TSPMatrix) << endl;
			cout << "\Najlepsza droga: ";
			for (int i = 0; i < start.getStartSize(); i++)
				cout << bestTab[i] << " ";
			cout << endl << endl;

			break;
		}

		case 5:
		{
		
			int nodesAmount = 0;
			int matrixSize = start.getStartSize();
			int *bestTab = new int[start.getStartSize() ];
			int min = 9999;

			czas.czasStart();
			
			tree(nodesAmount, matrixSize, bestTab, min, TSPMatrix);
			czas.czasStop();
			double czasSek;
			czasSek = czas.czasWykonania();
			czasSek = czasSek / 1000000;
			printf("\nCzas wykonania algorytmu: %.8gs", czasSek);
			cout << "\nMinimalna funkcja celu = " << min << endl;
			cout << "Najlepsza droga: ";
			for (int i = 0; i < matrixSize; i++)
				cout << bestTab[i] << " ";
			cout << "\nUtworzonych wierzcholkow = " << nodesAmount;
			cout << endl << endl;
			break;
		}

		case 6:
		{
			vector<Node> graph;
			int nodesAmount = 0, deleteNodesAmount = 0;
			int betterNodeId;//zmienna przechowujaca id wierzcholka ktory powinien byc rozwijany
			int matrixSize = start.getStartSize();
			int tempLevel;//zmienna przechowujaca poziom na ktorym jestesmy
			int counter = 0;//pomocnicza zmienna zapewniajaca unikalnosc atrybutu index nowych wierzcholkow
			int min = INT_MAX, tempMin = 0, helpMin = 0, bestMin;
			int savedBestCol;//zmienna przechowujaca id wierzcholka z najlepszym lower bound
			int *bestTab = new int[matrixSize];//tablica przechowujaca obecna najlepsza droge
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
			czas.czasStart();

			min = getFirstValue(bestTab, matrixSize, helpMin, macierz, mainMacierz, visitedTab, tempMin, routeTab, savedBestCol, nodesAmount, graph, deleteNodesAmount);
			betterNodeId = graphTidying(graph, tempLevel, deleteNodesAmount, min, index);

			//----------------------------------------------------------------------
			//				ZAPETLENIE-DO CALKOWITEGO USUNIECIA GRAFU
			//----------------------------------------------------------------------

			while (graph.size() != 0) {
				counterek++;
				if (counterek % 100 == 0) {
					cout << "Current min = " << min << ", iteration #" << counterek << ", " << nodesAmount << " nodes checked and " << nodesAmount - deleteNodesAmount << " still exist." << endl;
				}

				prepareNextIteration(helpMin, graph, matrixSize, visitedTab, routeTab, index, macierz, mainMacierz, tempLevel, counter, deleteNodesAmount);
				ifBetter = true; //pomocnicza zmienna logiczna okreslajaca czy z danego wierzcholka wychodza wierzcholki obiecujace

				//----------------------------------------------------------------------
				//Petla rozwijajaca wierzcholek az do liscia chyba ze ifbetter==false
				//----------------------------------------------------------------------
				developingGraph(min, tempLevel, matrixSize, ifBetter, bestMin, visitedTab, macierz, mainMacierz, routeTab, savedBestCol, tempMin, helpMin, counter, graph, nodesAmount, deleteNodesAmount);

				if (min > helpMin) {
					min = helpMin; //opcjonalne ustawienie nowej wartosci upper bound
					for (int i = 0; i < matrixSize; i++)
						bestTab[i] = routeTab[i];
				}

				betterNodeId = graphTidying(graph, tempLevel, deleteNodesAmount, min, index);
			}
			//----------------------------------------------------------------------
			czas.czasStop();
			double czasSek;
			czasSek = czas.czasWykonaniaMili();
			czasSek = czasSek / 1000;
			/*czasSek = czas.czasWykonaniaBB();
			czasSek = czasSek / 1000000000; dla niektorych instancji*/
			printf("\nCzas wykonania algorytmu: %.8gs", czasSek);
			cout << "\nMinimalna funkcja celu = " << min;
			cout << "\nNajlepsza droga: ";
			for (int i = 0; i < matrixSize; i++)
				cout << bestTab[i] << " ";
			cout << "\nUtworzonych wierzcholkow: " << nodesAmount;
			cout << "\nUsunietych wierzcholkow: " << deleteNodesAmount;
			cout << "\nLacznie iteracji: " << counterek;
			cout << endl << endl;
			break;
		}

		case 7:
			showMatrix(TSPMatrix, sizeMatrix);
			break;
		
		case 8:
			getchar();
			return 0;
		}
	} while (answer != 10);
}
