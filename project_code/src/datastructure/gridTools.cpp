#include "gridTools.hpp"

//Get an index from grid coordinates
int getIndex(int x, int y, int N) {
	return x * N + y;
};

//Get a grid coordinate from an index
int2 getCoord(int index, int N) {
	int x = floor(index / N);
	int y = index % N;

	return int2(x, y);
};