#version 450

#extension GL_EXT_nonuniform_qualifier : require

layout (location = 0) in vec3 frag_normal;
layout (location = 1) in vec3 frag_color;
layout (location = 2) in vec2 frag_uv;
layout (set = 0, binding = 1) uniform writeonly image2D render_targets[];

layout (set = 1, binding = 0) uniform DrawCallData {
    mat4 view;
    mat4 project;
    uint render_target_index;
} data;

void main() {
    imageStore(render_targets[data.render_target_index], ivec2(gl_FragCoord.xy), vec4(normalize(frag_normal) * 0.5 + 0.5, 1));
    // imageStore(render_targets[data.render_target_index], ivec2(gl_FragCoord.xy), vec4(1, 1, 1, 1));
}
