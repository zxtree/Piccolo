cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(Piccolo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_VERBOSE_MAKEFILE ON)

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
    add_compile_options("/MP")
    ##set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT PiccoloEditor)
endif()

set(vulkan_include ${THIRD_PARTY_DIR}/VulkanSDK/include)
set(vulkan_lib ${THIRD_PARTY_DIR}/VulkanSDK/lib/Win32/vulkan-1.lib)
set(glslangValidator_executable ${THIRD_PARTY_DIR}/VulkanSDK/bin/Win32/glslangValidator.exe)
add_compile_definitions("PICCOLO_VK_LAYER_PATH=${THIRD_PARTY_DIR}/VulkanSDK/bin/Win32")

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(VCPKG_ROOT "E:/lib/vcpkg")
# set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystem/vcpkg.cmake)

set(VCPKG_LIB_DIR_X64 ${VCPKG_ROOT}/installed/x64-windows/lib)
set(VCPKG_LIB_DIR_X64_DEBUG ${VCPKG_ROOT}/installed/x64-windows/debug/lib)
set(VCPKG_INCLUDE_DIR_X64 ${VCPKG_ROOT}/installed/x64-windows/include)
set(VCPKG_INCLUDE_DIR ${VCPKG_INCLUDE_DIR_X64})
#set(VCPKG_LIB_DIR ${VCPKG_LIB_DIR_X64_DEBUG})
set(VCPKG_LIB_DIR ${VCPKG_LIB_DIR_X64})
include_directories(${VCPKG_INCLUDE_DIR})
#link_directories(${VCPKG_LIB_DIR})

set(SHADER_COMPILE_TARGET PiccoloShaderCompile)
add_subdirectory(assets/shader)
add_subdirectory(3rdparty)

add_subdirectory(engine)
# add_subdirectory(test)
# add_subdirectory(editor)
add_subdirectory(game)