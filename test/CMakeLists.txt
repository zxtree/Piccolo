#set(TARGET_NAME TestPiccolo)
set(TARGET_SERVER TestServer)
set(TARGET_CLIENT TestClient)
# file(GLOB GAME_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
# file(GLOB GAME_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${GAME_HEADERS} ${GAME_SOURCES} ${GAME_RESOURCE})

#add_executable(${TARGET_NAME} ${GAME_HEADERS} ${GAME_SOURCES} ${GAME_RESOURCE})
add_executable(${TARGET_SERVER} testserver.h testserver.cpp)
add_executable(${TARGET_CLIENT} testclient.h testclient.cpp)

add_compile_definitions("PICCOLO_ROOT_DIR=${BINARY_ROOT_DIR}")

set_target_properties(${TARGET_SERVER} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "TestServer")
set_target_properties(${TARGET_CLIENT} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "TestClient")
#set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "Game")

target_compile_options(${TARGET_SERVER} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->")
target_compile_options(${TARGET_CLIENT} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->")

include_directories(${ENGINE_ROOT_DIR}/src)
target_link_libraries(${TARGET_SERVER} PUBLIC PiccoloRuntime)
target_link_libraries(${TARGET_CLIENT} PUBLIC PiccoloRuntime)

set(POST_BUILD_COMMANDS
  COMMAND ${CMAKE_COMMAND} -E make_directory "${BINARY_ROOT_DIR}"

  COMMAND ${CMAKE_COMMAND} -E copy_directory "$<TARGET_FILE_DIR:${TARGET_SERVER}>/" "${BINARY_ROOT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "$<TARGET_FILE_DIR:${TARGET_CLIENT}>/" "${BINARY_ROOT_DIR}"
)

add_custom_command(TARGET ${TARGET_SERVER} ${POST_BUILD_COMMANDS})
add_custom_command(TARGET ${TARGET_CLIENT} ${POST_BUILD_COMMANDS})

#precompile
#set global vari used by precompile
# set(PICCOLO_GAME_HEADS “${GAME_HEADERS}” PARENT_SCOPE)