#include "flood_fill.hpp"
#include "datastructure/gridTools.hpp"
#include <queue>

static cgp::int2 const directions[4] = { {1,0}, {0,1}, {-1, 0}, {0, -1} };

cgp::grid_2D<short> getfloodBool(cgp::mesh const& m)
{
	// Getting the grid sizes
	int N = m.position.size();
	int dim = std::sqrt(N);

	cgp::grid_2D<short> floodBool;
	floodBool.resize(dim);
	floodBool.fill(0);

	std::queue<cgp::int2> exploration;

	for (int i = 0; i < dim; i++) {
		cgp::int2 p = int2(i, 0);
		floodBool(p) = 1;
		exploration.push(p);
	}
	for (int i = 0; i < dim; i++) {
		cgp::int2 p = int2(0, i);
		floodBool(p) = 1;
		exploration.push(p);
	}
	for (int i = 0; i < dim; i++) {
		cgp::int2 p = int2(i, dim-1);
		floodBool(p) = 1;
		exploration.push(p);
	}
	for (int i = 0; i < dim; i++) {
		cgp::int2 p = int2(dim-1, i);
		floodBool(p) = 1;
		exploration.push(p);
	}
	//std::cout << dim << std::endl;
	while (!exploration.empty()) {
		cgp::int2 currPos = exploration.front();
		exploration.pop();
		floodBool(currPos) = 1;
		for (int dir = 0; dir < 4; dir++) {
			cgp::int2 nextPos = currPos + directions[dir];
			if (nextPos.x < 0 || nextPos.x >= dim || nextPos.y < 0 || nextPos.y >= dim) continue;
			//std::cout << nextPos << " inside" << std::endl;
			if (get_height(m, nextPos, dim) < 0.f && floodBool(nextPos) == 0) {
				floodBool(nextPos) = 1;
				exploration.push(nextPos);
			}
		}
	}
	return floodBool;
}
