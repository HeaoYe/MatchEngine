#include <MatchEngine/function/render/mesh/vertex_buffer_binder.hpp>

namespace MatchEngine {
    void VertexBufferBinder::buildInputBindings(std::vector<Match::InputBindingInfo> &binding_infos) {
        std::vector<uint32_t> exist_bindings;
        for (auto &binding_info : binding_infos) {
            if (std::find(exist_bindings.begin(), exist_bindings.end(), binding_info.binding) == exist_bindings.end()) {
                exist_bindings.push_back(binding_info.binding);
            }
        }
        first_binding = exist_bindings.size();
        onBuildInputBindings(binding_infos);
    }
}
