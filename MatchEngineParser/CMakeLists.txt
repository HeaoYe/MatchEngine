project(MatchEngineParser)

function (MatchEngineParseHeaders TARGET_NAME INPUT_DIR OUTPUT_DIR)
    file(GLOB_RECURSE headers CONFIGURE_DEPENDS ${INPUT_DIR}/*.hpp ${INPUT_DIR}/*.h $ENV{MATCHENGINE_INCLUDE_DIR}/*.hpp $ENV{MATCHENGINE_INCLUDE_DIR}/*.h)

    set(PYTHON_BIN python)

    add_custom_target(${TARGET_NAME}_AUTO_GENERATE)
    set(ENV{${TARGET_NAME}_OUTPUT_DIR} ${CMAKE_CURRENT_LIST_DIR}/${OUTPUT_DIR})

    add_custom_command(
        TARGET ${TARGET_NAME}_AUTO_GENERATE
        PRE_BUILD
        COMMAND ${PYTHON_BIN} ${MatchEngineParserPath}/parser.py $ENV{${TARGET_NAME}_OUTPUT_DIR} ${headers}
    )
endfunction ()


function (MatchEngineSolveDependency TARGET_NAME)
    add_dependencies(${TARGET_NAME} ${TARGET_NAME}_AUTO_GENERATE)
    add_dependencies(${TARGET_NAME}_AUTO_GENERATE MatchEngine)
    target_include_directories(${TARGET_NAME} PRIVATE $ENV{${TARGET_NAME}_OUTPUT_DIR}/include)
endfunction ()
