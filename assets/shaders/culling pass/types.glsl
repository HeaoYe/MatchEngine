struct MeshDescriptor {
    uint lod_count;
    uint lods[7];
    vec4 aabb_min__radius;
    vec4 aabb_max__pad;
};

struct PrimitiveDescriptor {
    int vertex_offset;
    uint first_index;
    uint index_count;
    float pad;
};

struct MeshInstance {
    vec3 location;
    vec3 rotation;
    vec3 scale;
    uint mesh_id;
};

struct VkDrawIndexedIndirectCommand {
    uint index_count;
    uint instance_count;
    uint first_index;
    int vertex_offset;
    uint first_instance;
};

layout (push_constant) uniform Constants {
    uint mesh_instance_count;
    uint primitive_count;
    uvec2 depth_texture_size;
    uint selected_mesh_instance_index;
};

layout (std430, binding = 0, set = 1) readonly buffer MeshInstanceBuffer {
    MeshInstance mesh_instances[];
};

layout (std430, binding = 1, set = 1) readonly buffer MeshDescriptorBuffer {
    MeshDescriptor mesh_descriptors[];
};

layout (std430, binding = 2, set = 1) readonly buffer PrimitiveDescriptorBuffer {
    PrimitiveDescriptor primitive_descriptors[];
};

layout (binding = 3, set = 1) uniform Camera {
    mat4 view;
    mat4 project;
    float near;
    float far;
} camera;
