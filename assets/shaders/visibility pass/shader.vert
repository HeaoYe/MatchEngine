#version 450

#extension GL_GOOGLE_include_directive : require

#include "../utils.glsl"

layout (location = 0) in vec3 pos;

layout (location = 1) in vec4 instance_location;
layout (location = 2) in vec4 instance_rotation;
layout (location = 3) in vec4 instance_scale;

layout (location = 0) out flat uint instance_id;

layout (binding = 0) uniform Camera {
    mat4 view;
    mat4 project;
} camera;

void main() {
    mat3 model = compute_model(instance_scale.xyz, instance_rotation.xyz);
    gl_Position = camera.project * camera.view * vec4(instance_location.xyz + model * pos, 1);
    instance_id = gl_InstanceIndex;
}
