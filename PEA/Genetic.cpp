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
#include "LocalSearch.h"
#include "Genetic.h"
#include <ctime>
#include <random>
#include <thread>
#include <queue>
#include <future>
using namespace std;

std::mutex muA;
std::mutex muB;

Genetic::Genetic() {
}

void Genetic::setSettingsGenetic(int a, int  b, double c, int d, int e, int f, int g, int h, int i) {
	populationSize = a;
	LocalSearch::amountRandomNodes = b;
	mutationProb = c;
	crossoverType = d;
	selectionType = e;
	mutationType = f;
	iterations = g;
	elitismDivider = h;
	timeGenetic = i;
}

int Genetic::costXY(int a, int b) {
	return LocalSearch::matrix[a][b];
}

int Genetic::GeneticMechanism(int a, int **TSPMatrix, int b, vector<unsigned>& islandsBest) {

	timeGenetic = b;
	int islandsAmount = 5;
	int result = INT_MAX;
	vector < vector<unsigned>> best;
	vector <unsigned> offspring1(a + 2, 0);
	for (int i = 0; i < islandsAmount; i++) {
		best.push_back(offspring1);
		islandsBest.push_back(0);
	}
	
	launchIslands(a, TSPMatrix, best, islandsBest);

	for (int i = 0; i < islandsAmount; i++) {
		if (best.at(i).at(a + 1) < result)
			result=best.at(i).at(a + 1);
	}

	return result;
}

void Genetic::launchIslands(int a, int **TSPMatrix, vector < vector<unsigned>>& best, vector<unsigned>& islandsBest) {
	Genetic island1;
	island1.setSettingsGenetic(300, 3, 0.15, 7, 1, 2, 50, 60,timeGenetic);
	auto a1 = std::async(std::launch::async, &Genetic::GeneticEngine, island1, a, TSPMatrix, 0, std::ref(best), std::ref(islandsBest));
	// populationSize, amountRandomNodes, mutationProb, crossoverType, selectionType, mutationType, iterations, elitism

	Genetic island2;
	island2.setSettingsGenetic(300, 3, 0.15, 7, 2, 1, 50, 50, timeGenetic);
	auto a2 = std::async(std::launch::async, &Genetic::GeneticEngine, island2, a, TSPMatrix, 1, std::ref(best), std::ref(islandsBest));

	Genetic island3;
	island3.setSettingsGenetic(300, 4, 0.15, 4, 2, 1, 50, 50, timeGenetic);
	auto a3 = std::async(std::launch::async, &Genetic::GeneticEngine, island3, a, TSPMatrix, 2, std::ref(best), std::ref(islandsBest));

	Genetic island4;
	island4.setSettingsGenetic(300, 4, 0.15, 1, 1, 2, 50, 50, timeGenetic);
	auto a4 = std::async(std::launch::async, &Genetic::GeneticEngine, island4, a, TSPMatrix, 3, std::ref(best), std::ref(islandsBest));

	Genetic island5;
	island5.setSettingsGenetic(300, 2, 0.15, 7, 1, 1, 50, 50, timeGenetic);
	auto a5 = std::async(std::launch::async, &Genetic::GeneticEngine, island5, a, TSPMatrix, 4, std::ref(best), std::ref(islandsBest));
}

void Genetic::GeneticEngine(int a, int **TSPMatrix, int islandId, vector < vector<unsigned>>& best, vector<unsigned>& islandBest) {

	islandBest.at(islandId) = INT_MAX;
	LocalSearch::matrixSize = a;
	matrixSize = a;
	LocalSearch::matrix = new int *[LocalSearch::matrixSize];
	for (int i = 0; i < LocalSearch::matrixSize; i++) {
		LocalSearch::matrix[i] = new int[LocalSearch::matrixSize];
		for (int j = 0; j < LocalSearch::matrixSize; j++)
			LocalSearch::matrix[i][j] = TSPMatrix[i][j];
	}

	vector < vector <unsigned>> population;
	vector <unsigned> parent1, parent2;
	vector <unsigned> offspring1(matrixSize + 2, 0);
	vector <unsigned> offspring2(matrixSize + 2, 0);
	vector <double> fitnesses(populationSize, 0);

	generateInitialPopulation(population, fitnesses);
	Czas onboardClock;
	bool continuing = true;
	onboardClock.start();

	//for (int j = 0; j < iterations; j++) {
	for (int j = 0; continuing==true; j++) {
		//cout << "Wyspa " << islandId << " iteracja " << j << endl;
		vector < vector <unsigned>> newPopulation;

		while (newPopulation.size() != populationSize) {
			doSelection(parent1, parent2, population, fitnesses);
			doCO(parent1, parent2, offspring1, offspring2);
			mutation(offspring1);
			memeticImprovement(offspring1);
			newPopulation.push_back(offspring1);
			if (offspring1.at(matrixSize + 1) < islandBest.at(islandId))
				islandBest.at(islandId) = offspring1.at(matrixSize + 1);

			if (crossoverType != 6 && crossoverType != 7) {
				mutation(offspring2);
				memeticImprovement(offspring2);
				newPopulation.push_back(offspring2);
				if (offspring2.at(matrixSize + 1) < islandBest.at(islandId))
					islandBest.at(islandId) = offspring2.at(matrixSize + 1);
			}
		}

		sortVector(newPopulation);

		if (j != 0 && j%5==0) {
			islandExchange(population, best, islandId);
		}

		overwritePopulation(population, newPopulation);
		sortVector(population);
		evaluatePopulation(population, fitnesses);	
		best.at(islandId)=population.at(0);

		onboardClock.stop();
		//-----sprawdzenie czy uplynal czas------
		if (onboardClock.read() > timeGenetic)
			continuing = false;

	}

	LocalSearch::optMin = population.at(0).at(matrixSize + 1);
	population.at(0).pop_back();
	population.at(0).pop_back();
	bestRoute = population.at(0);
}

vector <unsigned> Genetic::tournamentSelection(vector <vector <unsigned>> pop) {
	vector <unsigned> best;
	vector <unsigned> ind;

	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> indRand(0, pop.size() - 1);

	int k = 2;
	for (int i = 1; i <= k; i++) {
		if (i==1)
		best = pop.at(indRand(randomGen));

		else {
			ind = pop.at(indRand(randomGen));
			if (ind.at(matrixSize + 1) < best.at(matrixSize+1))
				best = ind;
		}
	}

	return best;
}

int Genetic::rouletteWheelSelection(vector <double> fitnesses) {

	double number, totalFitness = 0.0, offset = 0.0;
	int pick = 0;

	static_cast<double>(number = (double)rand() / (double)RAND_MAX);
	//cout << number << endl;

	for (int i = 0; i < fitnesses.size(); i++) {
		totalFitness += fitnesses.at(i);
	}
	for (int i = 0; i < fitnesses.size(); i++) {
		fitnesses.at(i) = fitnesses.at(1) / totalFitness;
	}

	for (int i = 0; i < fitnesses.size(); i++) {
		offset += fitnesses.at(i);
		if (number < offset) {
			pick = i;
			break;
		}
	}
	return pick;
}

int Genetic::rankSelection(vector <double> fitnesses, vector <vector <unsigned>> pop) {

	double number, totalProb = 0.0, offset = 0.0;
	double helpRank = populationSize;
	int pick = 0;

	static_cast<double>(number = (double)rand() / (double)RAND_MAX);
	//cout << number << endl;

	for (int i = 0; i < fitnesses.size(); i++) {
		fitnesses.at(i) = helpRank / (populationSize*(populationSize - 1));
		helpRank = helpRank - 1;
	}

	for (int i = 0; i < fitnesses.size(); i++) {
		totalProb += fitnesses.at(i);
	}
	for (int i = 0; i < fitnesses.size(); i++) {
		fitnesses.at(i) = fitnesses.at(1) / totalProb;
	}

	for (int i = 0; i < fitnesses.size(); i++) {
		offset += fitnesses.at(i);
		if (number < offset) {
			pick = i;
			break;
		}
	}
	return pick;
}

void Genetic::overwritePopulation(vector <vector<unsigned>>&population, vector <vector<unsigned>>popul) {
	for (int i = populationSize / elitismDivider+1; i < population.size(); i++) 
		population.at(i) = popul.at(i - populationSize / elitismDivider-1);
}

void Genetic::sortVector(vector <vector<unsigned>>&vect) {
	std::sort(vect.begin(), vect.end(),
		[&](const std::vector<unsigned>& a, const std::vector<unsigned>& b) {
		return a.at(matrixSize + 1) < b.at(matrixSize + 1);
	});
}

void Genetic::evaluatePopulation(vector <vector <unsigned>> popul, vector <double> &fitnesses) {
	double fitness, divider = 1;

	for (int i = 0; i < populationSize; i++) {
		static_cast<float>(fitness = divider / popul.at(i).at(matrixSize + 1));
		fitnesses.at(i) = fitness;
	}
}
void Genetic::mutation(vector <unsigned>& ind) {
	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> nodeRand(1, matrixSize - 1);
	int bestI = 0;
	int bestJ = 0;
	int bestBalance = INT_MAX;


	int i, j, balance = 0;
	if (static_cast<float>(rand()) / RAND_MAX < mutationProb) {
		if (mutationType == 0) {
			for (int k = 0; k < 2; k++) { //po 2 losowania, wybierane lepsze
				do {
					i = nodeRand(randomGen);
					j = nodeRand(randomGen);
				} while (i == j || j < i);

				LocalSearch::calculateSwap(i, j, balance, ind);
				if (balance < bestBalance) {
					bestI = i;
					bestJ = j;
					bestBalance = balance;
				}
			}
			LocalSearch::swapVector(bestI, bestJ, ind);
		}

		else if (mutationType == 2) {
			for (int k = 0; k < 2; k++) {
				do {
					i = nodeRand(randomGen);
					j = nodeRand(randomGen);
				} while (i == j || j < i);

				LocalSearch::calculateReverse(i, j, balance, ind);
				if (balance < bestBalance) {
					bestI = i;
					bestJ = j;
					bestBalance = balance;
				}
			}

			LocalSearch::reverseVector(bestI, bestJ, ind);
		}


		else if (mutationType == 1) {
			for (int k = 0; k < 2; k++) {
				do {
					i = nodeRand(randomGen);
					j = nodeRand(randomGen);
				} while (i == j - 1 || i == j || i == j + 1);

				LocalSearch::calculateInsert(i, j, balance, ind);
				if (balance < bestBalance) {
					bestI = i;
					bestJ = j;
					bestBalance = balance;
				}
			}
			LocalSearch::insertVector(bestI, bestJ, ind);
		}

		ind.at(matrixSize+1) += bestBalance;
	}
}

void Genetic::islandExchange(vector <vector<unsigned>>&population, vector < vector<unsigned>> best, int islandId) {
	bool cont = true;
	for (int i = 0; i < best.size(); i++)
		if (best.at(i).at(matrixSize + 1) == 0)
			cont = false;

	for (int i = 0; i < best.size(); i++) {
		for (int j = 0; j < best.size(); j++) {
			if (i != j && cont == true) {
				int helper = 0;
				if (j > islandId) helper = 1;
				population.at(populationSize / elitismDivider-best.size()+2+j-helper) = best.at(j);
			}
		}
	}
	/*
	if (islandId == 0 && cont == true)
		population.at(populationSize / elitismDivider) = best.at(1);

	if (islandId == 1 && cont == true)
		population.at(populationSize / elitismDivider) = best.at(0);

	if (islandId == 2 && cont == true)
		population.at(populationSize / elitismDivider) = best.at(0);

	if (islandId == 0 && cont == true)
		population.at(populationSize / elitismDivider - 1) = best.at(2);

	if (islandId == 1 && cont == true)
		population.at(populationSize / elitismDivider - 1) = best.at(2);

	if (islandId == 2 && cont == true)
		population.at(populationSize / elitismDivider - 1) = best.at(1);
		*/
}

void Genetic::memeticImprovement(vector <unsigned>& ind) {
	int bestBalance, bestI = 0, bestJ = 0, type = 1;
	//z/*
	if (type == 0) {
		bestBalance = getBestNeighborhoodSwap(bestI, bestJ, ind);
		swapVector(bestI, bestJ, ind);
	}

	if (type == 1) {
		bestBalance = getBestNeighborhoodInsert(bestI, bestJ, ind);
		insertVector(bestI, bestJ, ind);
	}

	if (type == 2) {
		bestBalance = getBestNeighborhoodReverse(bestI, bestJ, ind);
		reverseVector(bestI, bestJ, ind);
	}

	//*/
	/*
	int type = 0, i = 0, j = 0, balance = 0;

		bestBalance = getBestNeighborhoodSwap(i, j, ind);
		bestI = i;
		bestJ = j;
		type = 0;

	
		balance = getBestNeighborhoodInsert(i, j, ind);
		if (bestBalance > balance) {
			bestI = i;
			bestJ = j;
			type = 1;
			bestBalance = balance;

		}

		balance = getBestNeighborhoodReverse(i, j, ind);
		if (bestBalance > balance) {
			bestI = i;
			bestJ = j;
			type = 2;
			bestBalance = balance;
		}

		if (type == 0) 
			swapVector(bestI, bestJ, ind);
		else if(type==1)
			insertVector(bestI, bestJ, ind);
		else if(type==2)
			reverseVector(bestI, bestJ, ind);
//*/
	ind.at(matrixSize + 1) += bestBalance;
}

void Genetic::generateInitialPopulation(vector <vector <unsigned>>& pop, vector <double>& fitnesses) {

	vector < unsigned > route;
	route.push_back(getInitialReduction(route)); // 1 osobnik redukcyjnym
	pop.push_back(route);

	route.clear();
	route.push_back(getInitialGreedy(route)); // 1 osobnik zachlannym
	pop.push_back(route);

	for (int i = 2; i < populationSize / 2 + 1; i++) {
		vector < unsigned > route;
		route.push_back(getInitialReductionAndRandom(route)); // (n-2)/2 osobnikow losowo redukcyjnym
		pop.push_back(route);
	}

	for (int i = populationSize / 2 + 1; i < populationSize; i++) {
		vector < unsigned > route;
		route.push_back(getInitialGreedyAndRandom(route)); // (n-2)/2 osobnikow losowo zachlannym
		pop.push_back(route);
	}

	std::sort(pop.begin(), pop.end(), // sortowanie populacji 
		[&](const std::vector<unsigned>& a, const std::vector<unsigned>& b) {
		return a.at(matrixSize + 1) < b.at(matrixSize + 1);
	});

	//--------------Wygenerowanie wartosci funkcji zdatnoœci (fitness function)-----------------
	evaluatePopulation(pop, fitnesses);
}

void Genetic::doCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>&offspring1, vector <unsigned>&offspring2){
	switch (crossoverType) {
		case 1: 
			PartiallyMappedCO(parent1, parent2, offspring1, offspring2); // jest gituwa
			break;

		case 2: 
			OrderCO(parent1, parent2, offspring1, offspring2); //do zmiany
			break;

		case 3: 
			CycleCO(parent1, parent2, offspring1, offspring2); //do zmiany
			break;

		case 4: 
			CycleCO2(parent1, parent2, offspring1, offspring2); //jest gituwa
			break;

		case 5:
			TwoPointCO(parent1, parent2, offspring1, offspring2); //do zmiany
			break;

		case 6:
			SequentialCO(parent1, parent2, offspring1); //jest gituwa
			break;

		case 7:
			EnhancedSequentialCO(parent1, parent2, offspring1); //jest gituwa
			break;
	}
}


void Genetic::doSelection(vector <unsigned>& parent1, vector <unsigned>& parent2, vector <vector <unsigned>> population, vector <double>& fitnesses) {
	switch (selectionType) {
	case 1:
		parent1 = tournamentSelection(population);
		parent2 = tournamentSelection(population);
		break;

	case 2:
		parent1 = population.at(rankSelection(fitnesses, population));
		parent2 = population.at(rankSelection(fitnesses, population));
		break;

	case 3:
		parent1 = population.at(rouletteWheelSelection(fitnesses));
		parent2 = population.at(rouletteWheelSelection(fitnesses));
		break;
	}


}

void Genetic::displayBestRoute() {
	for (int i = 0; i < bestRoute.size(); i++)
		cout << bestRoute.at(i) << "-";
	cout << endl;

}

void Genetic::PartiallyMappedCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2) {
	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> nodeRand(1, matrixSize - 1);

	vector <unsigned> visitedOffspring1(matrixSize, 0);
	vector <unsigned> visitedOffspring2(matrixSize, 0);

	int a, b, balance = 0;

	do {
		a = nodeRand(randomGen);
		b = nodeRand(randomGen);
	} while (a == b || a > b);

	//cout << "A: " << a << endl;
	//cout << "B: " << b << endl;


	for (int i = a; i < b; i++) {
		offspring1.at(i)=parent2.at(i);
		offspring2.at(i)=parent1.at(i);
		visitedOffspring1.at(parent2.at(i)) = 1;
		visitedOffspring2.at(parent1.at(i)) = 1;
	}

	for (int i = a-1; i >=0; i--) {
		if (visitedOffspring1.at(parent1.at(i)) != 1) {
			offspring1.at(i)=parent1.at(i);
			if (parent1.at(i) != 0)
				visitedOffspring1.at(parent1.at(i)) = 1;
		}
		else
			offspring1.at(i) = 1000;

		if (visitedOffspring2.at(parent2.at(i)) != 1) {
			offspring2.at(i) = parent2.at(i);
			if (parent2.at(i) != 0)
				visitedOffspring2.at(parent2.at(i)) = 1;
		}
		else
			offspring2.at(i) = 1000;
	}


	for (int i = b; i <= matrixSize; i++) {
		if (visitedOffspring1.at(parent1.at(i)) != 1) {
			offspring1.at(i)=parent1.at(i);
			visitedOffspring1.at(parent1.at(i)) = 1;
		}
		else
			offspring1.at(i)=1000;

		if (visitedOffspring2.at(parent2.at(i))!= 1) {
			offspring2.at(i) = parent2.at(i);
			visitedOffspring2.at(parent2.at(i)) = 1;
		}
		else
			offspring2.at(i) = 1000;
	}

	int help = 0;
	bool cont = true;

	for (int i = 0; i < matrixSize; i++) {
		if (offspring1.at(i) == 1000) {
			help = i;
			while (cont == true) {
				for (int j=0; j < matrixSize; j++) {
					if (parent2.at(j) == parent1.at(help)) {
						if (visitedOffspring1.at(parent1.at(j)) != 1) {
							offspring1.at(i) = parent1.at(j);
							cont = false;
							break;
						}
						else {
							help = j;
						}
					}
				}
			}
			cont = true;
		}

		if (offspring2.at(i) == 1000) {
			help = i;
			while (cont == true) {
				for (int j = 0; j < matrixSize; j++) {
					if (parent1.at(j) == parent2.at(help)) {
						if (visitedOffspring2.at(parent2.at(j)) != 1) {
							offspring2.at(i) = parent2.at(j);
							cont = false;
							break;
						}
						else {
							help = j;
						}
					}
				}
			}
			cont = true;
		}

	}
	offspring1.at(matrixSize+1)=calculateCost(offspring1);
	offspring2.at(matrixSize+1)=calculateCost(offspring2);


}

void Genetic::OrderCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2) {
	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> nodeRand(1, matrixSize - 1);

	vector <unsigned> visitedOffspring1(matrixSize, 0);
	vector <unsigned> visitedOffspring2(matrixSize, 0);

	int a, b, balance = 0;

	do {
		a = nodeRand(randomGen);
		b = nodeRand(randomGen);
	} while (a == b || a > b);

	//cout << "A: " << a << endl;
	//cout << "B: " << b << endl;

	for (int i = a; i < b; i++) {
		offspring1.at(i)=parent1.at(i);
		offspring2.at(i) = parent2.at(i);
		visitedOffspring1.at(parent1.at(i)) = 1;
		visitedOffspring2.at(parent2.at(i)) = 1;
	}


	int omitted = 0, omitted2 = 0;

	for (int i = b; i < matrixSize; i++) {
		if (visitedOffspring1.at(parent2.at(i)) != 1) {
			offspring1.at(i- omitted) = parent2.at(i);
			visitedOffspring1.at(parent2.at(i)) = 1;
		}
		else
			omitted++;

		if (visitedOffspring2.at(parent1.at(i)) != 1) {
			offspring2.at(i - omitted2) = parent1.at(i);
			visitedOffspring2.at(parent1.at(i)) = 1;
		}
		else
			omitted2++;

	}

	int helpOmitted1 = omitted, helpOmitted2 = omitted2;



	for (int i = 1; i < b; i++) {
		if (visitedOffspring1.at(parent2.at(i)) != 1) {
			if (omitted != 0) {
				offspring1.at(matrixSize-omitted)=parent2.at(i);
				visitedOffspring1.at(parent2.at(i)) = 1;
				omitted--;
			}

			else {
				offspring1.at(i- helpOmitted1) = parent2.at(i);
				visitedOffspring1.at(parent2.at(i)) = 1;
			}
		}

		else 
			if (omitted == 0)
				helpOmitted1++;


		if (visitedOffspring2.at(parent1.at(i)) != 1) {
			if (omitted2 != 0) {
				offspring2.at(matrixSize - omitted2) = parent1.at(i);
				visitedOffspring2.at(parent1.at(i)) = 1;
				omitted2--;
			}

			else {
				offspring2.at(i - helpOmitted2) = parent1.at(i);
				visitedOffspring2.at(parent1.at(i)) = 1;
			}
		}

		else
			if (omitted2 == 0)
				helpOmitted2++;
		
	}

	offspring1.at(matrixSize+1)=calculateCost(offspring1);
	offspring2.at(matrixSize + 1) = calculateCost(offspring2);


}


void Genetic::CycleCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2) {

	int omitted = 0, inserted = 0;
	vector <unsigned> visitedOffspring1(matrixSize, 0);
	vector <unsigned> visitedOffspring2(matrixSize, 0);
	vector <unsigned> indexParent1(matrixSize, 0);
	vector <unsigned> indexParent2(matrixSize, 0);

	for (int i = 0; i < matrixSize; i++) {
		indexParent1.at(parent1.at(i)) = i;
		indexParent2.at(parent2.at(i)) = i;
	}

	offspring1.at(1) = parent1.at(1);
	offspring2.at(1) = parent2.at(1);
	visitedOffspring1.at(parent1.at(1)) = 1;
	visitedOffspring2.at(parent2.at(1)) = 1;

	int oldIndex1 = 1, oldIndex2 = 1;

	for (int i = 1; i < matrixSize; i++) {
		if (visitedOffspring1.at(parent2.at(oldIndex1)) != 1) {
			offspring1.at(indexParent1.at(parent2.at(oldIndex1))) = parent2.at(oldIndex1);
			visitedOffspring1.at(parent2.at(oldIndex1)) = 1;
			oldIndex1 = indexParent1.at(parent2.at(oldIndex1));
		}

		if (visitedOffspring2.at(parent1.at(oldIndex2)) != 1) {
			offspring2.at(indexParent2.at(parent1.at(oldIndex2))) = parent1.at(oldIndex2);
			visitedOffspring2.at(parent1.at(oldIndex2)) = 1;
			oldIndex2 = indexParent2.at(parent1.at(oldIndex2));
		}

	}

	for (int i = 1; i < matrixSize; i++) {
		if (offspring1.at(i) == 0) {
			offspring1.at(i) = parent2.at(i);
		}

		if (offspring2.at(i) == 0) {
			offspring2.at(i) = parent1.at(i);
		}
	}

	offspring1.at(matrixSize + 1) = calculateCost(offspring1);
	offspring2.at(matrixSize + 1) = calculateCost(offspring2);

}

void Genetic::CycleCO2(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2) {

	int omitted = 0, inserted = 0;
	vector <unsigned> visitedOffspring1(matrixSize, 0);
	vector <unsigned> visitedOffspring2(matrixSize, 0);
	vector <unsigned> indexParent1(matrixSize, 0);
	vector <unsigned> indexParent2(matrixSize, 0);

	for (int i = 0; i < matrixSize; i++) {
		indexParent1.at(parent1.at(i)) = i;
		indexParent2.at(parent2.at(i)) = i;
	}

	offspring1.at(1) = parent2.at(1);
	visitedOffspring1.at(parent2.at(1)) = 1;

	int oldIndex1 = 1, oldIndex2 = 1;
	for (int i = 1; i < matrixSize; i++) {
		if (visitedOffspring2.at(parent2.at(indexParent1.at(parent2.at(indexParent1.at(offspring1.at(i)))))) != 1) {
			offspring2.at(i) = parent2.at(indexParent1.at(parent2.at(indexParent1.at(offspring1.at(i)))));
			visitedOffspring2.at(parent2.at(indexParent1.at(parent2.at(indexParent1.at(offspring1.at(i)))))) = 1;
		}

		if (i < matrixSize - 1) {
			if (visitedOffspring1.at(parent2.at(indexParent1.at(offspring2.at(i)))) != 1) {
				offspring1.at(i + 1) = parent2.at(indexParent1.at(offspring2.at(i)));
				visitedOffspring1.at(parent2.at(indexParent1.at(offspring2.at(i)))) = 1;
			}
			else {
				for (int j = 1; j < matrixSize; j++) {
					if (visitedOffspring1.at(parent2.at(j)) != 1) {
						if (visitedOffspring1.at(parent2.at(j))!= 1) {
							offspring1.at(i + 1) = parent2.at(j);
							visitedOffspring1.at(parent2.at(j)) = 1;
							break;
						}
					}
				}
			}
		}
	}

	offspring1.at(matrixSize + 1) = calculateCost(offspring1);
	offspring2.at(matrixSize + 1) = calculateCost(offspring2);

}

void Genetic::TwoPointCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring1, vector <unsigned>& offspring2) {
	random_device randomSrc;
	default_random_engine randomGen(randomSrc());
	uniform_int_distribution<> nodeRand(1, matrixSize - 1);

	vector <unsigned> visitedOffspring1(matrixSize, 0);
	vector <unsigned> visitedOffspring2(matrixSize, 0);

	int a, b, balance = 0;

	do {
		a = nodeRand(randomGen);
		b = nodeRand(randomGen);
	} while (a == b || a > b);


	//cout << "A: " << a << endl;
	//cout << "B: " << b << endl;

	for (int i = 1; i < a; i++) {
		offspring1.at(i) = parent1.at(i);
		offspring2.at(i) = parent2.at(i);
		visitedOffspring1.at(parent1.at(i)) = 1;
		visitedOffspring2.at(parent2.at(i)) = 1;
	}

	for (int i = b; i < matrixSize; i++) {
		offspring1.at(i) = parent1.at(i);
		offspring2.at(i) = parent2.at(i);
		visitedOffspring1.at(parent1.at(i)) = 1;
		visitedOffspring2.at(parent2.at(i)) = 1;
	}

	bool cont = true;
	int omitted1 = 0, omitted2 = 0;

	for (int i = a; i < b; i++) {
		if (visitedOffspring1.at(parent2.at(i)) != 1) {
			offspring1.at(i) = parent2.at(i);
			visitedOffspring1.at(parent2.at(i)) = 1;
		}

		if (visitedOffspring2.at(parent1.at(i)) != 1) {
			offspring2.at(i) = parent1.at(i);
			visitedOffspring2.at(parent1.at(i)) = 1;
		}

	}
	for (int i = a; i < b; i++) {

		if(offspring1.at(i)==0){
			for (int j = 1; j < a; j++) {
				if (visitedOffspring1.at(parent2.at(j)) != 1) {
					offspring1.at(i) = parent2.at(j);
					visitedOffspring1.at(parent2.at(j)) = 1;
					cont = false;
					break;
				}
			}
			if (cont == true) {
				for (int j = b; j < matrixSize; j++) {
					if (visitedOffspring1.at(parent2.at(j)) != 1) {
						offspring1.at(i) = parent2.at(j);
						visitedOffspring1.at(parent1.at(j)) = 1;
						break;
					}
				}
			}
		}
		cont = true;

		if (offspring2.at(i) == 0) {
			for (int j = 1; j < a; j++) {
				if (visitedOffspring2.at(parent1.at(j)) != 1) {
					offspring2.at(i) = parent1.at(j);
					visitedOffspring2.at(parent1.at(j)) = 1;
					cont = false;
					break;
				}
			}
			if (cont == true) {
				for (int j = b; j < matrixSize; j++) {
					if (visitedOffspring2.at(parent1.at(j)) != 1) {
						offspring2.at(i) = parent1.at(j);
						visitedOffspring2.at(parent1.at(j)) = 1;
						break;
					}
				}
			}
		}
	}

	offspring1.at(matrixSize + 1) = calculateCost(offspring1);
	offspring2.at(matrixSize + 1) = calculateCost(offspring2);

}

void Genetic::SequentialCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring) {

	int omitted = 0, inserted = 0;
	vector <unsigned> visitedOffspring(matrixSize, 0);
	vector <unsigned> indexParent1(matrixSize, 0);
	vector <unsigned> indexParent2(matrixSize, 0);

	for (int i = 0; i < matrixSize; i++) {
		indexParent1.at(parent1.at(i)) = i;
		indexParent2.at(parent2.at(i)) = i;
	}
	offspring.at(0) = 0;

	int node1, node2;
	if (costXY(0, parent1.at(1)) < costXY(0, parent2.at(1))) {
		offspring.at(1) = parent1.at(1);
		visitedOffspring.at(parent1.at(1)) = 1;
	}
	else {
		offspring.at(1) = parent2.at(1);
		visitedOffspring.at(parent2.at(1)) = 1;
	}

	for (int i = 2; i < matrixSize; i++) {
		if (indexParent1.at(offspring.at(i - 1)) + 1 <= matrixSize - 1 && visitedOffspring.at(parent1.at(indexParent1.at(offspring.at(i - 1)) + 1)) != 1) {
			node1 = parent1.at(indexParent1.at(offspring.at(i - 1)) + 1);
		}

		else {
			for (int j = 1; j < matrixSize; j++) 
				if (visitedOffspring.at(j) != 1) {
					node1 = j;
					break;
				}
		}
		if (indexParent2.at(offspring.at(i - 1)) + 1 <= matrixSize - 1 && visitedOffspring.at(parent2.at(indexParent2.at(offspring.at(i - 1)) + 1))!=1) {
			node2 = parent2.at(indexParent2.at(offspring.at(i - 1)) + 1);
		}
		else {
			for (int j = 1; j < matrixSize; j++)
				if (visitedOffspring.at(j) != 1) {
					node2 = j;
					break;
				}
		}
		//cout << node1 << endl;
		//cout << node2 << endl;


		if (costXY(offspring.at(i - 1), node1) < costXY(offspring.at(i - 1), node2)) {
			offspring.at(i) = node1;
			visitedOffspring.at(node1) = 1;
		}

		else {
			offspring.at(i) = node2;
			visitedOffspring.at(node2) = 1;
		}
	}

	offspring.at(matrixSize + 1) = calculateCost(offspring);
	

}

void Genetic::EnhancedSequentialCO(vector <unsigned> parent1, vector <unsigned> parent2, vector <unsigned>& offspring) {

	int omitted = 0, inserted = 0;
	vector <unsigned> visitedOffspring(matrixSize, 0);
	vector <unsigned> indexParent1(matrixSize, 0);
	vector <unsigned> indexParent2(matrixSize, 0);

	for (int i = 0; i < matrixSize; i++) {
		indexParent1.at(parent1.at(i)) = i;
		indexParent2.at(parent2.at(i)) = i;
	}

	offspring.at(0) = 0;

	int node1, node2, minimum1 = INT_MAX, minimum2 = INT_MAX;

	for (int i = 0; i < matrixSize; i++) {
		if (visitedOffspring.at(i) != 1 && costXY(parent1.at(1), i)<minimum1)
			minimum1 = costXY(parent1.at(1), i);

		if (visitedOffspring.at(i) != 1 && costXY(parent2.at(1), i) < minimum2)
			minimum2 = costXY(parent2.at(1), i);
	}

	if (costXY(0, parent1.at(1))+minimum1 < costXY(0, parent2.at(1))+minimum2) {
		offspring.at(1) = parent1.at(1);
		visitedOffspring.at(parent1.at(1)) = 1;
	}
	else {
		offspring.at(1) = parent2.at(1);
		visitedOffspring.at(parent2.at(1)) = 1;
	}

	for (int i = 2; i < matrixSize; i++) {
		if (indexParent1.at(offspring.at(i - 1)) + 1 <= matrixSize - 1 && visitedOffspring.at(parent1.at(indexParent1.at(offspring.at(i - 1)) + 1)) != 1) {
			node1 = parent1.at(indexParent1.at(offspring.at(i - 1)) + 1);
		}

		else {
			for (int j = 1; j < matrixSize; j++)
				if (visitedOffspring.at(j) != 1) {
					node1 = j;
					break;
				}
		}
		if (indexParent2.at(offspring.at(i - 1)) + 1 <= matrixSize - 1 && visitedOffspring.at(parent2.at(indexParent2.at(offspring.at(i - 1)) + 1)) != 1) {
			node2 = parent2.at(indexParent2.at(offspring.at(i - 1)) + 1);
		}
		else {
			for (int j = 1; j < matrixSize; j++)
				if (visitedOffspring.at(j) != 1) {
					node2 = j;
					break;
				}
		}

		minimum1 = INT_MAX, minimum2 = INT_MAX;

		for (int j = 0; j < matrixSize; j++) {
			if (visitedOffspring.at(j) != 1 && costXY(parent1.at(1), j) < minimum1)
				minimum1 = costXY(node1, j);

			if (visitedOffspring.at(j) != 1 && costXY(parent2.at(1), j) < minimum2)
				minimum2 = costXY(node2, j);
		}

		if (costXY(offspring.at(i - 1), node1)+minimum1 < costXY(offspring.at(i - 1), node2)+minimum2) {
			offspring.at(i) = node1;
			visitedOffspring.at(node1) = 1;
		}

		else {
			offspring.at(i) = node2;
			visitedOffspring.at(node2) = 1;
		}
	}

	offspring.at(matrixSize + 1) = calculateCost(offspring);

	//showVector(offspring);
}