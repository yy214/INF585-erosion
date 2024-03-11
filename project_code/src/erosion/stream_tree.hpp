#pragma once

#include "cgp/cgp.hpp"

namespace StreamTree{
    const cgp::int2 NONE = {-1,-1};
    const cgp::int2 SEA = {-42,-42};

    cgp::grid_2D<cgp::int2> get_base_stream_tree(cgp::mesh const& m, cgp::grid_2D<bool> is_sea);
};