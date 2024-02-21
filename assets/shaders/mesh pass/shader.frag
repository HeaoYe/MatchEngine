#version 450

#extension GL_GOOGLE_include_directive : require

#include "../utils.glsl"
#include "../light pass/point_light.glsl"

struct VkDrawIndexedIndirectCommand {
    uint index_count;
    uint instance_count;
    uint first_index;
    int vertex_offset;
    uint first_instance;
};

layout (binding = 0, input_attachment_index = 0) uniform usubpassInput visibility_buffer;

layout (binding = 1) uniform Camera {
    mat4 view;
    mat4 project;
} camera;

layout (std430, binding = 2) readonly buffer IndexBuffer {
    uint indices[];
};

layout (std430, binding = 3) readonly buffer PositionBuffer {
    float positions[];
};

layout (std430, binding = 4) readonly buffer NormalBuffer {
    float normals[];
};

layout (std430, binding = 5) readonly buffer TexCoordBuffer {
    float tex_coords[];
};

layout (std430, binding = 6) readonly buffer ColorBuffer {
    float colors[];
};

layout (std430, binding = 7) readonly buffer InstanceData {
    vec4 instance_datas[];
};

layout (std430, binding = 8) readonly buffer CountsBuffer {
    uint visible_mesh_instance_count_div_256;
    uint y;
    uint z;
    uint visible_mesh_instance_count;
    uint available_indirect_command_count;
};

layout (std430, binding = 9) readonly buffer AvailableIndirectCommandsBuffer {
    VkDrawIndexedIndirectCommand available_indirect_commands_buffer[];
};

layout (std430, binding = 10) readonly buffer TilePointLightCountBuffer {
    uint tile_point_light_counts[];
};

layout (std430, binding = 11) readonly buffer TilePointLightIndicesBuffer {
    uint tile_point_light_indices[];
};

layout (std430, binding = 12) readonly buffer PointLightPool {
    PointLight point_lights[];
};

layout (push_constant) uniform Constants {
    uvec2 tile_size;
};

layout (location = 0) in vec2 ndc_pos;
layout (location = 0) out vec4 out_color;

#include "shading.glsl"

vec2 interpolate_vec2(mat3x2 attributes, vec3 db_dx, vec3 db_dy, vec2 delta) {
	vec3 attr0 = vec3(attributes[0].x, attributes[1].x, attributes[2].x);
	vec3 attr1 = vec3(attributes[0].y, attributes[1].y, attributes[2].y);
	vec2 attribute_x = vec2(dot(db_dx,attr0), dot(db_dx,attr1));
	vec2 attribute_y = vec2(dot(db_dy,attr0), dot(db_dy,attr1));
	vec2 attribute_s = attributes[0];
	vec2 result = (attribute_s + delta.x * attribute_x + delta.y * attribute_y);
	return result;
}

vec3 interpolate_vec3(mat3 attributes, vec3 db_dx, vec3 db_dy, vec2 delta) {
	vec3 attribute_x = attributes * db_dx;
	vec3 attribute_y = attributes * db_dy;
	vec3 attribute_s = attributes[0];
	return (attribute_s + delta.x * attribute_x + delta.y * attribute_y);
}

float interpolate_float(vec3 attributes, vec3 db_dx, vec3 db_dy, vec2 delta) {
    float attribute_x = dot(attributes, db_dx);
    float attribute_y = dot(attributes, db_dy);
    float attribute_s = attributes[0];
    return (attribute_s + delta.x * attribute_x + delta.y * attribute_y);
}

vec4 compute_out_color(VkDrawIndexedIndirectCommand command, uint instance_id, uint triangle_id) {
    // 获取索引偏移
    uint offset = command.first_index + triangle_id * 3;

    // 获取索引
    uint idx0 = (command.vertex_offset + indices[offset + 0]) * 3;
    uint idx1 = (command.vertex_offset + indices[offset + 1]) * 3;
    uint idx2 = (command.vertex_offset + indices[offset + 2]) * 3;
    uint idx_uv0 = (command.vertex_offset + indices[offset + 0]) * 2;
    uint idx_uv1 = (command.vertex_offset + indices[offset + 1]) * 2;
    uint idx_uv2 = (command.vertex_offset + indices[offset + 2]) * 2;

    // 获取三角形三个顶点的属性
    vec3 position0 = vec3(positions[idx0 + 0], positions[idx0 + 1], positions[idx0 + 2]);
    vec3 position1 = vec3(positions[idx1 + 0], positions[idx1 + 1], positions[idx1 + 2]);
    vec3 position2 = vec3(positions[idx2 + 0], positions[idx2 + 1], positions[idx2 + 2]);
    vec3 normal0 = vec3(normals[idx0 + 0], normals[idx0 + 1], normals[idx0 + 2]);
    vec3 normal1 = vec3(normals[idx1 + 0], normals[idx1 + 1], normals[idx1 + 2]);
    vec3 normal2 = vec3(normals[idx2 + 0], normals[idx2 + 1], normals[idx2 + 2]);
    vec2 tex_coord0 = vec2(tex_coords[idx_uv0 + 0], tex_coords[idx_uv0 + 1]);
    vec2 tex_coord1 = vec2(tex_coords[idx_uv1 + 0], tex_coords[idx_uv1 + 1]);
    vec2 tex_coord2 = vec2(tex_coords[idx_uv2 + 0], tex_coords[idx_uv2 + 1]);
    vec3 color0 = vec3(colors[idx0 + 0], colors[idx0 + 1], colors[idx0 + 2]);
    vec3 color1 = vec3(colors[idx1 + 0], colors[idx1 + 1], colors[idx1 + 2]);
    vec3 color2 = vec3(colors[idx2 + 0], colors[idx2 + 1], colors[idx2 + 2]);

    // 获取实例的变换矩阵和相机的变换矩阵
    mat3 model = compute_model(instance_datas[instance_id * 3 + 2].xyz, instance_datas[instance_id * 3 + 1].xyz);
    mat4 project_view = camera.project * camera.view;

    // 计算世界坐标系下的三角形顶点位置
    vec3 world_pos0 = instance_datas[instance_id * 3 + 0].xyz + model * position0;
    vec3 world_pos1 = instance_datas[instance_id * 3 + 0].xyz + model * position1;
    vec3 world_pos2 = instance_datas[instance_id * 3 + 0].xyz + model * position2;

    // 计算NDC坐标系下的三角形顶点位置
    vec4 ndc_pos0 = project_view * vec4(world_pos0, 1);
    vec4 ndc_pos1 = project_view * vec4(world_pos1, 1);
    vec4 ndc_pos2 = project_view * vec4(world_pos2, 1);

    // 除以w, 归一化
    vec3 one_div_w = 1.0 / vec3(ndc_pos0.w, ndc_pos1.w, ndc_pos2.w);
    ndc_pos0 *= one_div_w.x;
    ndc_pos1 *= one_div_w.y;
    ndc_pos2 *= one_div_w.z;

    // 根据当前像素和三角形顶点位置，计算偏导，用于插值顶点属性
    // 因为在ndc空间计算的偏导，所以插值不用考虑透视矫正
    vec2 v[3] = vec2[](
        ndc_pos0.xy,
        ndc_pos1.xy,
        ndc_pos2.xy
    );
    float d = 1.0 / determinant(mat2(v[2] - v[1], v[0] - v[1]));
	vec3 db_dx = vec3(v[1].y - v[2].y, v[2].y - v[0].y, v[0].y - v[1].y) * d;
	vec3 db_dy = vec3(v[2].x - v[1].x, v[0].x - v[2].x, v[1].x - v[0].x) * d;
    vec2 delta = ndc_pos - v[0];

    // 对所有顶点属性插值
    vec3 pos = interpolate_vec3(mat3(world_pos0, world_pos1, world_pos2), db_dx, db_dy, delta);
    vec3 normal = normalize(interpolate_vec3(mat3(normal0, normal1, normal2), db_dx, db_dy, delta));
    vec2 uv = interpolate_vec2(mat3x2(tex_coord0, tex_coord1, tex_coord2), db_dx, db_dy, delta);
    vec3 color = interpolate_vec3(mat3(color0, color1, color2), db_dx, db_dy, delta);
    // 因为model矩阵可能包含旋转和缩放，所以需要重新计算法向量
    normal = normalize(transpose(inverse(model)) * normal);

    // 进行着色
    return shading(pos, normal, uv, color);
}

void main() {
    // 获取visibility buffer的信息，并判断当前像素上有没有三角形
    uvec2 primitive_info = subpassLoad(visibility_buffer).xy;
    if (primitive_info.x + primitive_info.y == 0) {
        out_color = vec4(0);
        return;
    }

    // 根据实例索引，找到对应的draw call，draw call中包含了绘制该实例所需模型的信息
    for (int i = 0; i < available_indirect_command_count; i ++) {
        VkDrawIndexedIndirectCommand command = available_indirect_commands_buffer[i];
        if(step(command.first_instance, primitive_info.x) * step(primitive_info.x, command.first_instance + command.instance_count - 1) == 1) {
            out_color = compute_out_color(command, primitive_info.x, primitive_info.y - 1);
            return;
        }
    }
}
