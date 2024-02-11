project(Sandbox)

# 在构建前后分别调用MatchEngineParseHeaders 和 MatchEngineSolveDependency

# 调用解析器自动生成代码
# 会在src目录下遍历所有 头文件, 判断有没有需要反射或者序列化的类, 自动生成代码到src/auto_generated
MatchEngineParseHeaders(
    Sandbox             # TARGET
    src                 # 搜索头文件的根目录
    src/auto_generated  # 自动生成代码的目录
)

file(GLOB_RECURSE source CONFIGURE_DEPENDS src/*.cpp)

add_executable(Sandbox ${source})

target_link_libraries(Sandbox PRIVATE MatchEngine)

# 解决代码生成模块与Sandbox的依赖关系
# 保证选由MatchEngine解析器生成完代码, 再构建Sandbox
MatchEngineSolveDependency(Sandbox)
