cmake_minimum_required(VERSION 3.20)

project(ratel
    VERSION 1.0.0
    DESCRIPTION "A lightweight C++20 application framework"
    LANGUAGES CXX
)

include(GNUInstallDirs)

# ---------------------------------------------------------------------------
# 安装前缀与安装目录
# ---------------------------------------------------------------------------
# 仅在用户/预设未指定前缀时给出项目默认值，避免写死缓存覆盖外部配置。
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install"
        CACHE PATH "Ratel install prefix" FORCE)
endif()

set(RATEL_INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}")
set(RATEL_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}")
set(RATEL_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")

# ---------------------------------------------------------------------------
# 语言与运行时约定（对单配置/多配置生成器一致）
# ---------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Debug 产物统一追加 _d，替代各目标零散设置 DEBUG_POSTFIX。
set(CMAKE_DEBUG_POSTFIX "_d")

if(MSVC)
    # 与 vcpkg x64-windows（动态 CRT）保持一致，避免跨配置 /MD /MDd 混用。
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# 多配置生成器会自行追加配置子目录；显式使用 $<CONFIG> 使各生成器输出一致。
set(RATEL_OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${RATEL_OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${RATEL_OUTPUT_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${RATEL_OUTPUT_DIR}")

# ---------------------------------------------------------------------------
# 依赖
# ---------------------------------------------------------------------------
find_package(spdlog CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

# ---------------------------------------------------------------------------
# 公共编译要求：用目标替代全局 include_directories / add_compile_options
# ---------------------------------------------------------------------------
add_library(ratel_build_settings INTERFACE)
set_target_properties(ratel_build_settings PROPERTIES EXPORT_NAME build_settings)
target_compile_features(ratel_build_settings INTERFACE cxx_std_20)
target_include_directories(ratel_build_settings INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
if(MSVC)
    target_compile_options(ratel_build_settings INTERFACE
        /wd4996
        /wd4275
        /wd4251
    )
endif()

# ---------------------------------------------------------------------------
# 目标
# ---------------------------------------------------------------------------
add_subdirectory(src/ratel/basic)
add_subdirectory(src/ratel/geometry)
add_subdirectory(src/ratel/asio)
add_subdirectory(src/ratel/system)

option(RATEL_BUILD_TESTS "Build the ratel test executable" ON)
if(RATEL_BUILD_TESTS)
    add_subdirectory(src/ratel/test)
endif()

# ---------------------------------------------------------------------------
# 包导出：外部工程 find_package(ratel CONFIG) 后可使用 ratel::basic 等目标
# ---------------------------------------------------------------------------
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ratelConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ratelConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/ratelConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ratel"
)

install(TARGETS ratel_build_settings EXPORT ratelTargets)

install(EXPORT ratelTargets
    FILE ratelTargets.cmake
    NAMESPACE ratel::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ratel"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/ratelConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/ratelConfigVersion.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/ratel"
)

# 第三方运行时 DLL 由 vcpkg 的 VCPKG_APPLOCAL_DEPS 与
# X_VCPKG_APPLOCAL_DEPS_INSTALL 负责，不再按 CMAKE_BUILD_TYPE 手工安装。
