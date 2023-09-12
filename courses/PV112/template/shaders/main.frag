#version 450

layout(binding = 0, std140) uniform Camera {
    mat4 projection;
    mat4 view;
    vec3 position;
}
camera;

layout(binding = 1, std140) uniform MainLight {
    vec4 position;
    vec4 ambient_color;
    vec4 diffuse_color;
    vec4 specular_color;
}
main_light;

layout(binding = 2, std140) uniform Object {
    mat4 model_matrix;

    vec4 ambient_color;
    vec4 diffuse_color;
    vec4 specular_color;
}
object;

struct Light {
	vec4 position;
	vec4 ambient_color;
	vec4 diffuse_color;
	vec4 specular_color;
};

layout(binding = 3, std430) buffer Lights {
	Light lights[];
};

layout(location = 3) uniform bool has_texture = false;
layout(location = 4) uniform int x_repeat = 1;
layout(location = 5) uniform int y_repeat = 1;
layout(location = 6) uniform bool has_normal_map = false;
layout(location = 7) uniform bool moving_texture = false;
layout(location = 8) uniform float moving_factor = 0.0;
layout(location = 9) uniform bool blink = false;
layout(location = 10) uniform bool light_reflection = false;

layout(binding = 3) uniform sampler2D albedo_texture;
layout(binding = 4) uniform sampler2D normal_map;

layout(location = 0) in vec3 fs_position;
layout(location = 1) in vec3 fs_normal;
layout(location = 2) in vec2 fs_texture_coordinate;

layout(location = 0) out vec4 final_color;

mat3 create_tbn_matrix(vec2 tex_coord)
{
    // get edge vectors of the pixel triangle
    vec3 pos_vec_x = dFdx(fs_position);
    vec3 pos_vec_y = dFdy(fs_position);
    vec2 tex_vec_x = dFdx(tex_coord);
    vec2 tex_vec_y = dFdy(tex_coord);

    // solve the linear system
    vec3 pos_vec_y_perp = cross(pos_vec_y, fs_normal);
    vec3 pos_vec_x_perp = cross(fs_normal, pos_vec_x);
    vec3 T = pos_vec_y_perp * tex_vec_x.x + pos_vec_x_perp * tex_vec_y.x;
    vec3 B = pos_vec_y_perp * tex_vec_x.y + pos_vec_x_perp * tex_vec_y.y;

    // construct a scale-invariant frame 
    float invmax = inversesqrt(max(dot(T,T), dot(B,B)));
    return mat3(T * invmax, B * invmax, fs_normal);
}

vec3 add_color(Light light, bool cone, vec2 repeated_texture_coordinate, vec3 specular_color, float specular_exp) {
    vec3 light_vector = light.position.xyz - fs_position * light.position.w;
        vec3 L = normalize(light_vector);
        vec3 res_normal = fs_normal;

        mat3 tbn = create_tbn_matrix(repeated_texture_coordinate);

        if (has_normal_map) {
            vec3 normal = texture(normal_map, repeated_texture_coordinate).rgb * 2.0 - 1.0;
            res_normal = tbn * normal;
        }

        vec3 N = normalize(res_normal);
        vec3 E = normalize(camera.position - fs_position);
        vec3 H = normalize(L + E);

        vec4 light_ambient_color = light.ambient_color;
        vec4 light_diffuse_color = light.diffuse_color;
        vec4 light_specular_color = light.specular_color;
        
        if (cone) {
            float horizontal_dist = (sqrt(light_vector.x * light_vector.x + light_vector.z * light_vector.z));
            if (light_vector.y < horizontal_dist / 4) {
                float factor = 0.0;
                if (light_vector.y > horizontal_dist / 20) {
                    factor = (light_vector.y - horizontal_dist / 20) / (horizontal_dist / 4 - horizontal_dist / 20);
                }
                
                light_ambient_color = vec4(0.001);
                light_diffuse_color /= 1.0 + ((1 - factor) * 10.0);
                light_specular_color /= 1.0 + ((1 - factor) * 10.0);
            }
        }

        float NdotL = max(dot(N, L), 0.0);
        float NdotH = max(dot(N, H), 0.0001);

        vec3 ambient = object.ambient_color.rgb * light_ambient_color.rgb;
        vec3 diffuse = object.diffuse_color.rgb * (has_texture ? texture(albedo_texture, repeated_texture_coordinate).rgb : vec3(1.0)) * light_diffuse_color.rgb;
        vec3 specular = specular_color * light_specular_color.rgb;

        vec3 color = ambient.rgb + NdotL * diffuse.rgb + pow(NdotH, specular_exp) * specular;

        if (light.position.w == 1.0) {
            color /= (dot(light_vector, light_vector)); //attenuation (distance ^ 2)
        }

        color = color / (color + 1.0);       // tone mapping
        color = pow(color, vec3(1.0 / 2.2)); // gamma correction
        return color;
}

void main() {
    vec3 color_sum = vec3(0.0);

    vec2 repeated_texture_coordinate = vec2(fs_texture_coordinate.x * x_repeat, fs_texture_coordinate.y * y_repeat);

    float move_texture_by = normalize(fs_normal) == vec3(0.0f, 1.0f, 0.0f) ? -moving_factor : moving_factor; //clouds movement fix

    if (moving_texture) {
        repeated_texture_coordinate.y += move_texture_by;
    }

    for(int i = 0; i < lights.length(); i++) {
        if (i < 10) {
            color_sum += add_color(lights[i], true, repeated_texture_coordinate, object.specular_color.rgb, object.specular_color.w);
        } else if (i < 13) {
            vec3 light_vector = lights[i].position.xyz - fs_position;
            if (!light_reflection || (!blink && i == 12) || light_vector.x > 0) {
                continue;
            }
            float dim = clamp(abs(light_vector.z) / abs(light_vector.x), 0.0, 1.0);
            color_sum += add_color(lights[i], false, repeated_texture_coordinate, object.specular_color.rgb * (1 - dim), object.specular_color.w / 5);
        } else if (i < 16) {
            vec3 light_vector = lights[i].position.xyz - fs_position;
            if (!light_reflection || (!blink && i == 15) || light_vector.x < 0) {
                continue;
            }
            float dim = clamp(abs(light_vector.z) / abs(light_vector.x), 0.0, 1.0);
            color_sum += add_color(lights[i], false, repeated_texture_coordinate, object.specular_color.rgb * (1 - dim), object.specular_color.w / 5);
        }
    }

    color_sum += add_color(
        Light(
            main_light.position, 
            main_light.ambient_color, 
            main_light.diffuse_color, 
            main_light.specular_color
            ), 
            false, repeated_texture_coordinate, object.specular_color.rgb, object.specular_color.w
            );

    float transparency = has_texture ? texture(albedo_texture, repeated_texture_coordinate).a * object.diffuse_color.a : object.diffuse_color.a;

    final_color = vec4(color_sum, transparency);
}
