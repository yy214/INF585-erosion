#pragma once

#include "cgp/cgp.hpp"
#include "../deformers/deformers.hpp"

enum surface_type_enum {
	surface_plane,
	surface_cylinder,
	surface_sphere,
	surface_cube,
	surface_mesh
};


struct gui_parameters {
	bool display_frame = true;
	bool display_wireframe = true;

	surface_type_enum surface_type = surface_plane;    // Type of surface to be created
	deformer_parameters_structure deformer_parameters; // Type of deformation to apply

	float param_m = 0.5f;
	float param_n = 1.0f;

	float log_dt = -5.f; // use 10^log_dt as dt
	bool erosion_dummy = false; // automatically becomes false again when pressed (actually just a button)
	bool continuous_erosion = false;

	bool display();
};
