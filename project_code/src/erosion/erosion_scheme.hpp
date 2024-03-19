#pragma once

#include "cgp/cgp.hpp"
//#include "stream_tree.hpp"

class erosionScheme {
	cgp::mesh erosionMesh;
	cgp::grid_2D<cgp::int2> stream_tree;

	cgp::grid_2D<float> heightMap;

	void setHeightMap(cgp::mesh);


	void applyErosionStep(float step);




};