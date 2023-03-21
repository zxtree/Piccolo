cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(Piccolo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)
# set(CMAKE_VERBOSE_MAKEFILE ON)

include(CMakeDependentOption)

# ---- Include guards ----
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

set(PICCOLO_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_INSTALL_PREFIX "${PICCOLO_ROOT_DIR}/bin")
set(BINARY_ROOT_DIR "${CMAKE_INSTALL_PREFIX}/")

set(DEPLOY_CONFIG_DIR "configs/deployment")
set(DEVELOP_CONFIG_DIR "configs/development")

set(ENGINE_ROOT_DIR "${PICCOLO_ROOT_DIR}/engine")
set(THIRD_PARTY_DIR "${PICCOLO_ROOT_DIR}/3rdparty")
set(ASSETS_DIR "${PICCOLO_ROOT_DIR}/assets")

if(ENABLE_PHYSICS_DEBUG_RENDERER)
  set(JOLT_ASSET_DIR "${ASSETS_DIR}/jolt-asset")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options("/MP" "$<$<CONFIG:DEBUG>:/MDd>" "$<$<CONFIG:RELEASE>:/MD>")
endif()

link_directories($<$<CONFIG:DEBUG>:${PICCOLO_ROOT_DIR}/lib_debug/lib> $<$<CONFIG:RELEASE>:${PICCOLO_ROOT_DIR}/lib_release/lib>)
include_directories($<$<CONFIG:DEBUG>:${PICCOLO_ROOT_DIR}/lib_debug/include> $<$<CONFIG:RELEASE>:${PICCOLO_ROOT_DIR}/lib_release/include>)

set(vulkan_include ${THIRD_PARTY_DIR}/VulkanSDK/include)
set(vulkan_lib ${THIRD_PARTY_DIR}/VulkanSDK/lib/Win32/vulkan-1.lib)
set(glslangValidator_executable ${THIRD_PARTY_DIR}/VulkanSDK/bin/Win32/glslangValidator.exe)
add_compile_definitions("PICCOLO_VK_LAYER_PATH=${THIRD_PARTY_DIR}/VulkanSDK/bin/Win32")

set(SHADER_COMPILE_TARGET PiccoloShaderCompile)
#add_subdirectory(assets/shader)
#add_subdirectory(3rdparty)

#add_subdirectory(engine)
add_subdirectory(test)
# add_subdirectory(editor)
#add_subdirectory(game)