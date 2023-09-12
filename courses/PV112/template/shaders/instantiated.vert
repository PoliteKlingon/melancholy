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

// The position of the current vertex that is being processed.
layout(location = 0) in vec3 position;
// The normal of the current vertex that is being processed.
layout(location = 1) in vec3 normal;

// ----------------------------------------------------------------------------
// Output Variables
// ----------------------------------------------------------------------------
// The position forwared to fragment shader.
layout(location = 0) out vec3 fs_position;
// The normal forwared to fragment shader.
layout(location = 1) out vec3 fs_normal;
// The instance ID forwared to fragment shader.
layout(location = 2) out flat int fs_instance_id;

// ----------------------------------------------------------------------------
// Main Method
// ----------------------------------------------------------------------------
void main()
{
	Object object = objects[gl_InstanceID];

	fs_position = vec3(object.model_matrix * vec4(position, 1.0));
	fs_normal = transpose(inverse(mat3(object.model_matrix))) * normal;
	fs_instance_id = gl_InstanceID;

    gl_Position = camera.projection * camera.view * object.model_matrix * vec4(position, 1.0);
}
