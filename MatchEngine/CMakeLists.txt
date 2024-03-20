enable_language(CXX)
project(MatchEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(MatchEngine_CreateSubProject SubProjectName)
    project(MatchEngine-${SubProjectName} CXX)
    file(GLOB_RECURSE ${SubProjectName}-source CONFIGURE_DEPENDS Src/*.cpp)
    file(GLOB_RECURSE ${SubProjectName}-header CONFIGURE_DEPENDS Include/*.hpp Include/*.h)

    add_library(MatchEngine-${SubProjectName} SHARED ${${SubProjectName}-source} ${${SubProjectName}-header})

    target_include_directories(MatchEngine-${SubProjectName} PRIVATE Src)
    target_include_directories(MatchEngine-${SubProjectName} PUBLIC Include)

    set_property(GLOBAL APPEND PROPERTY MatchEngineLibraries MatchEngine-${SubProjectName})
endfunction()

function(MatchEngine_AddDependency SubProjectName DependencyName)
    target_link_libraries(MatchEngine-${SubProjectName} PRIVATE MatchEngine-${DependencyName})
    add_dependencies(MatchEngine-${SubProjectName} MatchEngine-${DependencyName})
endfunction()

function(MatchEngine_LinkThirdparty SubProjectName LibraryName)
    target_link_libraries(MatchEngine-${SubProjectName} PRIVATE ${LibraryName})
endfunction()

function(MatchEngine_Link TargetName)
    get_property(Property_MatchEngineLibraries GLOBAL PROPERTY MatchEngineLibraries)
    target_link_libraries(${TargetName} PRIVATE ${Property_MatchEngineLibraries})
endfunction()

add_subdirectory(Core)
