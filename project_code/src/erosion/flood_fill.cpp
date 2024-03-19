#include "flood_fill.hpp"
#include "datastructure/gridTools.hpp"

#include <queue>

cgp::grid_2D<int> floodFill::getfloodBool(cgp::mesh const& m,int initialIndex)
{
	int N = m.position.size();
	int dim = std::sqrt(N);
	int2 currPoint = getCoord(initialIndex, dim);

	cgp::grid_2D<int> floodBool = cgp::grid_2D<int>(dim,dim);
	cgp::grid_2D<int> visitedBool = cgp::grid_2D<int>(dim,dim);

	floodBool.fill(0);
	visitedBool.fill(0);
	

	//the queue to search all the points
	std::queue<int2> coordQueue;

	coordQueue.push(currPoint);


	int counter = 0;
	while (coordQueue.size() > 0) {
		counter += 1;
		//std::cout << counter;

		//getting the next coordinate to check flood
		currPoint = coordQueue.front();
		coordQueue.pop();
		int currIndex = getIndex(currPoint[0], currPoint[1], dim);

		visitedBool(currPoint[0], currPoint[1]) = 1;

		if (m.position[currIndex][2] < 0.5) {
			floodBool(currPoint[0], currPoint[1]) = 1;
		}
		else {
			continue;
		}
		
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
		
		

		//explore itself and its neighbors
		//changeme to have int2??
		
		
	
	}

	
	//visitedBool[{currIndex}] = true;
	//visitedBool(4,4) = 3;

	std::cout << visitedBool;
	std::cout << "got here";
	std::cout << floodBool;

	//m.color[0][0] = 0;




	return floodBool;
}
