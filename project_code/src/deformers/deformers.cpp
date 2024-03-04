#include "deformers.hpp"



using namespace cgp;

void apply_deformation(mesh& shape, numarray<vec3> const& position_before_deformation, vec2 const& translate_screen, vec3 const& picked_position, vec3 const& picked_normal, rotation_transform const& camera_orientation, deformer_parameters_structure const& deformer_parameters)
{

	/** 
		shape:  The position of shape are the one to be deformed
		position_before_deformation:  Reference position of the mesh before applying this deformation
		translate_screen:   Input translation of the user in the 2D-screen coordinates - tr must be converted into a 3D transformation applied to the positions of shape
		picked_position:    3D Position of the picked vertex
		picked_normal:      Normal of the surface at the picked vertex position
		camera_orientation: Current camera orientation - allows to convert the 2D-screen coordinates into 3D coordinates
	*/


	float const r = deformer_parameters.falloff; // radius of influence of the deformation
	size_t const N = shape.position.size();
	for (size_t k = 0; k < N; ++k)
	{
		vec3& p_shape = shape.position[k];                             // position to deform
		vec3 const& p_shape_original = position_before_deformation[k]; // reference position before deformation
		vec3 const& p_clicked = picked_position;                       // 3D position of picked position
		vec3 const& n_clicked = picked_normal;                         // normal of the surface (before deformation) at the picked position

		float const dist = norm(p_clicked - p_shape_original);         // distance between the picked position and the vertex before deformation


		float const w = std::exp(-dist * dist / (r * r));
		if (w > 0.001f)
		{
			if (deformer_parameters.type == deform_translate)
			{
				vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
				if (deformer_parameters.direction == direction_view_space)
					p_shape = p_shape_original + w * translation;

				else if (deformer_parameters.direction == direction_surface_normal) {
					if (norm(translation) > 1.0e-3f) {
						vec3 const translation_normal = dot(translation, n_clicked) * n_clicked;
						p_shape = p_shape_original + w * translation_normal;
					}
				}

				else if(deformer_parameters.direction == direction_z){
					p_shape = p_shape_original + w * translate_screen.y * vec3(0, 0, 1);
				}

			}
			if (deformer_parameters.type == deform_twist)
			{
				float const angle = -w * translate_screen.x * 2 * Pi;
				rotation_transform R;
				if (deformer_parameters.direction == direction_view_space)
				{
					vec3 const camera_front = camera_orientation.matrix_col_z();
					R = rotation_transform::from_axis_angle(camera_front, angle);
				}
				if (deformer_parameters.direction == direction_surface_normal)
				{
					R = rotation_transform::from_axis_angle(n_clicked, angle);
				}
				p_shape = p_clicked + R * (p_shape_original - p_clicked);
			}

			if (deformer_parameters.type == deform_scale)
			{
				p_shape = p_clicked + (1 + translate_screen.x * w) * (p_shape_original - p_clicked);
			}

			if (deformer_parameters.type == deform_noise_perlin) {
				vec3 const translation = camera_orientation * vec3(translate_screen, 0.0f);
				vec3 const translation_normal = norm(translation) * dot(normalize(translation), n_clicked) * n_clicked;
				float const noise = noise_perlin(10.5f * p_shape_original);
				p_shape = p_shape_original + w * noise * translation_normal;
			}

		}
	}
}