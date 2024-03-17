#include "stream_tree.hpp"
#include "cgp/cgp.hpp"

#include <vector>
#include <algorithm> // reverse
#include <set>
#include <queue>
#include <stack>

// put elsewhere btw
cgp::int2 const directions[4] = {{1,0}, {0,1}, {-1, 0}, {0, -1}};

namespace StreamTree
{
    cgp::grid_2D<cgp::int2> get_base_stream_tree(cgp::mesh const& m, cgp::grid_2D<short> const& is_sea){
        cgp::grid_2D<cgp::int2> stream_tree;

        cgp::int2 dim = is_sea.dimension;
        stream_tree.resize(dim);
        stream_tree.fill(NONE);

        for(int i = 1; i < dim.x-1; i++){
            for(int j = 1; j < dim.y-1; j++){
                cgp::int2 curr_pos = cgp::int2(i,j);
                
                if(is_sea(curr_pos)) continue;
                
                int optiDir = -1;
                float min_height = get_height(m, stream_tree, curr_pos);
                for(int dir = 0; dir < 4; dir++){
                    cgp::int2 next_pos = directions[dir] + curr_pos;
                    float new_height = get_height(m, stream_tree, next_pos);
                    if(new_height < min_height){
                        min_height = new_height;
                        optiDir = dir;
                    }
                }
                if(optiDir == -1){
                    stream_tree(i, j) = StreamTree::NONE; // lake
                }
                else{
                    cgp::int2 flow_next_pos = directions[optiDir] + curr_pos;
                    if(is_sea(flow_next_pos)){
                        stream_tree(i, j) = StreamTree::SEA;
                    }
                    else{
                        stream_tree(i, j) = flow_next_pos;
                    }
                }
            }
        }
        return stream_tree;
    }

    void topological_sort_aux(cgp::grid_2D<cgp::int2> const& stream_tree, cgp::grid_2D<short>& visited, std::vector<cgp::int2> sorted, cgp::int2 curr_node){
        if(curr_node == NONE || curr_node == SEA) return;
        if(visited(curr_node)) return;
        visited(curr_node) = true;

        topological_sort_aux(stream_tree, visited, sorted, stream_tree(curr_node)); //bc only 1 outwards edge
        sorted.push_back(curr_node);
    }

    std::vector<cgp::int2> topological_sort(cgp::grid_2D<cgp::int2> const& stream_tree){
        cgp::int2 dim = stream_tree.dimension;
        
        cgp::grid_2D<short> visited;
        visited.resize(dim);
        visited.fill(false);

        std::vector<cgp::int2> sorted;

        // note: sorted in reverse order
        for(int i = 0; i < dim.x; i++){
            for(int j = 0; j < dim.y; j++){
                topological_sort_aux(stream_tree, visited, sorted, cgp::int2(i,j));
                // cgp::int2 curr_pos = cgp::int2(i,j);
                // std::stack<cgp::int2> s;
                // while(curr_pos != NONE && curr_pos != SEA && !visited(curr_pos)){
                //     visited(curr_pos) = true;
                //     s.push(curr_pos);
                //     curr_pos = stream_tree(curr_pos);
                // }
                // while(!s.empty()){
                //     cgp::int2 pos = s.top();
                //     s.pop();
                //     sorted.push_back(pos);
                // }
            }
        }
        std::reverse(sorted.begin(), sorted.end());

        return sorted;
    }
 
    /// returns the center of the lake (or StreamTree::SEA if it's the sea)
    cgp::grid_2D<cgp::int2> get_lakes(cgp::grid_2D<cgp::int2> const& stream_tree){
        cgp::int2 dim = stream_tree.dimension;
        cgp::grid_2D<cgp::int2> lake_centers;
        lake_centers.resize(dim);

        std::vector<cgp::int2> sorted_vertices = topological_sort(stream_tree);
        std::reverse(sorted_vertices.begin(), sorted_vertices.end());

        for(cgp::int2 v : sorted_vertices){
            if(stream_tree(v) == NONE){
                lake_centers(v) = v;
            }
            else{
                // check if works properly with SEA
                lake_centers(v) = lake_centers(stream_tree(v)); //works bc of topological sort
            }
        }

        return lake_centers;
    }

    std::map<cgp::int2, std::vector<LakeEdgeHeight>, lex_order_class> get_lake_graph(cgp::mesh const& m,
                                                                                     cgp::grid_2D<cgp::int2>& stream_tree, 
                                                                                     cgp::grid_2D<cgp::int2> lakes){
        cgp::int2 dim = stream_tree.dimension;
        std::map<LakeEdge, float, lake_edge_comp> min_edge_height;

        for(int i = 0; i < dim.x; i++){
            for(int j = 0; j < dim.y; j++){
                cgp::int2 curr_pos = cgp::int2(i,j);
                cgp::int2 next_pos = stream_tree(curr_pos);
                if(next_pos == NONE) continue;

                cgp::int2 lake_a = lakes(curr_pos), lake_b = lakes(next_pos);
                if(lakes(curr_pos) == lakes(next_pos)) continue;

                LakeEdge lake_edge = {lake_a, lake_b};
                lake_edge.make_consistent();

                float curr_height = std::min(get_height(m, stream_tree, curr_pos), get_height(m, stream_tree, next_pos));
                if(min_edge_height.find(lake_edge) == min_edge_height.end()){ // 1st edge between these 2 lakes
                    min_edge_height[lake_edge] = curr_height;
                }
                else{
                    min_edge_height[lake_edge] = std::min(min_edge_height[lake_edge], curr_height);
                }
            }
        }

        std::map<cgp::int2, std::vector<LakeEdgeHeight>, lex_order_class> lake_graph;
        for(std::map<LakeEdge, float>::iterator it = min_edge_height.begin(); it != min_edge_height.end(); it++){
            LakeEdge e = it->first;
            float height = it->second;
            lake_graph[e.src].push_back({e, height}); // check if works properly
            // other side
            e.swap();
            lake_graph[e.src].push_back({e, height});
        }

        return lake_graph;
    }

    void get_final_stream_tree_from_lake_graph(cgp::grid_2D<cgp::int2>& stream_tree, 
                                               std::map<cgp::int2, std::vector<LakeEdgeHeight>, lex_order_class> lake_graph){
        std::set<cgp::int2, lex_order_class> seen; //check if ok, otherwise define a comparator class for int2
        std::priority_queue<LakeEdgeHeight, std::vector<LakeEdgeHeight>, lake_edge_height_comp> edges;

        seen.insert(SEA);
        for(LakeEdgeHeight e : lake_graph[SEA]){
            edges.push(e);
        }
        
        while(!edges.empty()){
            LakeEdgeHeight e = edges.top();
            edges.pop();
            if(seen.find(e.edge.dest) != seen.end()) continue;  

            // debug
            if(stream_tree(e.edge.dest) != NONE){
                std::cout << "error: the bottom of the lake flows somewhere" << std::endl;
            }
            stream_tree(e.edge.dest) = e.edge.src;
            seen.insert(e.edge.dest);
            for(LakeEdgeHeight out_edge : lake_graph[e.edge.dest]){
                edges.push(out_edge);
            }
        }
    }


    cgp::grid_2D<float> get_drainage_area(cgp::grid_2D<cgp::int2> const& stream_tree){
        cgp::int2 dim = stream_tree.dimension;
        
        float area = 1 / dim.x / dim.y; // otherwise make an "area" function or even store it somewhere
        std::vector<cgp::int2> sorted_vertices = topological_sort(stream_tree);

        cgp::grid_2D<float> drainage_area;

        drainage_area.resize(dim);
        drainage_area.fill(0.f);

        for(cgp::int2 v : sorted_vertices){
            drainage_area(v) += area;
            drainage_area(stream_tree(v)) += drainage_area(v);
        }

        return drainage_area;
    }


}