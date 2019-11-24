#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <string>

using namespace std;

class Node {
public:
	//Default constructor
	Node();

	//Overload constructor
	Node(int, int, int, int, int**, int, int*);
	Node(int*, int*, int, int, int, int*);

	//Destructor
	~Node();

	//Accessor Function
	void copyMatrix(int**);
	void setMatrix(int **, int);
	void loadInfo();
	int getStartSize();
	int getSize();
	string getInstanceName();
	int **getMatrix();
	int getIndex();
	int getId();
	void setId(int);
	int getValue();
	int getLvl();
	int *getRoute();
	int *getVisited();
	int *getBeforeVisited();
	void setIndex(int);
	/*void setInd(int);
	void setId(int);
	void setValue(int);
	void setLvl(int);
	void setRoute(int*);*/


private:
	//Member variables
	int size;
	int index;
	int id;
	int *beforeVisited;
	int *route;
	int *visited;
	int value;
	int lvl;
	int **matrix;
	int startMatrixSize;
	int **startMatrix;
	string instanceName;
};
#endif