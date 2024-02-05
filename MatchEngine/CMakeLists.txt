project(MatchEngine)

file(GLOB_RECURSE source CONFIGURE_DEPENDS src/MatchEngine/*.cpp)


add_library(MatchEngine SHARED ${source})

set(ENV{MATCHENGINE_INCLUDE_DIR} ${CMAKE_CURRENT_LIST_DIR}/include) 
# MatchEngineParseHeaders(MatchEngine include .)

target_link_libraries(MatchEngine PUBLIC Match)

target_include_directories(MatchEngine PUBLIC include)
target_include_directories(MatchEngine PRIVATE src/MatchEngine)
