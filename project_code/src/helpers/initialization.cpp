
#include "initialization.hpp"
#include "../environment.hpp"
#include "../erosion/stream_tree.hpp"
#include "../erosion/erosion_scheme.hpp"

#include "datastructure/gridTools.hpp"

#include "erosion/flood_fill.hpp"
#include "helpers/colorInterp.hpp"

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <cotmatrix.h>
#include <iostream>



using namespace cgp;
using namespace colorInterpolation;
using namespace StreamTree;

/// <summary>
/// Initializes a plane with an island and a sea
/// </summary>
/// <returns></returns>
mesh initialize_plane()
{
    // 70 nodes seem to decent for finding good stable parameters
	int const N = 300;
    
    // The Initial Mesh is a flat plane
    cgp::mesh initMesh = mesh_primitive_grid({ -1,-1,0 }, { 1,-1,0 }, { 1,1,0 }, { -1,1,0 }, N, N);

    // Vectors for constructing the initial terrain
    vec3 const translation_normal = vec3(0.0, 0.0, 10.0);
    vec3 const translation_x = vec3(10.0, 0.0, 10.0);
    vec3 const translation_y = vec3(0.0, 10.0, 0.0);

    // Iterating over all the grid vertices
    for (size_t k = 0; k < N*N; ++k)
    {
            // Getting noise data from the 2-D Perlin function
            vec3& p_shape = initMesh.position[k];
            float noise = noise_perlin(1.5f * p_shape,10,0.3,1.5);

            //SOURCE: stackoverflow.com/questions/10847007/using-the-gaussian-probability-density-function-in-c
            float THE_DISTANCE = (pow((initMesh.position[k][0] - 0.0), 2) + pow((initMesh.position[k][1] - 0.0), 2));
            float const gaussian = normal_pdf(THE_DISTANCE, 0.0, 0.3)*0.8;

            p_shape += (gaussian + noise - 1.0) * 0.05 * translation_normal;

            //Just to add a bit of random noise (Note: these numbers are indeed correct)
            //p_shape += translation_x * (rand() - 0.5) * 0.00000006f * float(100.f/N);
            //p_shape += translation_y *(rand() - 0.5) * 0.00000006f * float(100.f / N);

            initMesh.color[k] = getColor(p_shape[2]);

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


