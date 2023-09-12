#version 450

// ----------------------------------------------------------------------------
// Input Variables
// ----------------------------------------------------------------------------

// The buffer with data about camera.
layout(binding = 0, std140) uniform Camera {
	mat4 projection;
	mat4 view;
	vec3 position;
} camera;

// The representation of one light.
struct Light {
	vec4 position;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};
// The buffer with data about all lights.
layout(binding = 1, std430) buffer Lights {
	Light lights[];
};

// The representation of one object.
struct Object {
	mat4 model_matrix;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
	vec4 padding;
};
// The buffer with data about all objects.
layout(binding = 2, std430) buffer Objects {
	Object objects[];
};

// The intpolated position from the vertex shader.
layout(location = 0) in vec3 fs_position;
// The intpolated normal from the vertex shader.
layout(location = 1) in vec3 fs_normal;
// The instance ID from the vertex shader.
layout(location = 2) in flat int fs_instance_id;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------

// The final output color.
layout(location = 0) out vec4 final_color;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
    final_color = vec4(1.0f);
	Object object = objects[fs_instance_id];
    
	vec3 color_sum = vec3(0.0);
	for(int i = 0; i < lights.length(); i++) {
		Light light = lights[i];

		vec3 light_vector = light.position.xyz - fs_position * light.position.w;
		vec3 L = normalize(light_vector);
		vec3 N = normalize(fs_normal);
		vec3 E = normalize(camera.position - fs_position); 
		vec3 H = normalize(L + E);

		float NdotL = max(dot(N, L), 0.0);
		float NdotH = max(dot(N, H), 0.0001);

		vec3 ambient = object.ambient_color.rgb * light.ambient_color.rgb;
		vec3 diffuse = object.diffuse_color.rgb * light.diffuse_color.rgb;
		vec3 specular = object.specular_color.rgb * light.specular_color.rgb;

		vec3 color = ambient.rgb
			+ NdotL * diffuse.rgb
			+ pow(NdotH, object.specular_color.w) * specular;
		color /= dot(light_vector, light_vector);

		color_sum += color;
	}
	
    color_sum = color_sum / (color_sum + 1.0);   // tone mapping
    color_sum = pow(color_sum, vec3(1.0 / 2.2)); // gamma correction
	final_color = vec4(color_sum, object.diffuse_color.a);
}
