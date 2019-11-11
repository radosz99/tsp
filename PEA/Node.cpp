#include <istream>
#include <string>
#include <vector>
#include <fstream>
#include "Node.H"

Node::Node() {
}

Node::Node(int a, int b, int c, int d, int **macierz, int e, int *f) {
	index = a;
	id = b;
	value = c;
	lvl = d;
	size = e;
	matrix = new int *[size];
	route = new int[size];
	for (int i = 0; i < size; i++) {
		route[i] = f[i];
		route[lvl] = id;
		matrix[i] = new int[size];
		for (int j = 0; j < size; j++)
			matrix[i][j] = macierz[i][j];
	}

}

Node::Node(int *a, int*b, int c, int d, int e, int *f) {
	//cout << "Dajemy d = " << d << endl;
	size = d;
	route = new int[size];
	visited = new int[size];
	beforeVisited = new int[size];
	id = e;
	lvl = c;
	for (int i = 0; i < d; i++) {
		route[i] = a[i];
		visited[i] = b[i];
		beforeVisited[i] = f[i];
	}
}

Node::~Node() {
}

void Node::setIndex(int i) {
	index = i;
}

int Node::getIndex() {
	return index;
}

int Node::getId() {
	return id;
}

void Node::setId(int a) {
	id = a;
}


int Node::getValue() {
	return value;
}

int Node::getLvl() {
	return lvl;
}

int *Node::getRoute() {
	return route;
}

int *Node::getBeforeVisited() {
	return beforeVisited;
}

int *Node::getVisited() {
	return visited;
}

string Node::getInstanceName() {
	return instanceName;
}

int Node::getSize() {
	return size;
}

int **Node::getMatrix() {
	return matrix;
}

void Node::loadInfo() {
	string name;
	ifstream plik;
	bool ifOpen = false;

	while (ifOpen == false) {
		cout << "Wczytaj nazwe pliku z macierza: ";
		cin >> name;
		name = name + ".txt";

		plik.open("C:/Users/Radek/source/repos/PEA/Input/" + name);

		if (plik.good() == true)
		{
			cout << "\nUzyskano dostep do pliku!" << std::endl;
			ifOpen = true;
			plik >> instanceName;
			plik >> startMatrixSize;

			startMatrix = new int *[startMatrixSize];
			for (int i = 0; i < startMatrixSize; i++)  // dla ka¿dego wiersza macierzy:
				startMatrix[i] = new int[startMatrixSize];  // utworzenie i-tej tablicy typu int

			for (int i = 0; i < startMatrixSize; i++) {
				for (int j = 0; j < startMatrixSize; j++) {
					plik >> startMatrix[i][j];
					cout << startMatrix[i][j] << " ";
				}
				cout << endl;
			}

		}
		else cout << "Dostep do pliku zostal zabroniony!" << endl;
	}
	plik.close();
}




