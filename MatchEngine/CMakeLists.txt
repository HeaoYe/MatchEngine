project(MatchEngine)

file(GLOB_RECURSE source CONFIGURE_DEPENDS src/MatchEngine/*.cpp)


add_library(MatchEngine SHARED ${source})

# MatchEngineParseHeaders(MatchEngine include .)

target_link_libraries(MatchEngine PUBLIC Match)

target_include_directories(MatchEngine PUBLIC include)
target_include_directories(MatchEngine PRIVATE src/MatchEngine)
