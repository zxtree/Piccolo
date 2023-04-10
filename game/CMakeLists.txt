set(TARGET_NAME PiccoloGame)

file(GLOB GAME_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB GAME_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${GAME_HEADERS} ${GAME_SOURCES} ${GAME_RESOURCE})

add_executable(${TARGET_NAME} ${GAME_HEADERS} ${GAME_SOURCES} ${GAME_RESOURCE})

add_compile_definitions("PICCOLO_ROOT_DIR=${BINARY_ROOT_DIR}")

set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 17 OUTPUT_NAME "PiccoloGame")
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER "Game")

target_compile_options(${TARGET_NAME} PUBLIC "$<$<COMPILE_LANG_AND_ID:CXX,MSVC>:/WX->")

target_link_libraries(${TARGET_NAME} PUBLIC PiccoloRuntime)
target_link_libraries(${TARGET_NAME} PUBLIC libconfig)

set(POST_BUILD_COMMANDS
  COMMAND ${CMAKE_COMMAND} -E make_directory "${BINARY_ROOT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "$<TARGET_FILE_DIR:${TARGET_NAME}>/" "${BINARY_ROOT_DIR}"
  COMMAND ${CMAKE_COMMAND} -E remove_directory "${BINARY_ROOT_DIR}/${JOLT_ASSET_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ENGINE_ROOT_DIR}/${JOLT_ASSET_DIR}" "${BINARY_ROOT_DIR}/${JOLT_ASSET_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${ASSETS_DIR}" "${BINARY_ROOT_DIR}/assets"
  COMMAND ${CMAKE_COMMAND} -E copy "${PICCOLO_ROOT_DIR}/configs/development/PiccoloEditor.ini" "${BINARY_ROOT_DIR}/PiccoloEditor.ini"
  COMMAND ${CMAKE_COMMAND} -E copy "${PICCOLO_ROOT_DIR}/build/game/debug/PiccoloGame.exe" "${BINARY_ROOT_DIR}/PiccoloGame.exe"
)

add_custom_command(TARGET ${TARGET_NAME} ${POST_BUILD_COMMANDS})

#precompile
#set global vari used by precompile
set(PICCOLO_GAME_HEADS "${GAME_HEADERS}" PARENT_SCOPE)
