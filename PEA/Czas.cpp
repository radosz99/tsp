#include <iostream>
#include <chrono>

#include "Czas.h"

using namespace std;
using namespace std::chrono;

void Czas::czasStart() {
	czasPoczatkowy = high_resolution_clock::now();
}

void Czas::czasStop() {
	czasKoncowy = high_resolution_clock::now();
}

long Czas::czasWykonania() {

	return duration_cast<microseconds>(Czas::czasKoncowy - Czas::czasPoczatkowy).count();
}

long Czas::czasWykonaniaMili() {

	return duration_cast<milliseconds>(Czas::czasKoncowy - Czas::czasPoczatkowy).count();
}

long Czas::czasWykonaniaNano() {

		return duration_cast<nanoseconds>(Czas::czasKoncowy - Czas::czasPoczatkowy).count();

}

Czas::Czas()
{
	
}

void Czas::start()
{
	tstart = std::chrono::steady_clock::now();
}

void Czas::stop()
{
	tstop = std::chrono::steady_clock::now();
	measurement = tstop - tstart;
}

double Czas::read()
{
	return measurement.count();
}