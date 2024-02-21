// 着色函数
vec4 shading(vec3 pos, vec3 normal, vec2 tex_coord, vec3 color) {
    uvec2 tile = uvec2(floor(vec2(gl_FragCoord.xy) / 32.0));
    uint tile_id = tile.x + tile.y * tile_size.x;
    uint offset = tile_id * 1024;
    uint light_count = tile_point_light_counts[tile_id];
    vec3 diffuse = vec3(0);
    // 计算点光源
    for (int light_index_idx = 0; light_index_idx < light_count; light_index_idx ++) {
        uint light_index = tile_point_light_indices[offset + light_index_idx];
        PointLight point_light = point_lights[light_index];

        vec3 L = normalize(point_light.position__radius.xyz - pos);
        float D = length(point_light.position__radius.xyz - pos);
        vec3 shade_color = step(0, dot(normal, L)) * point_light.color__pad.xyz;
        float f_dist = pow(max(1 - pow(D / point_light.position__radius.w, 2), 0), 2);
        diffuse += shade_color * f_dist;
    }
    return vec4(diffuse, 1);
}
