#version 450

#extension GL_GOOGLE_include_directive : require

layout (location = 0) in flat uint instance_id;

layout (location = 0) out uvec2 out_visibility;

layout(early_fragment_tests) in;
void main() {
    // 写入 实例索引 和 三角形索引+1
    // 这样当visibility buffer值为0时，代表那个位置没有被三角形覆盖
    out_visibility = uvec2(instance_id, gl_PrimitiveID + 1);
}
