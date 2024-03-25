enable_language(CXX)
project(MatchEngine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 创建一个子项目
function(MatchEngine_CreateSubProject SubProjectName)
    project(MatchEngine-${SubProjectName} CXX)
    file(GLOB_RECURSE ${SubProjectName}-source CONFIGURE_DEPENDS Src/*.cpp)
    file(GLOB_RECURSE ${SubProjectName}-header CONFIGURE_DEPENDS Include/*.hpp Include/*.h)

    add_library(MatchEngine-${SubProjectName} SHARED ${${SubProjectName}-source} ${${SubProjectName}-header})

    target_include_directories(MatchEngine-${SubProjectName} PRIVATE Src)
    target_include_directories(MatchEngine-${SubProjectName} PUBLIC Include)

    set_property(GLOBAL APPEND PROPERTY MatchEngineLibraries MatchEngine-${SubProjectName})
endfunction()

# 为子项目添加依赖
function(MatchEngine_AddDependency SubProjectName DependencyName)
    target_link_libraries(MatchEngine-${SubProjectName} PRIVATE MatchEngine-${DependencyName})
    add_dependencies(MatchEngine-${SubProjectName} MatchEngine-${DependencyName})
endfunction()

# 为子项目添加私有第三方库
function(MatchEngine_LinkPrivateThirdparty SubProjectName LibraryName)
    target_link_libraries(MatchEngine-${SubProjectName} PRIVATE ${LibraryName})
endfunction()

# 为子项目添加公共第三方库
function(MatchEngine_LinkPublicThirdparty SubProjectName LibraryName)
    target_link_libraries(MatchEngine-${SubProjectName} PUBLIC ${LibraryName})
endfunction()

# 为子项目添加私有宏
function(MatchEngine_AddPrivateDefines SubProjectName Defines)
    target_compile_definitions(MatchEngine-${SubProjectName} PRIVATE ${Defines})
endfunction()

# 为子项目添加公共宏
function(MatchEngine_AddPublicDefines SubProjectName Defines)
    target_compile_definitions(MatchEngine-${SubProjectName} PUBLIC ${Defines})
endfunction()

# 链接MatchEngine
function(LinkMatchEngine TargetName)
    get_property(Property_MatchEngineLibraries GLOBAL PROPERTY MatchEngineLibraries)
    target_link_libraries(${TargetName} PRIVATE ${Property_MatchEngineLibraries})
endfunction()

# 子项目
add_subdirectory(Core)
