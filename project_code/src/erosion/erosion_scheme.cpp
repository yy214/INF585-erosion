#include "cgp/cgp.hpp"
//#include "stream_tree.hpp"
#include "erosion_scheme.hpp"
#include "datastructure/gridTools.hpp"
#include <queue>
#include "erosion/stream_tree.hpp"

using namespace cgp;

//void erosionSheme::initializeWithMesh(c

void erosionScheme::setHeightMap(cgp::mesh m)
{
	

	int total = m.position.size();
	int N = std::sqrt(total);

	heightMap = cgp::grid_2D<float>(N);
	
	for (int i = 0; i < m.position.size(); i++) {
		int2 currPoint = getCoord(i, N);
		heightMap(currPoint[0], currPoint[1]) = m.position[i][2];
	}
}

void erosionScheme::applyErosionStep(cgp::mesh& m, cgp::grid_2D<cgp::int2> const& stream_tree, cgp::grid_2D<cgp::int2> lakeCenters,float step=10)
{
	int total = m.position.size();
	int N = std::sqrt(total);
	//int2 POS = int2(0,0);
	int k = 0.5;
	int M = 3;



	//the queue to search all the root nodes / lakes??
	std::queue<int2> rootQueue;

	std::vector<cgp::int2> sorted_vertices = StreamTree::topological_sort(stream_tree);
	std::cout << sorted_vertices.size();

	for (cgp::int2 v : sorted_vertices) {
		

		//std::cout << stream_tree(v);
		//if (stream_tree(v) == cgp::int2(-1,-1)) {

		if (v == lakeCenters(v) && lakeCenters(v) != cgp::int2(-42, -42)) {
			std::cout << "ceeeeeeeeeeeeeeeeeeeeeenter";
			std::cout << v;
			std::cout << "notttttceeenter";
			//std::abort();
			
			rootQueue.push(v);
		}
		

	}

	/*for (node in nodes?):
		if (node is root):
			rootQueue.push(node)*/
	

	cgp::grid_2D<float> newHeightMap = cgp::grid_2D<float>(N);

	cgp::grid_2D<float> areaMap = cgp::grid_2D<float>(N);



	//I don't even think we need a new heightmap
	//for (int i = 0; i < m.position.size(); i++) {
	std::cout << rootQueue.size();
	std::cout << "queueueueueueueu";
	

	while (!rootQueue.empty()) {

		//std::cout << rootQueue.front();
		//rootQueue.pop();


		std::cout << "iter";
		int2 currPoint = rootQueue.front();
		rootQueue.pop();
		
		std::cout << currPoint;
		int colorIndex = getIndex(currPoint[0], currPoint[1], N);
		m.color[colorIndex] = cgp::vec3(100.0, 0.0, 0.0);

		////must check if the node)) a reciever or not
		//float piPJ = 1.0;
		//float kAm = k * std::pow(areaMap(currPoint[0], currPoint[1]), M);
		////here we assume uplift is 0;
		int2 jPoint = stream_tree(currPoint);
		std::cout << jPoint;
		//
		////int2 currPoint = getCoord(i, N);
		//heightMap(currPoint[0], currPoint[1]) = (heightMap(currPoint[0], currPoint[1]) + 
		//	step * (0.0 + kAm * heightMap(jPoint[0], jPoint[1])) ) /
		//	(1.0 + kAm *step / piPJ);
	}


}
