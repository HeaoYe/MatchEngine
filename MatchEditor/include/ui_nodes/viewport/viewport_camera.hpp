#pragma once

#include <MatchEngine/function/camera/camera_id.hpp>
#include <glm/vec3.hpp>

namespace MatchEditor {
    class ViewportCamera {
    public:
        ViewportCamera();
        ~ViewportCamera();

        void reset();
        void onTick(float dt);
    private:
        void updateViewMatrix();
    private:
        glm::vec3 location { 0 };
        float yaw { 0 }, pitch { 0 };
        float speed { 10 };
        float sensitivity { 10 };
    };
}
