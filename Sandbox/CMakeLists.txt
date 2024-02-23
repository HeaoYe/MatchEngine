project(Sandbox)

file(GLOB_RECURSE source CONFIGURE_DEPENDS src/*.cpp)

add_executable(Sandbox ${source})
MatchEngine_LinkLibraries(Sandbox)
