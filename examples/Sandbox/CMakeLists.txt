project(Sandbox)

file(GLOB_RECURSE source CONFIGURE_DEPENDS src/*.cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Sandbox ${source})
LinkMatchEngine(Sandbox)
