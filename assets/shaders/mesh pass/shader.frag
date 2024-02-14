#version 450

layout (location = 0) in vec3 frag_pos;
layout (location = 1) in vec3 frag_normal;
layout (location = 2) in vec3 frag_color;
layout (location = 3) in vec2 frag_uv;
layout (location = 4) in flat uint draw_id;

layout (location = 0) out vec4 out_color;

struct FragInfo {
    uint draw_id;
    uint triangle_id;
};

layout (binding = 1) writeonly buffer visibility_buffer {
    FragInfo frag_infos[];
};

const vec3 point_light_pos = vec3(3, 4, 5);
const vec3 point_light_color = vec3(0.4, 0.7, 0.5);
const vec3 deffuse = vec3(0.2, 0.2, 0.4);

void main() {
    // 显示Lod的颜色
    // out_color = vec4(frag_color, 1);

    vec3 L = normalize(point_light_pos - frag_pos);
    vec3 color = max(dot(L, normalize(frag_normal)), 0) * point_light_color + deffuse;
    out_color = vec4(color, 1);

    // atomicExchange(frag_infos[int(gl_FragCoord.y * 1920 + gl_FragCoord.x)].draw_id, draw_id);
    // atomicExchange(frag_infos[int(gl_FragCoord.y * 1920 + gl_FragCoord.x)].triangle_id, gl_PrimitiveID);
    // uint n = ((draw_id & 0xfff) << 12) | (gl_PrimitiveID & 0xfffff);
    // out_vb = vec4(
    //     ((n & 0xff) << 0) / 255,
    //     ((n & 0xff00) << 8) / 255,
    //     ((n & 0xff0000) << 16) / 255,
    //     ((n & 0xff000000) << 24) / 255
    // );
}
