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

void erosionScheme::applyErosionStep(float step,cgp::mesh& m, cgp::grid_2D<cgp::int2> const& stream_tree, cgp::grid_2D<cgp::int2> lakeCenters, cgp::grid_2D<float> areaMap, )
{
	int total = m.position.size();
	int N = std::sqrt(total);
	//int2 POS = int2(0,0);
	int k = 5;
	int M = 0.5;

	//for now this is constant
	float uplift = 0.001;

	//the queue to search all the root nodes / lakes??
	std::queue<int2> rootQueue;

	std::vector<cgp::int2> sorted_vertices = StreamTree::topological_sort(stream_tree);
	std::reverse(sorted_vertices.begin(), sorted_vertices.end());

	for (cgp::int2 v : sorted_vertices) {
		int colorIndex = getIndex(v[0], v[1], N);
		float newHeight = 0.0f;
		cgp::vec3 posVi = m.position[colorIndex];
		else if (stream_tree(v) == StreamTree::NONE) {
			//it is a sea node
			m.color[colorIndex] = vec3(100.0, 0.0, 0.0);
			newHeight = posVi[2] + uplift;
		}
		else {
			int2 receiver = stream_tree(v);
			float receiver_height = heightMap(receiver);
			if (receiver == StreamTree::SEA) {
				//IT IS AN OUTFLOW
				m.color[colorIndex] = vec3(100.0, 100.0, 100.0);
				receiver_height = 0;
			}
			else {
				//it is a non-root node
				m.color[colorIndex] = vec3(0.0, 100.0, 0.0);
			}

			//float kAm = 
			cgp::vec3 posVj = m.position[getIndex(receiver[0],receiver[1],N)];

			float piPJ = std::sqrt(pow(posVi[0] - posVj[0],2) + pow(posVi[1] - posVj[1], 2));
			float kAm = k * std::pow(areaMap(v[0], v[1]), M);

			
			//here we assume uplift is 0;

			newHeight = (heightMap(v[0], v[1]) + step * (uplift + kAm * receiver_height)) /
				(1.0 + kAm * step / piPJ);

			//heightMap(v[0], v[1]) = -1.0;
			//IT WORKS!!!!
			
			//heightMap(v[0], v[1]) = 0.0;
		}
		heightMap(v[0], v[1]) = newHeight;

	}

	//update the positions
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			//std::cout << newLakes(i, j) << std::endl;
			if (true) {
				//std::cout << 'ha';
				int colorIndex = getIndex(i, j, N);
				//initMesh.color[colorIndex][2] = newDrainage(i, j) * 300.0;

				//Erosion update
				m.position[colorIndex][2] = heightMap(i, j) + 0.0;
			}
		}
	}

	/*for (node in nodes?):
		if (node is root):
			rootQueue.push(node)*/
	

	cgp::grid_2D<float> newHeightMap = cgp::grid_2D<float>(N);

	//cgp::grid_2D<float> areaMap = cgp::grid_2D<float>(N);



	//I don't even think we need a new heightmap
	//for (int i = 0; i < m.position.size(); i++) {
	std::cout << rootQueue.size();
	std::cout << "queueueueueueueu";
	

	//while (!rootQueue.empty()) {

	//	//std::cout << rootQueue.front();
	//	//rootQueue.pop();


	//	std::cout << "iter";
	//	int2 currPoint = rootQueue.front();
	//	rootQueue.pop();
	//	
	//	std::cout << currPoint;
	//	int colorIndex = getIndex(currPoint[0], currPoint[1], N);
	//	m.color[colorIndex] = cgp::vec3(100.0, 0.0, 0.0);

	//	////must check if the node)) a receiver or not
	//	//float piPJ = 1.0;
	//	//float kAm = k * std::pow(areaMap(currPoint[0], currPoint[1]), M);
	//	////here we assume uplift is 0;
	//	int2 jPoint = stream_tree(currPoint);
	//	std::cout << jPoint;
	//	//
	//	////int2 currPoint = getCoord(i, N);
	//	//heightMap(currPoint[0], currPoint[1]) = (heightMap(currPoint[0], currPoint[1]) + 
	//	//	step * (0.0 + kAm * heightMap(jPoint[0], jPoint[1])) ) /
	//	//	(1.0 + kAm *step / piPJ);
	//}


}
