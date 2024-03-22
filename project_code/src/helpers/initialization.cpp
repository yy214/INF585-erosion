
#include "initialization.hpp"
#include "../environment.hpp"
#include "../erosion/stream_tree.hpp"
#include "../erosion/erosion_scheme.hpp"

#include "datastructure/gridTools.hpp"

#include "erosion/flood_fill.hpp"
#include "helpers/colorInterp.hpp"

//do not use add file???

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <cotmatrix.h>
#include <iostream>






using namespace cgp;
using namespace colorInterpolation;
using namespace StreamTree;

mesh initialize_plane()
{
	int const N = 6;
    
    cgp::mesh initMesh = mesh_primitive_grid({ -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, N, N);


    vec3 const translation_normal = vec3(0.0, 0.0, 10.0);

    //A simple test for eigen/libigl
    Eigen::MatrixXd V(4, 2);
    V << 0, 0,
        1, 0,
        1, 1,
        0, 1;
    Eigen::MatrixXi F(2, 3);
    F << 0, 1, 2,
        0, 2, 3;
    Eigen::SparseMatrix<double> L;
    igl::cotmatrix(V, F, L);
    std::cout << "Hello, mesh: " << std::endl << L * V << std::endl;

    //size_t const N = shape.position.size();
    for (size_t k = 0; k < N*N; ++k)
    {
            vec3& p_shape = initMesh.position[k];
            float noise = noise_perlin(1.5f * p_shape,10,0.3,1.5);
            

            //stackoverflow.com/questions/10847007/using-the-gaussian-probability-density-function-in-c
            float THE_DISTANCE = (pow((initMesh.position[k][0] - 0.0), 2) + pow((initMesh.position[k][1] - 0.0), 2));  // I must make this nicer later
            //noise *= (1 - pow(THE_DISTANCE,0.2));
            float const gaussian = normal_pdf(THE_DISTANCE, 0.0, 0.3)*0.8;

            p_shape += (gaussian + noise - 1.0) * 0.05 * translation_normal;
            //initMesh.color[k][0] = (p_shape.z - 0.25) * 10.0;
            //initMesh.color[k] = getColor(p_shape[2]);


    }

    
    //initMesh.color[k][0] = (p_shape.z - 0.25) * 10.0;
    //int2 colorTextCoord = int2(5,5);
    //int2 colorTestCoord = getCoord(index, N);
    //int colorIndex = getIndex(colorTextCoord[0], colorTextCoord[1],N);
    //initMesh.color[colorIndex][1] = 100;

    //Visualizing the Stream Trees

    cgp::grid_2D<short> is_sea = floodFill::getfloodBool(initMesh,0);

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (is_sea(i, j) == 1){
                int colorIndex = getIndex(i, j, N);
                initMesh.color[colorIndex][0] = 0.f;
                initMesh.color[colorIndex][1] = 0.f;
                initMesh.color[colorIndex][2] = 100.f;
                //is_sea_bool(i,j) = true;
            }

        }
    }
    //is_sea.fill(0);

    cgp::grid_2D<cgp::int2> newStream = get_base_stream_tree(initMesh, is_sea);
    //std::cout << "builing lakesssssssssss";
    cgp::grid_2D<cgp::int2> newLakes = get_lakes(newStream);
    //std::cout << newLakes;
    //std::cout << "llllllllaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    

    std::map<cgp::int2, std::vector<LakeEdgeHeight>, lex_order_class> newLakeGraph = get_lake_graph(initMesh,newStream,newLakes);
    get_final_stream_tree_from_lake_graph(newStream,newLakeGraph);
 
    cgp::grid_2D<float> newDrainage = get_drainage_area(newStream);
    //std::abort();
    //std::cout << newStream;




    //Erosion stuff
    erosionScheme myErosion = erosionScheme();

    //std::cout << "beeeeeeeeeeeeeeeeeeesssssssssssssssst";
    //std::cout << newLakes;
    myErosion.setHeightMap(initMesh);
    //std::cout << "teesssssssssssssssst";
    //std::cout << myErosion.heightMap;
    //std::cout << newLakeGraph.;
    //std::cout << newDrainage;

    
    
    myErosion.applyErosionStep(initMesh, newStream, 100);
    //std::abort();
    /*myErosion.applyErosionStep(initMesh, newStream, 100);
    myErosion.applyErosionStep(initMesh, newStream, 100);
    myErosion.applyErosionStep(initMesh, newStream, 100);
    myErosion.applyErosionStep(initMesh, newStream, 100);*/

    //std::cout << myErosion.heightMap;




    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            //std::cout << newLakes(i, j) << std::endl;
            if (true) {
                //std::cout << 'ha';
                int colorIndex = getIndex(i, j, N);
                initMesh.color[colorIndex][2] = newDrainage(i, j) / 100.0;

                //Erosion update
                initMesh.position[colorIndex][2] = myErosion.heightMap(i, j) + 1.0;
            }
        }
    }

    return initMesh;
}
mesh initialize_cylinder()
{
	float const h = 1.5f;
    float const radius = 0.4f;
    float const N_sample_circumferential = 80;
    float const N_sample_length = int( h/(2*3.14f*radius)*(N_sample_circumferential-1) + 1 + 0.5f );
    return mesh_primitive_cylinder(radius, {0,-h/2,0}, {0,h/2,0}, N_sample_circumferential, N_sample_length);
}
mesh initialize_sphere()
{
    int const N = 100;
    float const radius = 0.75f;
    return mesh_primitive_sphere(radius, {0,0,0}, N, 2*N);
}
mesh initialize_cube()
{
    int const N=40;
    return mesh_primitive_cubic_grid({0,0,0},{1,0,0},{1,1,0},{0,1,0}, {0,0,1},{1,0,1},{1,1,1},{0,1,1}, N, N, N);
}
mesh initialize_mesh()
{    
    std::string const filename = project::path+"assets/face.obj";
    mesh shape = mesh_load_file_obj(filename);
    for(auto& p : shape.position) 
        p *= 0.5f;
    return shape;
}

///Utility
float normal_pdf(float x, float m, float s) {
    static const float inv_sqrt_2pi = 0.3989422804014327;
    float a = (x - m) / s;

    return inv_sqrt_2pi / s * std::exp(-0.5f * a * a);
};


