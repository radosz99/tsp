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