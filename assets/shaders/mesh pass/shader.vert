#version 450

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 color;

layout (location = 4) in vec4 instance_location;
layout (location = 5) in vec4 instance_rotation;
layout (location = 6) in vec4 instance_scale;

layout (location = 0) out vec3 frag_pos;
layout (location = 1) out vec3 frag_normal;
layout (location = 2) out vec3 frag_color;
layout (location = 3) out vec2 frag_uv;

layout (binding = 0) uniform Camera {
    mat4 view;
    mat4 project;
    vec4 planes[6];
} camera;

void main() {
    float sin_v = sin(instance_rotation.x), cos_v = cos(instance_rotation.x);
    mat3 rotate_x = mat3(
        cos_v, sin_v, 0,
        -sin_v, cos_v, 0,
        0, 0, 1
    );
    sin_v = sin(instance_rotation.y), cos_v = cos(instance_rotation.y);
    mat3 rotate_y = mat3(
        cos_v, 0, sin_v,
        0, 1, 0,
        -sin_v, 0, cos_v
    );
    sin_v = sin(instance_rotation.z), cos_v = cos(instance_rotation.z);
    mat3 rotate_z = mat3(
        1, 0, 0,
        0, cos_v, sin_v,
        0, -sin_v, cos_v
    );
    mat3 scale = mat3(
        instance_scale.x, 0, 0,
        0, instance_scale.y, 0,
        0, 0, instance_scale.z
    );
    mat3 model = scale * rotate_z * rotate_y * rotate_x;
    frag_pos = instance_location.xyz + model * pos;
    gl_Position = camera.project * camera.view * vec4(frag_pos, 1);
    frag_normal = normalize(transpose(inverse(model)) * normal);
    frag_color = color;
    frag_uv = uv;
}
