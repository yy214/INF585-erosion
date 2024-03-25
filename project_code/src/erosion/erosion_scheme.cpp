#include "cgp/cgp.hpp"
#include "erosion_scheme.hpp"
#include "datastructure/gridTools.hpp"
#include <queue>
#include "erosion/stream_tree.hpp"
#include "erosion/flood_fill.hpp"
#include "helpers/gui.hpp"
#include "math.h"
#include "helpers/colorInterp.hpp"

using namespace colorInterpolation;

void ErosionScheme::applyErosionStep(cgp::mesh& m, cgp::grid_2D<cgp::int2> const& stream_tree, cgp::grid_2D<cgp::int2> lakeCenters, cgp::grid_2D<float> areaMap, gui_parameters const& gui_param)
{
	// Getting the grid sizes
	int total = m.position.size();
	int N = std::sqrt(total);
	float step = std::pow(10.f, gui_param.log_dt);

	// All of the nodes are passed in order such that the 
	std::vector<cgp::int2> sorted_vertices = StreamTree::topological_sort(stream_tree);
	std::reverse(sorted_vertices.begin(), sorted_vertices.end());

	for (cgp::int2 v : sorted_vertices) {
		float dist = std::sqrt(std::pow((float)v[0] / N - 0.5f, 2.f) + std::pow((float)v[1] / N - 0.5f, 2.f));
		
		float uplift = gui_param.uplift * std::exp(-dist * dist / gui_param.radius / gui_param.radius);

		int colorIndex = getIndex(v[0], v[1], N);
		if (stream_tree(v) == StreamTree::NONE) {
			// This is a Sea node
			m.color[colorIndex] = vec3(100.0, 0.0, 0.0);
			float h = get_height(m, v, N) + uplift*step;
			h = (1.f * ((h + 0.5f) * (h + 0.5f) - 0.5f) + 999.f * h) / 1000.f; //prevents sea from completely dissapearing
			get_height(m, v, N) = h;
		}
		else {
			int2 receiver = stream_tree(v);
			float receiver_height = -0.f; 
			if (receiver == StreamTree::SEA) {
				// This is an Outflow node
				m.color[colorIndex] = vec3(100.0, 100.0, 100.0);
			}
			else {
				// This is a Terrain node
				m.color[colorIndex] = vec3(0.0, 100.0, 0.0);
				receiver_height = get_height(m, receiver, N);
			}

			float piPJ = 1.f / N;//std::sqrt(pow(posVi[0] - posVj[0],2) + pow(posVi[1] - posVj[1], 2));
			float kAm = gui_param.param_k * std::pow(areaMap(v[0], v[1]), gui_param.param_m);

			float h = (get_height(m, v, N) + step * (uplift + kAm * receiver_height / piPJ)) /
				(1.0 + kAm * step / piPJ);
			h = (9.f * h + 1.f * std::log(h + 1.f)) / 10.f;
			get_height(m, v, N) = h;
		}
		
		// This gives the vertices the proper coloring according to their height / z-axis coordinate
		m.color[colorIndex] = getColor(m.position[colorIndex][2]);
	}

}


// Carries out one erosion step
void ErosionScheme::erodeOnce(cgp::mesh& m, gui_parameters const& gui_param) {
	size_t N = std::sqrt(m.position.size());
	
	cgp::grid_2D<short> is_sea = getfloodBool(m);
	
	//std::cout << is_sea << std::endl;

	// Debugging statements included
	//std::cout << "building base stream tree" << std::endl;
	cgp::grid_2D<cgp::int2> newStream = StreamTree::get_base_stream_tree(m, is_sea);

	//std::cout << "building lakes" << std::endl;;
	cgp::grid_2D<cgp::int2> newLakes = StreamTree::get_lakes(newStream);

	//std::cout << "updating stream tree with lakes" << std::endl;
	std::map<cgp::int2, 
			 std::vector<StreamTree::LakeEdgeHeight>, 
			 lex_order_class> newLakeGraph = StreamTree::get_lake_graph(m, newStream, newLakes);
	get_final_stream_tree_from_lake_graph(newStream, newLakeGraph);

	//std::cout << "calculating drainage areas" << std::endl;
	cgp::grid_2D<float> newDrainage = StreamTree::get_drainage_area(newStream);

	//std::cout << "applying erosion" << std::endl;
	ErosionScheme::applyErosionStep(m, newStream, newLakes, newDrainage, gui_param);
}