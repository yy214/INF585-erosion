#pragma once

#include "cgp/cgp.hpp"
#include "helpers/gui.hpp"

namespace ErosionScheme {
	void applyErosionStep(cgp::mesh& m, cgp::grid_2D<cgp::int2> const& stream_tree, cgp::grid_2D<cgp::int2> lakeCenters, cgp::grid_2D<float> areaMap, gui_parameters const& gui_param);
	void erodeOnce(cgp::mesh& m, gui_parameters const& gui_param);
}