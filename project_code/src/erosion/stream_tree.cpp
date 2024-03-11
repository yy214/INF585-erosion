#include "stream_tree.hpp"
#include "cgp/cgp.hpp"

// put elsewhere btw
cgp::int2 const directions[4] = {{1,0}, {0,1}, {-1, 0}, {0, -1}};

cgp::grid_2D<cgp::int2> StreamTree::get_base_stream_tree(cgp::mesh const& m, cgp::grid_2D<bool> is_sea){
    cgp::grid_2D<cgp::int2> result;

    cgp::int2 dim = is_sea.dimension;
    result.resize(dim);

    for(int i = 1; i < dim.x-1; i++){
        for(int j = 1; j < dim.y-1; j++){
            if(is_sea(i,j)) continue;
            
            int optiDir = -1;
            float min_height = m.position(result.index_to_offset(i,j)).z;
            for(int dir = 0; dir < 4; dir++){
                cgp::int2 next_pos = directions[dir] + cgp::int2({i, j});
                float new_height = result.index_to_offset(next_pos.x, next_pos.y);
                if(new_height < min_height){
                    min_height = new_height;
                    optiDir = dir;
                }
            }
            if(optiDir == -1){
                result(i, j) = StreamTree::NONE; // lake
            }
            else{
                cgp::int2 flow_next_pos = directions[optiDir] + cgp::int2({i,j});
                if(is_sea(flow_next_pos)){
                    result(i, j) = StreamTree::SEA;
                }
                else{
                    result(i, j) = flow_next_pos;
                }
            }
        }
    }
}