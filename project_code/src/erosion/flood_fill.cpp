#include "flood_fill.hpp"
#include "datastructure/gridTools.hpp"
#include <queue>

cgp::grid_2D<short> floodFill::getfloodBool(cgp::mesh const& m,int initialIndex)
{
	// Getting the grid sizes
	int N = m.position.size();
	int dim = std::sqrt(N);
	int2 currPoint = getCoord(initialIndex, dim);

	// Grid-like bool structure for if a node is part of the sea or not
	cgp::grid_2D<short> floodBool = cgp::grid_2D<short>(dim,dim);

	// Keeps track if we have already visited this node in this function call
	cgp::grid_2D<short> visitedBool = cgp::grid_2D<short>(dim,dim);

	// Initializing bools
	floodBool.fill(0);
	visitedBool.fill(0);
	

	// The queue to search all the points on the grid
	std::queue<int2> coordQueue;

	coordQueue.push(currPoint);

	// Also push many more points, in case the original point ends up on a mini-island,
	// stopping the flood fill
	coordQueue.push(int2(dim-1,dim-1));
	coordQueue.push(int2(0, dim-1));
	coordQueue.push(int2(dim-1, dim-1));
	coordQueue.push(int2(int(dim/2), dim-1));


	int counter = 0;
	while (coordQueue.size() > 0) {
		counter += 1;

		// Askking the queue what point we should visit next, and removing it from the queue
		currPoint = coordQueue.front();
		coordQueue.pop();

		// Converting from coordinate to index in mesh
		int currIndex = getIndex(currPoint[0], currPoint[1], dim);

		visitedBool(currPoint[0], currPoint[1]) = 1;

		//Sea nodes are those below y=0
		if (m.position[currIndex][2] < 0.0) {
			floodBool(currPoint[0], currPoint[1]) = 1;
		}
		else {
			continue;
		}
		
		/////// Checking points in all 4 directions around the current node

		//Top
		if (currPoint[1] < dim - 1 && visitedBool(currPoint[0],currPoint[1]+1) == 0) {
			int2 topPoint = currPoint;
			topPoint[1] += 1;
			coordQueue.push(topPoint);
			visitedBool(topPoint[0], topPoint[1]) = 1;
		}

		//Bottom
		if (currPoint[1] > 0 && visitedBool(currPoint[0], currPoint[1] - 1) == 0) {
			int2 botPoint = currPoint;
			botPoint[1] += -1;
			coordQueue.push(botPoint);
			visitedBool(botPoint[0], botPoint[1]) = 1;
		}

		//Right
		if (currPoint[0] < dim - 1 && visitedBool(currPoint[0] + 1, currPoint[1]) == 0) {
			int2 rightPoint = currPoint;
			rightPoint[0] += 1;
			coordQueue.push(rightPoint);
			visitedBool(rightPoint[0], rightPoint[1]) = 1;
		}

		//Left
		if (currPoint[0] > 0 && visitedBool(currPoint[0] - 1, currPoint[1]) == 0) {
			int2 leftPoint = currPoint;
			leftPoint[0] += -1;
			coordQueue.push(leftPoint);
			visitedBool(leftPoint[0], leftPoint[1]) = 1;
		}
	}

	return floodBool;
}
