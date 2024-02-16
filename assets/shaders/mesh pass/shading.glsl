const vec3 point_light_pos = vec3(3, 4, 5);
const vec3 point_light_color = vec3(0.9, 0.9, 0.9);
const vec3 diffuse = vec3(0.2, 0.2, 0.4);

// 着色函数，简单的光照模型
vec4 shading(vec3 pos, vec3 normal, vec2 tex_coord, vec3 color) {
    vec3 L = normalize(point_light_pos - pos);
    vec3 shade_color = max(dot(normal, L), 0) * point_light_color * color + diffuse;
    return vec4(shade_color, 1);
}
