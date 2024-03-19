#pragma once

#include "cgp/cgp.hpp"

namespace floodFill {

	cgp::grid_2D<int> getfloodBool(cgp::mesh const& m,int initialIndex);

}