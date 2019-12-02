#include <iostream>
#include <chrono>

#ifndef SDIZO_PROJEKT_CZAS_H
#define SDIZO_PROJEKT_CZAS_H

using namespace std;
using namespace std::chrono;

class Czas {
public:
	high_resolution_clock::time_point czasPoczatkowy;
	high_resolution_clock::time_point czasKoncowy;

	void czasStart();

	void czasStop();

	long czasWykonania();
	long czasWykonaniaMili();
	long czasWykonaniaNano();

	Czas();
	void start();
	void stop();
	double read();

private:
	std::chrono::duration<double> measurement;
	std::chrono::time_point<std::chrono::steady_clock> tstart;
	std::chrono::time_point<std::chrono::steady_clock> tstop;


};



#endif //SDIZO_PROJEKT_CZAS_H#pragma once
