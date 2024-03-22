#pragma once

#include "cgp/cgp.hpp"
//#include "stream_tree.hpp"

class erosionScheme {
	public:

	cgp::mesh erosionMesh;
	cgp::grid_2D<cgp::int2> stream_tree;

	cgp::grid_2D<float> heightMap;

	void setHeightMap(cgp::mesh m);


	void applyErosionStep(cgp::mesh& m, cgp::grid_2D<cgp::int2> const& stream_tree, cgp::grid_2D<cgp::int2> lakeCenters,float step);


};