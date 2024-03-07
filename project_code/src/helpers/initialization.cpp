
#include "initialization.hpp"
#include "../environment.hpp"
#include "../datastructure/HalfedgeBuilder.h"
#include "../datastructure/HalfedgeDS.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <cotmatrix.h>
#include <iostream>



using namespace cgp;

mesh initialize_plane()
{
	int const N = 100;
    
    /////////////DIMITRI CODE
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

    //The Half-edge data structure
    Eigen::MatrixXi eigenConnectivity(initMesh.connectivity.size(), 3);
    for (int i = 0; i < eigenConnectivity.rows(); i++) {
        Eigen::Vector3i nextRow;
        nextRow << initMesh.connectivity[i][0], initMesh.connectivity[i][1], initMesh.connectivity[i][2];
        eigenConnectivity.row(i) = nextRow;
    }
    HalfedgeBuilder builder = HalfedgeBuilder();
    HalfedgeDS eigenMesh = builder.createMesh(eigenConnectivity.rows(), eigenConnectivity);

    //Testing the Structure
    std::cout << eigenMesh.getEdge(23);
    std::cout << "aaaaaaaaaaaaaaa";


    //size_t const N = shape.position.size();
    for (size_t k = 0; k < N*N; ++k)
    {
            vec3& p_shape = initMesh.position[k];
            float const noise = noise_perlin(1.5f * p_shape,10,0.4);

            p_shape += noise * 0.05 * translation_normal;
            initMesh.color[k][0] = (p_shape.z - 0.25) * 10.0;
    }

    return initMesh;
    //////////////////END DIMITRI CODE
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

