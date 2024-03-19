#include "cgp/cgp.hpp"
//#include "stream_tree.hpp"
#include "erosion_scheme.hpp"
#include "datastructure/gridTools.hpp"

using namespace cgp;

void erosionScheme::setHeightMap(cgp::mesh m)
{
	int N = m.position.size();
	int dim = std::sqrt(N);
	
	for (int i = 0; i < m.position.size(); i++) {
		int2 currPoint = getCoord(i, dim);
		heightMap(currPoint[0], currPoint[1]) = m.position[i][2];
	}
}

void erosionScheme::applyErosionStep(float step=10)
{
	int2 POS = int2(0,0);
	// for POS in root nodes:
	//complete the erosion equation
	//heightMap(POS[0], POS[1]) = (heightMap(POS[0], POS[1]) + step*(ui + k*area) / ();


}
