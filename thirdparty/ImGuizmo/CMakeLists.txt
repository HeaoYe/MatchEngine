project(ImGuizmo)

file(GLOB source CONFIGURE_DEPENDS ImGuizmo/*.cpp)
# file(GLOB source CONFIGURE_DEPENDS imgui/backends/imgui_impl_glfw.cpp)

add_library(ImGuizmo SHARED ${source})
target_include_directories(ImGuizmo PUBLIC ImGuizmo)
target_include_directories(ImGuizmo PRIVATE ../Match/thirdparty/imgui/imgui)

# link glfw
# target_link_libraries(ImGuizmo PUBLIC glfw)
# target_include_directories(ImGuizmo PUBLIC "${GLFW_SOURCE_DIR}/deps")
