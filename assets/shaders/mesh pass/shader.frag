#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout (location = 0) in vec3 frag_pos;
layout (location = 1) in vec3 frag_normal;
layout (location = 2) in vec3 frag_color;
layout (location = 3) in vec2 frag_uv;

layout (location = 0) out vec4 out_color;

const vec3 point_light_pos = vec3(3, 4, 5);
const vec3 point_light_color = vec3(0.4, 0.7, 0.5);
const vec3 albedo = vec3(0.2, 0.2, 0.4);

void main() {
    vec3 L = normalize(point_light_pos - frag_pos);
    vec3 color = max(dot(L, frag_normal), 0) * point_light_color + albedo;
    out_color = vec4(color, 1);
}
