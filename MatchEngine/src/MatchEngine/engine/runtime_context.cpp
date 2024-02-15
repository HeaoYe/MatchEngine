#include <MatchEngine/engine/runtime_context.hpp>

namespace MatchEngine {
    RuntimeContext *global_runtime_context = nullptr;

    namespace Renderer {
        std::string output_attachment_name = Match::SWAPCHAIN_IMAGE_ATTACHMENT;
    }
}
