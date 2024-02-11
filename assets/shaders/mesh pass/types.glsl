struct MeshDescriptor {
    uint lod_count;
    uint lods[6];
    float radius;
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
};
