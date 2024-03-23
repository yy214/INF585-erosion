#include "cgp/cgp.hpp"
//#include "stream_tree.hpp"
#include "erosion_scheme.hpp"
#include "datastructure/gridTools.hpp"
#include <queue>
#include "erosion/stream_tree.hpp"
#include "erosion/flood_fill.hpp"
#include "helpers/gui.hpp"
#include "math.h"


void ErosionScheme::applyErosionStep(cgp::mesh& m, cgp::grid_2D<cgp::int2> const& stream_tree, cgp::grid_2D<cgp::int2> lakeCenters, cgp::grid_2D<float> areaMap, gui_parameters const& gui_param)
{
	int total = m.position.size();
	int N = std::sqrt(total);
	float step = std::pow(gui_param.log_dt, 10);

	//for now this is constant
	float uplift = 0.001f;


	std::vector<cgp::int2> sorted_vertices = StreamTree::topological_sort(stream_tree);
	std::reverse(sorted_vertices.begin(), sorted_vertices.end());

	for (cgp::int2 v : sorted_vertices) {
		int colorIndex = getIndex(v[0], v[1], N);
		float newHeight = 0.0f;
		cgp::vec3 posVi = m.position[colorIndex];
		if (stream_tree(v) == StreamTree::NONE) {
			//it is a sea node
			m.color[colorIndex] = vec3(100.0, 0.0, 0.0);
			newHeight = posVi[2] + uplift;
		}
		else {
			int2 receiver = stream_tree(v);
			float receiver_height = 0;
			if (receiver == StreamTree::SEA) {
				//IT IS AN OUTFLOW
				m.color[colorIndex] = vec3(100.0, 100.0, 100.0);
			}
			else {
				//it is a non-root node
				m.color[colorIndex] = vec3(0.0, 100.0, 0.0);
				receiver_height = get_height(m, receiver, N);
			}

			//cgp::vec3 posVj = m.position[getIndex(receiver[0],receiver[1],N)];

			float piPJ = 1.f / N;//std::sqrt(pow(posVi[0] - posVj[0],2) + pow(posVi[1] - posVj[1], 2));
			float kAm = gui_param.param_k * std::pow(areaMap(v[0], v[1]), gui_param.param_m);

			get_height(m, v, N) = (get_height(m, v, N) + step * (uplift + kAm * receiver_height / piPJ)) /
				(1.0 + kAm * step / piPJ);

		}
		//heightMap(v[0], v[1]) = newHeight;
	}
}


//Visualizing the Stream Trees
void ErosionScheme::erodeOnce(cgp::mesh& m, gui_parameters const& gui_param) {
	cgp::grid_2D<short> is_sea = floodFill::getfloodBool(m, 0);

	size_t N = std::sqrt(m.position.size());

	//for (int i = 0; i < N; i++) {
	//	for (int j = 0; j < N; j++) {
	//		//vec3& p_shape = initMesh.position[colorIndex];
	//		//p_shape[2] = 0.0f;


	//		//initMesh.position[colorIndex][2] = 0.0f;
	//		if (is_sea(i, j) == 1) {
	//			int colorIndex = getIndex(i, j, N);
	//			//initMesh.color[colorIndex][0] = 0.f;
	//			//initMesh.color[colorIndex][1] = 0.f;
	//			//initMesh.color[colorIndex][2] = 100.f;
	//		}

	//	}
	//}
	//is_sea.fill(0);

	std::cout << "building base stream tree" << std::endl;
	cgp::grid_2D<cgp::int2> newStream = StreamTree::get_base_stream_tree(m, is_sea);

	std::cout << "building lakes" << std::endl;;
	cgp::grid_2D<cgp::int2> newLakes = StreamTree::get_lakes(newStream);

	std::cout << "updating stream tree with lakes" << std::endl;
	std::map<cgp::int2, 
			 std::vector<StreamTree::LakeEdgeHeight>, 
			 lex_order_class> newLakeGraph = StreamTree::get_lake_graph(m, newStream, newLakes);
	get_final_stream_tree_from_lake_graph(newStream, newLakeGraph);

	std::cout << "calculating drainage areas" << std::endl;
	cgp::grid_2D<float> newDrainage = StreamTree::get_drainage_area(newStream);

	std::cout << "applying erosion" << std::endl;
	ErosionScheme::applyErosionStep(m, newStream, newLakes, newDrainage, gui_param);
}