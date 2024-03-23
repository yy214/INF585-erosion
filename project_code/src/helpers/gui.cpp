#include "gui.hpp"


bool gui_parameters::display()
{
	ImGui::Checkbox("Display frame", &display_frame);
	ImGui::Checkbox("Wireframe", &display_wireframe);

	// Create interface
	//ImGui::Text("Surface type:"); // Select surface to be deformed
	//int* ptr_surface_type  = reinterpret_cast<int*>(&surface_type); // trick - use pointer to select enum
	bool new_surface = false;
 //   new_surface |= ImGui::RadioButton("Plane",ptr_surface_type, surface_plane); ImGui::SameLine();
 //   new_surface |= ImGui::RadioButton("Cylinder",ptr_surface_type, surface_cylinder); ImGui::SameLine();
 //   new_surface |= ImGui::RadioButton("Sphere",ptr_surface_type, surface_sphere); ImGui::SameLine();
 //   new_surface |= ImGui::RadioButton("Cube",ptr_surface_type, surface_cube);  ImGui::SameLine();
 //   new_surface |= ImGui::RadioButton("Mesh",ptr_surface_type, surface_mesh);

	ImGui::Text("Deformer type:"); // Select the type of deformation to apply
	int* ptr_deformer_type = (int*)&deformer_parameters.type;
	ImGui::RadioButton("Translate",ptr_deformer_type, deform_translate); ImGui::SameLine();
    ImGui::RadioButton("Twist",ptr_deformer_type, deform_twist); ImGui::SameLine();
    ImGui::RadioButton("Scale",ptr_deformer_type, deform_scale);
	
	ImGui::SameLine();
	ImGui::RadioButton("Perlin Noise",ptr_deformer_type, deform_noise_perlin);

	ImGui::Text("Deformer direction:"); // Select the type of deformation to apply
	int* ptr_deformer_direction = (int*)&deformer_parameters.direction;
    ImGui::RadioButton("View space",ptr_deformer_direction, direction_view_space); ImGui::SameLine();
    ImGui::RadioButton("Surface normal",ptr_deformer_direction, direction_surface_normal); ImGui::SameLine();
	ImGui::RadioButton("Always z axis", ptr_deformer_direction, direction_z);

	ImGui::SliderFloat("(In simultation) time interval", &log_dt, -0.7f, -0.01f);
	ImGui::Checkbox("Do 1 erosion (or press E)", &erosion_dummy);
	ImGui::Checkbox("Continuious erosion", &continuous_erosion);

	ImGui::SliderFloat("Parameter k", &param_n, 0.01f, 10.f);
	ImGui::SliderFloat("Parameter m", &param_m, 0.1f, 0.9f);
	//ImGui::SliderFloat("Parameter n", &param_n, 0.2f, 2.f);

	// Select falloff distance using slider
    ImGui::SliderFloat("Falloff distance", &deformer_parameters.falloff, 0.01f, 0.8f);

	return new_surface;
}