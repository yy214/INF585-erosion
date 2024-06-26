#pragma once

#include "cgp/cgp.hpp"
#include <vector>
#include <map>
#include <algorithm>
#include "datastructure/gridTools.hpp"


namespace StreamTree{
    struct LakeEdge{
        cgp::int2 src, dest;
        void swap(){
            std::swap(src, dest);
        }
        void make_consistent(){
            if(!lex_order(src, dest)){
                swap();
            }
        }
    };

    class lake_edge_comp{
    public:
        inline bool operator()(LakeEdge const& e1, LakeEdge const& e2) const {
            if(e1.src == e2.src) return lex_order(e1.dest, e2.dest);
            return lex_order(e1.src, e2.src);
        }
    };

    struct LakeEdgeHeight{
        LakeEdge edge;
        float height;
    };

    class lake_edge_height_comp{
    public:
        inline bool operator()(LakeEdgeHeight const& e1, LakeEdgeHeight const& e2) const {
            return e1.height > e2.height;
        }
    };

    const cgp::int2 NONE = {-1,-1};
    const cgp::int2 SEA = {-42,-42};

    cgp::grid_2D<cgp::int2> get_base_stream_tree(cgp::mesh const& m, cgp::grid_2D<short> const& is_sea);

    void topological_sort_aux(cgp::grid_2D<cgp::int2> const& stream_tree, cgp::grid_2D<short>& visited, std::vector<cgp::int2>& sorted,cgp::int2 curr_node);
    std::vector<cgp::int2> topological_sort(cgp::grid_2D<cgp::int2> const& stream_tree);

    cgp::grid_2D<cgp::int2> get_lakes(cgp::grid_2D<cgp::int2> const& stream_tree);

    std::map<cgp::int2, std::vector<LakeEdgeHeight>, lex_order_class> get_lake_graph(cgp::mesh const& m,
                                                                                     cgp::grid_2D<cgp::int2>& stream_tree, 
                                                                                     cgp::grid_2D<cgp::int2> lakes);
    void get_final_stream_tree_from_lake_graph(cgp::grid_2D<cgp::int2>& stream_tree, 
                                               std::map<cgp::int2, std::vector<LakeEdgeHeight>, lex_order_class> lake_graph);

    cgp::grid_2D<float> get_drainage_area(cgp::grid_2D<cgp::int2> const& stream_tree);

};