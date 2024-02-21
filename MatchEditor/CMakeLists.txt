project(MatchEditor)

MatchEngineParseHeaders(
    MatchEditor
    include
    src/auto_generated
)

file(GLOB_RECURSE source CONFIGURE_DEPENDS src/*.cpp)

add_executable(MatchEditor ${source})

target_compile_definitions(MatchEditor PRIVATE MATCHEDITOR_INNER_VISIBLE)
target_compile_definitions(MatchEditor PRIVATE MATCH_INNER_VISIBLE)
target_link_libraries(MatchEditor PRIVATE MatchEngine)
target_link_libraries(MatchEditor PRIVATE ImGuizmo)
target_include_directories(MatchEditor PRIVATE include)

MatchEngineSolveDependency(MatchEditor)
