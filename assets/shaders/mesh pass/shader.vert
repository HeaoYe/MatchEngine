#version 450

#extension GL_GOOGLE_include_directive : require

#include "../utils.glsl"

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
    mat3 model = compute_model(instance_scale.xyz, instance_rotation.xyz);
    frag_pos = instance_location.xyz + model * pos;
    gl_Position = camera.project * camera.view * vec4(frag_pos, 1);
    frag_normal = normalize(transpose(inverse(model)) * normal);
    frag_color = color;
    frag_uv = uv;
}
