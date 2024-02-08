#version 450

layout (location = 0) in vec2 ndc_pos;

layout (set = 0, binding = 0) uniform sampler2D output_render_target;

layout (location = 0) out vec4 out_color;

void main() {
    vec2 uv = ndc_pos * 0.5 + 0.5;
    uv.y = 1 - uv.y;
    out_color = vec4(texture(output_render_target, uv).xyz, 1);
}
